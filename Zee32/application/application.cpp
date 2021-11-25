#include "application.h"
#include "application_delegates.h"
#include "../win32helper/windows_with_macro.h"
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
public:
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
	
	app_inst->instance_handle_ = hInstance;
	
	math::vec2i window_size;
	window_size.x = app_inst->config_["window_size"]["width"].get<int32>();
	window_size.y = app_inst->config_["window_size"]["height"].get<int32>();
	const tstring app_name = to_tstring((std::string)app_inst->config_["name"]);

	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = app_name.c_str();
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	app_inst->window_handle_ = 
	CreateWindow(
		app_name.c_str(), 
		app_name.c_str(), 
		WS_OVERLAPPEDWINDOW,
		0, 0, 100, 100,
		NULL, (HMENU)NULL, app_inst->instance_handle<HINSTANCE>(), NULL
	);
	
	SetWindowPos(app_inst->window_handle<HWND>(), HWND_TOPMOST, 100, 100, 200, 200, 
		SWP_SHOWWINDOW
	);
	//ShowWindow(app_inst->window_handle<HWND>(), nShowCmd);
	RECT rt;
	WINDOWPLACEMENT pl;
	memset(&pl, 0, sizeof(pl));
	pl.showCmd = nShowCmd;
	pl.rcNormalPosition.left   = 0;
	pl.rcNormalPosition.top    = 0;
	pl.rcNormalPosition.right  = 0;
	pl.rcNormalPosition.bottom = 0;

	GetWindowPlacement(app_inst->window_handle<HWND>(), &pl);
	GetClientRect(app_inst->window_handle<HWND>(), &rt);
	
	int d = 0;
	MSG Message;
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	app_inst.reset();
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	//static TCHAR str[256];
	//int len;
	switch (iMessage) {
	case WM_CHAR:
		//len = strlen(str);
		//str[len] = (TCHAR)wParam;
		//str[len + 1] = 0;
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//TextOut(hdc, 100, 100, str, strlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
namespace zee {
	application::application()
	{
	}
}