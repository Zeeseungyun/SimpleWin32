#include "application.h"
#include "application_delegates.h"
#include <zlog/zlog.h>
#include "../win32helper/windows_with_macro.h"
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"
#include "../stat/simple_stat.h"
#include "tick_manager.h"
#include "key_state.h"

#include "../stage/stage.h"
#include <shape/intersect.h>
#pragma comment(lib, "winmm.lib")

using namespace zee;

//-------------------------------------------------------------------------------------------
//win main
//-------------------------------------------------------------------------------------------
#ifdef UNICODE
#define ZEE_WINMAIN_NAME wWinMain
#else
#define ZEE_WINMAIN_NAME WinMain
#endif

class application_win32 : public application {
	friend application& application::get() noexcept;
	friend int
#if defined(_M_CEE_PURE)
		__clrcall
#else
		WINAPI
#endif
		ZEE_WINMAIN_NAME(
			_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ TCHAR* lpCmdLine,
			_In_ int nShowCmd
		);

	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

public:

	using application::app_config_;
	using application::config_;
	using application::is_started_;

	application_win32() {
		
	}
};

static std::unique_ptr<application_win32> app_inst;

static void create_app_inst() {
	if (!app_inst) {
		app_inst = std::unique_ptr<application_win32>(new application_win32);
	}
}

application& application::get() noexcept {
	create_app_inst();
	return *app_inst;
}

