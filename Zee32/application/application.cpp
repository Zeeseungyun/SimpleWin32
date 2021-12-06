#include "application.h"
#include "application_delegates.h"
#include <zlog/zlog.h>
#include "../win32helper/windows_with_macro.h"
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"
#include "../stat/simple_stat.h"
#include "tick_manager.h"
#include "key_state.h"
#include <../public/shape/intersect.h>
#pragma comment(lib, "winmm.lib")

using namespace zee;


//함수 원형
void init_game();
void set_timer(const HWND& hWnd, const int& num, const float& time_cycle);
void kill_timer(const HWND& hWnd, const int& num);
void change_var_ingame(const HWND& hWnd);
void change_var_ending(const HWND& hWnd);
//상수
enum const_variable {
	TIMER_CYCLE = 1000,	//시간 표시
	TIME_LIMIT = 50,	//제한시간
	KILL_REMAIN = 3,	//단계별 남은 처치 수
	KILL_REMAIN_CLEAR = 36,	//클리어까지 남은 처치 수
	SPAWN_MONSTER_MAX = 2,	//동시 스폰할 몬스터 수
	SPAWN_CYCLE_1 = 1000,	//몬1 최초 스폰 주기
	SPAWN_CYCLE_2 = 1500,	//몬2 최초 스폰 주기
	DIG_NUM = 6		//두더지 구멍 개수
};
//좌표
static bool pos[DIG_NUM];	//몬스터 이미 스폰된 자리에 다른 몬스터 스폰 안 하기 위함
const enum e_coord {
	lefttop_lt, lefttop_rb, middletop_lt, middletop_rb, righttop_lt, righttop_rb,
	leftmiddle_lt, leftmiddle_rb, middlemiddle_lt, middlemiddle_rb, rightmiddle_lt, rightmiddle_rb,
	DigMax,	//랜덤범위에 쓰임
	//타이틀
	title_st, title_st2, title_st3, title_st4, title_st5,
	title_ending_box_lt, title_ending_box_rb,
	//인게임
	stagebox_lt, stagebox_rb,
	time_st, score_st, stage_st, time_limit_st,
	time_num, score_num, stage_num, time_limit_num,
	kill_remain_clear_st, kill_remain_st, spawn_1_speed_st, spawn_2_speed_st,
	kill_remain_clear_num, kill_remain_num, spawn_1_speed_num, spawn_2_speed_num,
	//엔딩
	ending_st, ending_st2, time_taken, time_taken_num, score_final, score_final_num,
	Max
};
static const math::vec2i coord_list[Max] = {
	{100, 100}, {200, 200},	{300, 100}, {400, 200},	{500, 100}, {600, 200},
	{100, 300}, {200, 400},	{300, 300}, {400, 400},	{500, 300}, {600, 400},
	{0, 0},
	//타이틀
	{240, 310}, {234, 290}, {234, 330}, {264, 360}, {294, 380},
	{220, 250}, {480, 450},
	//인게임
	{230, 470}, {460, 650},
	{240, 480}, {240, 500}, {240, 520}, {240, 540},
	{296, 480}, {296, 500}, {326, 520}, {326, 540},
	{240, 560}, {240, 580}, {240, 600}, {240, 620},
	{410, 560}, {432, 580}, {370, 600}, {370, 620},
	//엔딩
	{300, 300}, {262, 400}, {290, 340}, {400, 340}, {290, 360}, {340, 360},
};
//게임 상태
enum e_game_state {
	TITLE,
	INGAME,
	ENDING,
};
//전역 변수(점수, 시간 등)
static int game_state;
static int timer;
static int score;
static int now_stage;
static int time_limit;
static int kill_remain;
static int kill_remain_clear;
static const float spawn_coef = 0.8f;	//단계별 더 빠른 스폰 위함. 현재 스폰 주기 * 계수
//입력
static math::vec2i mouse_position;
//몬스터(뚜떠쥐)
enum e_monster_state {
	NONE,
	SPAWN,
	ALIVE,
	DIE
};
class monster {
public:
	explicit monster() noexcept : state(NONE), rand_pos(0), rand_rect({ 2000,2000,2000,2000 }) {}	//mouse_position과 최초 안 겹치게 하기 위함
	virtual ~monster() noexcept {}
	void spawn();
	void attacked(const HWND& hWnd);
	void render_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
		std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer);
	void spawn_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
		std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const;
	void attacked_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
		std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const;
	void set_state(const int& other_state);
	void set_rand_pos(const int& other_rand_pos);
	void set_rand_rect(const shape::recti& other_rand_rect);
	const int& get_state() const;
	const int& get_rand_pos() const;
	const shape::recti& get_rand_rect() const;
