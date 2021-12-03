#include "application.h"
#include "application_delegates.h"
#include <zlog/zlog.h>
#include "../win32helper/windows_with_macro.h"
#include "../win32helper/win32helper.h"
#include "../win32gdi/device_context.h"
#include "../stat/simple_stat.h"
#include "tick_manager.h"
#include "key_state.h"


using namespace zee;

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

	case WM_PAINT:
	{
		if (key_state::is_pressed(keys::A)) {
			app_inst->change_window_size({110, 110});
		} else if(key_state::is_pressed(keys::S)) {
			app_inst->change_window_size({ 220, 220 });
		}

		win32gdi::device_context_auto temp_dc(hWnd, win32gdi::device_context_auto_type::paint);
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
				temp_image.load_image(TEXT("./assets/testyjj.bmp"));
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