float g_fps = 0.0f;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int
#if defined(_M_CEE_PURE)
__clrcall
#else
WINAPI
#endif
ZEE_WINMAIN_NAME(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ TCHAR* lpCmdLine,
	_In_ int nShowCmd
)
{
	create_app_inst();
	const application::config_data& app_config_data = application::get().config();

	app_inst->instance_handle_ = hInstance;
	const TCHAR* class_name = TEXT("my application");
	{//Register window class.
		WNDCLASS wnd_class;
		wnd_class.cbClsExtra = 0;
		wnd_class.cbWndExtra = 0;
		wnd_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wnd_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		wnd_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wnd_class.hInstance = hInstance;
		wnd_class.lpfnWndProc = (WNDPROC)WndProc;
		wnd_class.lpszClassName = class_name;
		wnd_class.lpszMenuName = NULL;
		wnd_class.style = CS_HREDRAW | CS_VREDRAW;

		RegisterClass(&wnd_class);
	}

	{//Create window
		app_inst->window_handle_ = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,
			class_name,
			app_config_data.app_name.c_str(),
			WS_OVERLAPPEDWINDOW,
			0, 0, 0, 0,
			NULL, (HMENU)NULL, app_inst->instance_handle<HINSTANCE>(), NULL
		);
	}

	WINDOWPLACEMENT pl = {};
	shape::recti rc;
	rc.data[1] = app_config_data.window_size;
	rc += app_config_data.window_position;

	pl.length = sizeof(pl);
	pl.showCmd = app_config_data.maximize ? SW_SHOWMAXIMIZED : SW_NORMAL;
	pl.rcNormalPosition = to_win32(rc);

	SetWindowPlacement(app_inst->window_handle<HWND>(), &pl);

	MSG msg = {};
	{
		using namespace std::chrono;
		using namespace std::chrono_literals;
		const int frame_count = app_config_data.frame_count;
		const std::chrono::duration<float, simple_stat_base::sec_ratio> fps = 1s / (float)frame_count;
		std::chrono::duration<float, simple_stat_base::sec_ratio> cur_frame_time = std::chrono::duration<float, simple_stat_base::sec_ratio>::zero();
		std::chrono::duration<float, simple_stat_base::sec_ratio> accumulated_frame_time = std::chrono::duration<float, simple_stat_base::sec_ratio>::zero();

		zee::simple_stat<> stat;
		while (true) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

				if (msg.message == WM_QUIT || msg.message == WM_DESTROY) {
					break;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);

			} else {
				cur_frame_time += stat.duration();
				stat.reset();

				if (cur_frame_time >= fps || !app_config_data.vsync) {
					auto delta_time = cur_frame_time;
					zee::simple_stat<> tick_stat;

					tick_manager::get().pre_tick.broadcast(delta_time.count());
					delta_time += tick_stat.duration();
					tick_stat.reset();

					key_state::get().internal_use_update(delta_time.count());
					delta_time += tick_stat.duration();
					tick_stat.reset();

					if (key_state::is_down(keys::escape)) {
						PostQuitMessage(0);
						continue;
					}

					tick_manager::get().tick.broadcast(delta_time.count());
					delta_time += tick_stat.duration();
					tick_stat.reset();

					tick_manager::get().post_tick.broadcast(delta_time.count());

					//accumulated_frame_time = math::lerp(cur_frame_time, accumulated_frame_time, 0.1f);
					accumulated_frame_time = cur_frame_time * 0.1f + accumulated_frame_time * 0.9f;
					g_fps = math::reciprocal(accumulated_frame_time.count());
					cur_frame_time = cur_frame_time.zero();


					InvalidateRect(app_inst->window_handle<HWND>(), nullptr, FALSE);
				}
			}
		}
	}

	app_inst->app_config_().save();
	application_delegates::on_destroied().broadcast();
	app_inst.reset();
	return (int)msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	//temp_back_buffer
	static std::shared_ptr<win32gdi::device_context_dynamic> temp_back_buffer;
	if (!temp_back_buffer) {
		temp_back_buffer = std::make_shared< win32gdi::device_context_dynamic>();
		application_delegates::on_client_size_changed().add_sp(temp_back_buffer, &win32gdi::device_context_dynamic::resize);

		application_delegates::on_destroied().add_sp(temp_back_buffer, &win32gdi::device_context_dynamic::clear);
	}

	image_data images;

	//https://docs.microsoft.com/en-us/windows/win32/winmsg/window-notifications
	switch (iMessage) {
	case WM_CREATE:
	{
		app_inst->is_started_ = true;
		application_delegates::on_started().broadcast(application::get());


		return 0;
	}//WM_CREATE

	case WM_DESTROY:
	{
		kill_timer(hWnd, 1);
		kill_timer(hWnd, 2);
		kill_timer(hWnd, 3);
		PostQuitMessage(0);
		return 0;
	}//WM_DESTORY

	//https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-windowposchanged
	case WM_WINDOWPOSCHANGED:
	{
		auto& app_config = app_inst->config_();
		const WINDOWPOS& window_pos = *(WINDOWPOS*)(lParam);
		app_config.window_position.x = (int32)window_pos.x;
		app_config.window_position.y = (int32)window_pos.y;

		RECT rc;
		GetWindowRect(app_inst->window_handle<HWND>(), &rc);
		app_config.window_size = to_zee(rc).size();

		GetClientRect(app_inst->window_handle<HWND>(), &rc);
		app_config.client_size = to_zee(rc).size();

		application_delegates::on_client_size_changed().broadcast(app_config.client_size);
		break;
	}//WM_WINDOWPOSCHANGED

	case WM_SIZE:
	{
		app_inst->config_().maximize = false;
		switch (wParam)
		{
		case SIZE_MAXIMIZED:
			app_inst->config_().maximize = true;
			application_delegates::on_maximized().broadcast();
			break;
		case SIZE_MINIMIZED:
			application_delegates::on_minimized().broadcast();
			break;
		default:
			break;
		}

		return 0;
	}//WM_SIZE

	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1:
			if (timer >= timer_limit) {
				change_var_ending(hWnd);
			}
			else {
				timer++;
			}
			break;
		case 2:
			spawn_stage();
			break;
		case 3:
			spawn_stage(); 
			break;
		}
	}

	case WM_KEYDOWN:
	{
		switch (wParam) {

		}
			if (game_state == title) {
				init_ingame();
				init_monster();
				set_timer(hWnd, 1, timer_cycle);	//게임 시간 표시
				for (int i = 0; i != dig_num; i++) {	//몬스터 스폰 타이머
					set_timer(hWnd, i + 2, spawn_cycle[i]);
				}
				game_state = ingame;
			}
			else if (game_state == ending) {
				game_state = title;
			}
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		mouse_position.x = (int32)(short)LOWORD(lParam);
		mouse_position.y = (int32)(short)HIWORD(lParam);

		for (monster& mon : monsters) {
			if (shape::intersect(mon.get_hitbox(), mouse_position) != shape::collide_type::none) {
				if (mon.get_state() == alive) {
					mon.attacked(hWnd);
					change_var_ingame(hWnd);
				}
			}
		}
		return 0;
	}

	case WM_PAINT:
	{
		win32gdi::device_context_auto temp_dc(hWnd, win32gdi::device_context_auto_type::paint);

		if (temp_back_buffer && temp_back_buffer->is_valid()) {

			//이미지 세팅
			images.init_load_images();
			//공통 배경
			images.back.bit_blt(*temp_back_buffer, {});


			//타이틀
			if (game_state == title) {
				temp_back_buffer->rectangle({ coord_list[title_ending_box_lt], coord_list[title_ending_box_rb] });
				temp_back_buffer->print_text(coord_list[title_st], TEXT("| 버그 두더쥐 잡기 게이임 >_< |"));
				temp_back_buffer->print_text(coord_list[title_st2], TEXT("┎-------------------------------------------------┒"));
				temp_back_buffer->print_text(coord_list[title_st3], TEXT("┖-------------------------------------------------┚"));
				temp_back_buffer->print_text(coord_list[title_st4], TEXT("---아무 키나 눌러 시작---"));
				temp_back_buffer->print_text(coord_list[title_st5], TEXT("만든 이: 유재준"));
			}
			//엔딩
			else if (game_state == ending) {
				temp_back_buffer->rectangle({ coord_list[title_ending_box_lt], coord_list[title_ending_box_rb] });
				temp_back_buffer->print_text(coord_list[ending_st], TEXT("참 잘했어요!"));
				temp_back_buffer->print_text(coord_list[time_taken], TEXT("걸린 시간(초): "));
				temp_back_buffer->print_text(coord_list[score_final], TEXT("점수: "));
				temp_back_buffer->print_text(coord_list[ending_st2], TEXT("---아무 키나 눌러 재시작---"));
				temp_back_buffer->print_text(coord_list[time_taken_num], to_tstring(to_wstring(timer)));
				temp_back_buffer->print_text(coord_list[score_final_num], to_tstring(to_wstring(score)));
				temp_back_buffer->bit_blt(temp_dc, {});
			}
			//인게임
			else if (game_state == ingame) {
				temp_back_buffer->rectangle({ coord_list[stagebox_lt], coord_list[stagebox_rb] });
				temp_back_buffer->print_text(coord_list[stage_st], TEXT("스테이지 : "));
				temp_back_buffer->print_text(coord_list[time_limit_st], TEXT("제한시간 : "));
				temp_back_buffer->print_text(coord_list[kill_remain_clear_st], TEXT("클리어까지 남은 처치 : "));
				temp_back_buffer->print_text(coord_list[kill_remain_st], TEXT("다음 단계까지 남은 처치 : "));
				temp_back_buffer->print_text(coord_list[time_st], TEXT("시간 : "));
				temp_back_buffer->print_text(coord_list[score_st], TEXT("점수 : "));
				temp_back_buffer->print_text(coord_list[spawn_1_speed_st], TEXT("초당 두더쥐1 수 : "));
				temp_back_buffer->print_text(coord_list[spawn_2_speed_st], TEXT("초당 두더쥐2 수 : "));

				for (int i = 0; i != dig_num; i++) {
					images.dig.transparent_blt(*temp_back_buffer, coord_list[i * 2], RGB(195, 195, 195));
				}

				temp_back_buffer->print_text(coord_list[stage_num], to_tstring(to_wstring(to_wstring(stage_now))));
				temp_back_buffer->print_text(coord_list[time_limit_num], to_tstring(to_wstring(time_limit)));
				temp_back_buffer->print_text(coord_list[kill_remain_num], to_tstring(to_wstring(kill_remain_now)));
				temp_back_buffer->print_text(coord_list[kill_remain_clear_num], to_tstring(to_wstring(kill_remain_clear_now)));
				temp_back_buffer->print_text(coord_list[time_num], to_tstring(to_wstring(timer)));
				temp_back_buffer->print_text(coord_list[score_num], to_tstring(to_wstring(score)));

				//초당 두더쥐 1,2 수 정보를 다른 위치에 표시하기 위해 함수나 반복문으로 안 뺌. 1000 / 주기
				temp_back_buffer->print_text(coord_list[spawn_1_speed_num], to_tstring(to_wstring(1000 / spawn_cycle[0])));
				temp_back_buffer->print_text(coord_list[spawn_2_speed_num], to_tstring(to_wstring(1000 / spawn_cycle[1])));

				//생성 및 히트 애니
				for (monster& mon : monsters) {
					if (mon.get_state() == alive) {
						mon.spawn_ani(hWnd, temp_dc, temp_back_buffer, images.monster_spawn);
					}
					if (mon.get_state() == die) {
						mon.attacked_ani(hWnd, temp_dc, temp_back_buffer, images.monster_die);
					}
				}
			}
			//fps
			tstring fps_str = to_tstring(std::to_wstring(g_fps));
			temp_back_buffer->print_text({}, fps_str);

			//그리기
			temp_back_buffer->bit_blt(temp_dc, {});
		}

		//사용법
		/*
		if (key_state::is_pressed(keys::A)) {
			app_inst->change_window_size({ 110, 110 });
		}
		else if (key_state::is_pressed(keys::S)) {
			app_inst->change_window_size({ 220, 220 });
		}

		if (temp_back_buffer) {
			static simple_stat<> ssss;
			if (ssss.sec() > 0.5f) {
				//ZEE_LOG(normal, TEXT("test key"), TEXT("%s"), to_tstring(key_state::is_toggle_on(keys::A)).c_str());
				ssss.reset();
			}

			if (temp_back_buffer->is_valid()) {
				shape::recti rc;
				rc.data[1] = { 100,200 };

				math::vec2i d = { 100,100 };

				win32gdi::device_context_dynamic temp_image;
				temp_image.load_image(TEXT("./assets/back.bmp"));
				temp_image.bit_blt(*temp_back_buffer, {});
				temp_back_buffer->rectangle(rc);
				temp_back_buffer->move_to(d);
				temp_back_buffer->line_to(d += math::vec2i::constants::unit_x * 10);
				temp_back_buffer->line_to(d += math::vec2i::constants::unit_y * 10);
				temp_back_buffer->line_to(d += math::vec2i::constants::unit_x * 10 + math::vec2i::constants::unit_y * 10);
				temp_back_buffer->line_to(d -= math::vec2i::constants::unit_y * 10);
				tstring fps_str = to_tstring(std::to_wstring(g_fps));
				temp_back_buffer->print_text({}, fps_str);
				temp_back_buffer->bit_blt(temp_dc, {});
			}
		}*/

		return 0;
	}//WM_PAINT
	}//switch(iMessage)

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void zee::application::change_window_size(const math::vec2i& new_window_size) noexcept {
	if (is_started()) {
		MoveWindow(window_handle<HWND>(), config().window_position.x, config().window_position.y, new_window_size.x, new_window_size.y, FALSE);
	}
}

void zee::application::change_window_position(const math::vec2i& new_window_position) noexcept {
	if (is_started()) {
		MoveWindow(window_handle<HWND>(), new_window_position.x, new_window_position.y, config().window_size.x, config().window_size.y, FALSE);
	}
}