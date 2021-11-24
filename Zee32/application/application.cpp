#include "application.h"
#include "application_delegates.h"
#include "../win32helper/windows_nominmax.h"
using namespace zee;

#ifdef UNICODE
#define ZEE_ENTRY_NAME wWinMain
#else
#define ZEE_ENTRY_NAME WinMain
#endif

int APIENTRY ZEE_ENTRY_NAME(HINSTANCE, HINSTANCE, LPSTR, int);

class application_win32 : public application {
	friend application& application::get() noexcept;
	friend int APIENTRY ZEE_ENTRY_NAME(HINSTANCE, HINSTANCE, LPSTR, int);

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

int APIENTRY ZEE_ENTRY_NAME(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	create_app_inst();
	
	app_inst->instance_handle_ = hInstance;
	
	math::vec2i window_size;
	window_size.x = app_inst->config_["window_size"]["x"].get<int32>();
	window_size.y = app_inst->config_["window_size"]["y"].get<int32>();
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
		0, 0, window_size.x, window_size.y,
		NULL, (HMENU)NULL, app_inst->instance_handle<HINSTANCE>(), NULL
	);
	
	if (!ShowWindow(app_inst->window_handle<HWND>(), nCmdShow)) {

	}

	MSG Message;
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

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
