#include "application.h"
#include "application_delegates.h"
#include "../win32helper/windows_with_macro.h"
#include "../win32gdi/device_context.h"
#include "../stat/simple_stat.h"
#include "tick_manager.h"
#include <zlog/zlog.h>

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
	pl.showCmd = SW_NORMAL;
	pl.rcNormalPosition.left   = rc.get_left  <LONG>();
	pl.rcNormalPosition.top    = rc.get_top   <LONG>();
	pl.rcNormalPosition.right  = rc.get_right <LONG>();
	pl.rcNormalPosition.bottom = rc.get_bottom<LONG>();

	SetWindowPlacement(app_inst->window_handle<HWND>(), &pl);

	HWND h_wnd = app_inst->window_handle<HWND>();
	
	zee::simple_stat<> stat;
	MSG msg = {};
	{
		float gDeltaTime = 0;
		float gFramerate = 0;
		int gFrameTick = 0;
		float gFrameratePerFrameAccum = 0;
		int gFrameratePerFrameIdx = 0;
		float gFramerateSecPerFrame[60] = {};
		float fixedFrame = 120.0f;
		float perSecAccum = 0.0f;
		float perSecond = 0.0f;
		std::chrono::time_point<std::chrono::steady_clock> gPrevTime;

		gPrevTime = std::chrono::steady_clock::now();

		using namespace std::chrono;
		using namespace std::chrono_literals;
		std::vector<float> averages;
		constexpr int frame_count = 60;
		constexpr std::chrono::duration<float, simple_stat_base::milli_sec_ratio> fps = 1s / (float)frame_count;
		std::chrono::duration<float, simple_stat_base::milli_sec_ratio> elapsed_time = std::chrono::duration<float, simple_stat_base::milli_sec_ratio>::zero();
		std::chrono::duration<float, simple_stat_base::milli_sec_ratio> elapsed_time2 = std::chrono::duration<float, simple_stat_base::milli_sec_ratio>::zero();
		std::chrono::duration<float, simple_stat_base::milli_sec_ratio> elapsed_time3 = std::chrono::duration<float, simple_stat_base::milli_sec_ratio>::zero();
		int count = 0;
		while (true) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				
				if (msg.message == WM_QUIT || msg.message == WM_DESTROY) {
					break;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//auto now = std::chrono::steady_clock::now();
				//std::chrono::duration<float> diff = now - gPrevTime;
				//gPrevTime = now;
				//gDeltaTime = diff.count();
				//
				//gFrameratePerFrameAccum += gDeltaTime - gFramerateSecPerFrame[gFrameratePerFrameIdx];
				//gFramerateSecPerFrame[gFrameratePerFrameIdx] = gDeltaTime;
				//gFrameratePerFrameIdx = (gFrameratePerFrameIdx + 1) % 60;
				//gFramerate = gFrameratePerFrameAccum > 0.0f ? 1.0f / (gFrameratePerFrameAccum / (float)60.0f) : FLT_MAX;
				//perSecond += gDeltaTime;
				//g_fps = gFramerate;
				//
				//if (perSecAccum < perSecond) {
				//	perSecond -= perSecAccum;
				//
				//} else {
				//	Sleep(1);
				//}
				//InvalidateRect(h_wnd, nullptr, FALSE);
				//continue;

				auto cur_dur = stat.duration();
				elapsed_time += cur_dur;
				elapsed_time2 += cur_dur;
				stat.reset();

				if (elapsed_time >= fps) {
					count++;
					if (elapsed_time2 >= 1s) {
						ZEE_LOG(normal, TEXT("Sex"), TEXT("count : %d"), count);
						count = 0;
						elapsed_time2 = elapsed_time2.zero();
					}
					auto temp_elapsed = elapsed_time;
					elapsed_time = elapsed_time * 0.5f + elapsed_time3 * 0.5f;
					elapsed_time3 = elapsed_time;
					g_fps = 1000.0f / elapsed_time.count();
					elapsed_time = elapsed_time.zero();

					zee::simple_stat<> stat2;
					tick_manager::get().pre_tick.broadcast(simple_stat_base::milli_sec_to_sec(temp_elapsed.count()));
					temp_elapsed += stat2.duration();
					stat2.reset();
					tick_manager::get().tick.broadcast(simple_stat_base::milli_sec_to_sec(temp_elapsed.count()));
					temp_elapsed += stat2.duration();
					tick_manager::get().post_tick.broadcast(simple_stat_base::milli_sec_to_sec(temp_elapsed.count()));

					//ZEE_LOG(normal, TEXT("tick"), TEXT("average inserted sec : [%f]"), count, stat2.duration().count());
					InvalidateRect(h_wnd, nullptr, FALSE);
				}
			}
		}
	}

	app_inst->app_config_().save();
	app_inst.reset();
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	//https://docs.microsoft.com/en-us/windows/win32/winmsg/window-notifications
	switch (iMessage) {
	//https://docs.microsoft.com/en-us/windows/win32/winmsg/wm-move
	case WM_MOVE:
	{
		auto& position = app_inst->config_().window_position;
		position.x = (int32)(short)LOWORD(lParam);   // horizontal position 
		position.y = (int32)(short)HIWORD(lParam);   // vertical position
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		win32gdi::device_context_auto temp_dc1(hWnd, win32gdi::device_context_auto_type::temp);
		temp_dc1.change_brush_color(win32gdi::colors::Yellow);
		temp_dc1.rectangle({ 100,100,400,400 });

		win32gdi::device_context_auto temp_dc2(hWnd, win32gdi::device_context_auto_type::temp);
		temp_dc2.change_brush_color(win32gdi::colors::Green);
		temp_dc2.rectangle({ 10,10,200,200 });

		temp_dc1.rectangle({ 100,500,400,400 });
		return 0;
	}
	case WM_CREATE:
	{
		app_inst->is_started_ = true;
		application_delegates::on_started().broadcast(application::get());
		win32gdi::device_context_auto temp_dc(hWnd, win32gdi::device_context_auto_type::temp);

		return 0;
	}
	case WM_SIZE:
	{
		auto& app = application::get();
		int d = 0;
		return 0;
	}
	case WM_CHAR:
	{
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}
	case WM_PAINT:
	{
		win32gdi::device_context_auto temp_dc(hWnd, win32gdi::device_context_auto_type::paint);
		static std::shared_ptr<win32gdi::device_context_dynamic> temp_back_buffer;
		if (!temp_back_buffer) {
			temp_back_buffer = std::make_shared< win32gdi::device_context_dynamic>();
		}

		shape::recti rc;
		rc.data[1] = { 100,200 };

		math::vec2i d = { 100,100 };

		if (temp_back_buffer) {
			if (temp_back_buffer->is_valid()) {
				if (temp_back_buffer->get_bitmap_size() != temp_dc.get_bitmap_size()) {
					temp_back_buffer->create_empty_image(temp_dc.get_bitmap_size());
				}
			} else {
				temp_back_buffer->create_empty_image(temp_dc.get_bitmap_size());
			}

			if (temp_back_buffer->is_valid()) {
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
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
