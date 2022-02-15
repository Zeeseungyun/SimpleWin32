#include "application.h"
#include "application_delegates.h"
#include <zlog/zlog.h>
#include "../win32helper/windows_with_macro.h"
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"
#include "../stat/simple_stat.h"
#include "../../ZeeCore/public/shape/intersect.h"
#include "../stage/stage.h"

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
static math::vec2i mouse{ 2000, 2000 };

std::shared_ptr<stage> stage_;

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

	app_inst->is_started_ = true;
	application_delegates::on_started().broadcast(application::get());
	stage_ = std::make_shared<stage>();// std::shared_ptr<yjj_stage>(new yjj_stage);
	stage_->on_app_started();

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
	stage_.reset();
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

		return 0;
	}//WM_CREATE

	case WM_DESTROY:
	{
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

	case WM_LBUTTONDOWN:
	{
		mouse.x = (int32)(short)LOWORD(lParam);
		mouse.y = (int32)(short)HIWORD(lParam);

		//InvalidateRect(hWnd, nullptr, false);
		return 0;
	}

	case WM_KEYDOWN:
	{
		return 0;
	}
	case WM_PAINT:
	{
		win32gdi::device_context_auto temp_dc(hWnd, win32gdi::device_context_auto_type::paint);
		int a = 5;
		/*
		win32gdi::device_context_dynamic temp_back_dc;
		temp_back_dc.clear();
		temp_back_dc.create_empty_image({ 1200, 1200 });
		shape::rectf rect_empty_white(-10,-10,1200,1200);
		temp_back_dc.rectangle(rect_empty_white);


		math::vec2f line_begin(100, 200);
		math::vec2f line_end(150, 200);
		temp_back_dc.move_to(line_begin);
		temp_back_dc.line_to(line_end);
		shape::rectf rect_small(200, 200, 240, 240);
		shape::rectf rect_big(300, 200, 450, 350);
		math::vec2f cir_origin_small(600, 250);
		shape::circlef circle_small(cir_origin_small, 40);
		math::vec2f cir_origin_big(800, 250);
		shape::circlef circle_big(cir_origin_big, 100);
		shape::circlef mouse_cir(mouse, 50);
		temp_back_dc.rectangle(rect_small);
		temp_back_dc.rectangle(rect_big);
		temp_back_dc.circle(circle_small);
		temp_back_dc.circle(circle_big);
		temp_back_dc.circle(mouse_cir);


		math::vec2f text_pos(400, 100);
		if (shape::intersect(rect_big, mouse_cir) == shape::collide_type::intersect) {
			temp_back_dc.print_text(text_pos, TEXT("사각형과 원 충돌! "));
		}
		else if (shape::intersect(rect_big, mouse_cir) == shape::collide_type::contain) {
			temp_back_dc.print_text(text_pos, TEXT("사각형이 원을 포함!  "));
		}
		else if (shape::intersect(mouse_cir, rect_small) == shape::collide_type::intersect) {
			temp_back_dc.print_text(text_pos, TEXT("원과 사각형 충돌! "));
		}
		else if (shape::intersect(mouse_cir, rect_small) == shape::collide_type::contain) {
			temp_back_dc.print_text(text_pos, TEXT("원이 사각형을 포함!  "));
		}
		else if (shape::intersect(circle_big, mouse_cir) == shape::collide_type::intersect) {
			temp_back_dc.print_text(text_pos, TEXT("큰 원과 원 충돌!         "));
		}
		else if (shape::intersect(circle_big, mouse_cir) == shape::collide_type::contain) {
			temp_back_dc.print_text(text_pos, TEXT("큰 원이 원을 포함!         "));
		}
		else if (shape::intersect(mouse_cir, circle_small) == shape::collide_type::intersect) {
			temp_back_dc.print_text(text_pos, TEXT("원과 작은 원 충돌!         "));
		}
		else if (shape::intersect(mouse_cir, circle_small) == shape::collide_type::contain) {
			temp_back_dc.print_text(text_pos, TEXT("원이 작은 원을 포함!         "));
		}
		else if (shape::intersect(mouse_cir, line_begin, line_end) == shape::collide_type::intersect) {
			temp_back_dc.print_text(text_pos, TEXT("선과 원 충돌!         "));
		}
		else if (shape::intersect(mouse_cir, line_begin, line_end) == shape::collide_type::contain) {
			temp_back_dc.print_text(text_pos, TEXT("원이 선을 포함!         "));
		}
		else {
			temp_back_dc.print_text(text_pos, TEXT("                                "));
		}

		temp_back_dc.bit_blt(temp_dc, {});
		*/
		
		if (stage_) {
			stage_->render(temp_dc, g_fps);
		}
		
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