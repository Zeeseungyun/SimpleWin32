#include "application.h"
#include "application_delegates.h"
#include "../win32helper/windows_with_macro.h"
#include "../win32gdi/device_context.h"
#include "../stat/simple_stat.h"

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
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = app_config_data.app_name.c_str();
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WNDCLASSEX dd;
	RegisterClass(&WndClass);

	app_inst->window_handle_ = 
	CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		app_config_data.app_name.c_str(),
		app_config_data.app_name.c_str(),
		WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL, (HMENU)NULL, app_inst->instance_handle<HINSTANCE>(), NULL
	);
	
	WINDOWPLACEMENT pl;
	memset(&pl, 0, sizeof(pl));

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

	int d = 0;
	MSG Message;
	zee::simple_stat stat;
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
		stat.mili_sec();//TODO::TICK ±¸Çö.
	}

	app_inst->app_config_().save();
	app_inst.reset();
	return Message.wParam;
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
		shape::recti rc;
		rc.data[1] = { 100,200 };

		math::vec2i d = { 100,100 };
		temp_dc.rectangle(rc);
		temp_dc.move_to(d);
		temp_dc.line_to(d += math::vec2i::constants::unit_x * 10);
		temp_dc.line_to(d += math::vec2i::constants::unit_y * 10);
		temp_dc.line_to(d += math::vec2i::constants::unit_x * 10 + math::vec2i::constants::unit_y * 10);
		temp_dc.line_to(d -= math::vec2i::constants::unit_y * 10);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