private:
	int state;
	int rand_pos;	//랜덤 번째
	shape::recti rand_rect;		//랜덤 번째로 생성한 클릭 범위 (히트박스)
};
//몬스터 전역 객체와 변수
static std::vector<monster> monsters(SPAWN_MONSTER_MAX);
static float spawn_cycle[SPAWN_MONSTER_MAX] = { SPAWN_CYCLE_1, SPAWN_CYCLE_2 };

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

			}
			else {
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
			if (timer >= TIME_LIMIT) {
				change_var_ending(hWnd);
			}
			else {
				timer++;
			}
			break;
		case 2:
			monsters[0].spawn();
			break;
		case 3:
			monsters[1].spawn();
			break;
		}
	}

	case WM_LBUTTONDOWN:
	{
		mouse_position.x = (int32)(short)LOWORD(lParam);
		mouse_position.y = (int32)(short)HIWORD(lParam);

		for (monster& mon : monsters) {
			if (intersect(mon.get_rand_rect(), mouse_position) != shape::collide_type::none) {
				mon.attacked(hWnd);
			}
		}
		return 0;
	}

	case WM_KEYDOWN:
	{
		if (game_state == TITLE) {
			init_game();
			set_timer(hWnd, 1, TIMER_CYCLE);	//게임 시간 표시
			for (int i = 0; i != SPAWN_MONSTER_MAX; i++) {	//몬스터 스폰 타이머
				set_timer(hWnd, i + 2, spawn_cycle[i]);
			}
			game_state = INGAME;
		}
		else if (game_state == ENDING) {
			game_state = TITLE;
		}
		return 0;
	}
	case WM_PAINT:
	{
		win32gdi::device_context_auto temp_dc(hWnd, win32gdi::device_context_auto_type::paint);

		if (temp_back_buffer && temp_back_buffer->is_valid()) {
			//공통배경
			win32gdi::device_context_dynamic temp_image;
			temp_image.load_image(TEXT("./assets/back.bmp"));
			temp_image.bit_blt(*temp_back_buffer, {});
			temp_image.clear();

			//타이틀
			if (game_state == TITLE) {
				temp_back_buffer->rectangle({ coord_list[title_ending_box_lt], coord_list[title_ending_box_rb] });
				temp_back_buffer->print_text(coord_list[title_st], TEXT("| 버그 두더쥐 잡기 게이임 >_< |"));
				temp_back_buffer->print_text(coord_list[title_st2], TEXT("┎-------------------------------------------------┒"));
				temp_back_buffer->print_text(coord_list[title_st3], TEXT("┖-------------------------------------------------┚"));
				temp_back_buffer->print_text(coord_list[title_st4], TEXT("---아무 키나 눌러 시작---"));
				temp_back_buffer->print_text(coord_list[title_st5], TEXT("만든 이: 유재준"));
			}
			//엔딩
			else if (game_state == ENDING) {
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
			else if (game_state == INGAME) {
				temp_back_buffer->rectangle({ coord_list[stagebox_lt], coord_list[stagebox_rb] });
				temp_back_buffer->print_text(coord_list[stage_st], TEXT("스테이지 : "));
				temp_back_buffer->print_text(coord_list[time_limit_st], TEXT("제한시간 : "));
				temp_back_buffer->print_text(coord_list[kill_remain_clear_st], TEXT("클리어까지 남은 처치 : "));
				temp_back_buffer->print_text(coord_list[kill_remain_st], TEXT("다음 단계까지 남은 처치 : "));
				temp_back_buffer->print_text(coord_list[time_st], TEXT("시간 : "));
				temp_back_buffer->print_text(coord_list[score_st], TEXT("점수 : "));
				temp_back_buffer->print_text(coord_list[spawn_1_speed_st], TEXT("초당 두더쥐1 수 : "));
				temp_back_buffer->print_text(coord_list[spawn_2_speed_st], TEXT("초당 두더쥐2 수 : "));

				temp_image.load_image(TEXT("./assets/dig.bmp"));
				for (int i = 0; i != DIG_NUM; i++) {
					temp_image.transparent_blt(*temp_back_buffer, coord_list[i * 2], RGB(195, 195, 195));
				}
				temp_image.clear();

				temp_back_buffer->print_text(coord_list[stage_num], to_tstring(to_wstring(to_wstring(now_stage))));
				temp_back_buffer->print_text(coord_list[time_limit_num], to_tstring(to_wstring(time_limit)));
				temp_back_buffer->print_text(coord_list[kill_remain_num], to_tstring(to_wstring(kill_remain)));
				temp_back_buffer->print_text(coord_list[kill_remain_clear_num], to_tstring(to_wstring(kill_remain_clear)));
				temp_back_buffer->print_text(coord_list[time_num], to_tstring(to_wstring(timer)));
				temp_back_buffer->print_text(coord_list[score_num], to_tstring(to_wstring(score)));

				//초당 두더쥐 1,2 수 정보를 "다른 위치"에 표시하기 위해 함수로 안 뺌. 1000 / 주기
				temp_back_buffer->print_text(coord_list[spawn_1_speed_num], to_tstring(to_wstring(1000 / spawn_cycle[0])));
				temp_back_buffer->print_text(coord_list[spawn_2_speed_num], to_tstring(to_wstring(1000 / spawn_cycle[1])));

				//생성 및 히트 애니
				for (monster& mon : monsters) {
					mon.render_ani(hWnd, temp_dc, temp_image, temp_back_buffer);
				}
			}

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


//----------------------------------------------------------------------------------------------
//함수 정의
//----------------------------------------------------------------------------------------------
//타이머
void set_timer(const HWND& hWnd, const int& num, const float& time_cycle) {
	SetTimer(hWnd, num, static_cast<int>(time_cycle), nullptr);
}
void kill_timer(const HWND& hWnd, const int& num) {
	KillTimer(hWnd, num);
}
//타이틀에서 게임 초기화
void init_game() {
	game_state = TITLE;
	now_stage = 1;
	timer = 0;
	score = 0;
	mouse_position = { 0, 0 };
	kill_remain = KILL_REMAIN;
	kill_remain_clear = KILL_REMAIN_CLEAR;
	spawn_cycle[0] = SPAWN_CYCLE_1;
	spawn_cycle[1] = SPAWN_CYCLE_2;
	time_limit = TIME_LIMIT;
	for (monster& mon : monsters) {
		monsters.pop_back();
	}
	monsters.resize(SPAWN_MONSTER_MAX);
}
//몬스터 함수
inline const int& monster::get_state() const {
	return state;
}
inline const int& monster::get_rand_pos() const {
	return rand_pos;
}
inline const shape::recti& monster::get_rand_rect() const {
	return rand_rect;
}
inline void monster::set_state(const int& other_state) {
	state = other_state;
}
inline void monster::set_rand_pos(const int& other_rand_pos) {
	rand_pos = other_rand_pos;
}
inline void monster::set_rand_rect(const shape::recti& other_rand_rect) {
	rand_rect.left = other_rand_rect.left;
	rand_rect.top = other_rand_rect.top;
	rand_rect.right = other_rand_rect.right;
	rand_rect.bottom = other_rand_rect.bottom;
}
inline void monster::spawn() {
	rand_pos = rand(0, DigMax / 2 - 1);
	pos[rand_pos] = true;
	rand_rect = { coord_list[rand_pos * 2], coord_list[rand_pos * 2 + 1] }; //터치 영역 (마우스 포인터 점과 사각형 충돌)
	state = ALIVE;
}
inline void monster::attacked(const HWND& hWnd) {
	if (state == ALIVE) {
		state = DIE;
		pos[rand_pos] = false;
		PlaySound(TEXT("./assets/Hit_Hurt14.wav"), nullptr, SND_ASYNC);
		change_var_ingame(hWnd);
	}
}
//스폰, 어택 애니 호출
inline void monster::render_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
	std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) {
	if (state == ALIVE) {
		spawn_ani(hWnd, temp_dc, temp_image, temp_back_buffer);
	}
	else if (state == DIE) {
		attacked_ani(hWnd, temp_dc, temp_image, temp_back_buffer);
	}
}
inline void monster::spawn_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
	std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const {
	temp_image.load_image(TEXT("./assets/monster_spawn.bmp"));
	temp_image.transparent_blt(*temp_back_buffer, coord_list[rand_pos * 2], RGB(195, 195, 195));
	temp_image.clear();
}
inline void monster::attacked_ani(const HWND& hWnd, win32gdi::device_context_auto& temp_dc, win32gdi::device_context_dynamic& temp_image,
	std::shared_ptr<win32gdi::device_context_dynamic>& temp_back_buffer) const {
	temp_image.load_image(TEXT("./assets/monster_die.bmp"));
	temp_image.transparent_blt(*temp_back_buffer, coord_list[rand_pos * 2], RGB(195, 195, 195));
	temp_image.clear();
}
//두더쥐 사망 시 스테이지 변수 변동
void change_var_ingame(const HWND& hWnd) {
	score++;
	kill_remain--;
	kill_remain_clear--;
	if (kill_remain_clear <= 0) {
		change_var_ending(hWnd);
	}
	else if (kill_remain <= 0) {
		now_stage++;
		kill_remain = KILL_REMAIN * now_stage;
		//스테이지 상승 시 : 현재 사이클 * 계수
		for (int i = 0; i != SPAWN_MONSTER_MAX; i++) {
			if (spawn_cycle[i] * spawn_coef > 0) {
				spawn_cycle[i] *= spawn_coef;
			}
		}

		//몬스터 스폰 시간 가속해서 갱신
		for (int i = 0; i != SPAWN_MONSTER_MAX; i++) {
			kill_timer(hWnd, i + 2);
			set_timer(hWnd, i + 2, spawn_cycle[i]);
		}
	}
}
//게임 종료 시 (제한시간 종료, 두더쥐 처치수 만족)
void change_var_ending(const HWND& hWnd) {

	game_state = ENDING;

	for (monster& mon : monsters) {
		mon.set_rand_rect({ 2000, 2000, 2000, 2000 });
	}

	kill_timer(hWnd, 1);
	for (int i = 0; i != SPAWN_MONSTER_MAX; i++) {
		kill_timer(hWnd, i + 2);
	}
}