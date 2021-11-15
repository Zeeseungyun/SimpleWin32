#include "application.h"
#include "log/log.h"
#include "core/core.h"
#include <tchar.h>
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LPCTSTR lpszClass = _T("KeyDown");

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdParam, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

namespace zee {
	class application_impl {
		friend int WINAPI ::wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdParam, int nCmdShow);
	public:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

		static void on_window_create(handle_t hWnd, handle_t hInst) {
			application::app_ = std::unique_ptr<application>(new application());
			application::get().on_window_create(hWnd, hInst);
		}

		static void on_window_size_changed() {
			math::vec2 size = application::get().get_window_client_size();
			application::get().on_window_size_changed(size);
		}

		static void on_window_destory() {
			application::get().on_window_destory();
			application::app_ = nullptr;
		}
	};

}//namespace zee 

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)zee::application_impl::WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);

	zee::application_impl::on_window_create(hWnd, hInstance);
	ShowWindow(hWnd, nCmdShow);

	//zee::application::get().tick();

	while (true) {
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	zee::application_impl::on_window_destory();

	return (int)Message.wParam;
}

#include "render/win32gdi/device_context.h"
#include "math/shape.h"

LRESULT CALLBACK zee::application_impl::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{

	using namespace zee;
	using namespace zee::win32gdi;
	static std::shared_ptr<device_context_dynamic> backbuffer;
	switch (iMessage) {
	case WM_SIZE:
	{
		application_impl::on_window_size_changed();
		if (!backbuffer) {
			backbuffer = std::make_shared<device_context_dynamic>();
		}

		bool bb = backbuffer->load_image(TEXT("../assets/temp_test.bmp"));
		int d = 0;
	}
	break;
	case WM_PAINT:
	{
		device_context_auto tempdc(hWnd, device_context_auto_type::paint);
		if (backbuffer) {
			//backbuffer->transparent_blt(tempdc, { 0,0 }, colors::Black);
			backbuffer->alphablend(tempdc, { 0,0 }, 0.3f);
		}
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}