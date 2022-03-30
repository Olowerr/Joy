#include "Window.h"

Window::Window()
	:hWnd(nullptr), rect(), open(false)
{
}

bool Window::Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height)
{
	const wchar_t win_Class[] = L"WinClass";

	WNDCLASS winClass = {};
	winClass.lpfnWndProc = WindowProc;
	winClass.hInstance = hInst;
	winClass.lpszClassName = win_Class;
	winClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	winClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClass(&winClass);

	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(0, win_Class, L"Joy", WS_OVERLAPPEDWINDOW,
		0, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInst, nullptr);
	
	if (!hWnd)
		return false;

#ifdef _DEBUG
	// Fullscreen
#endif // _DEBUG

	ShowWindow(hWnd, showCmd);
	open = true;

	return true;
}

void Window::ProcessMessages()
{
	MSG msg = {};
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
		open = false;
}

LRESULT Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Window::IsActive() const
{
	return hWnd == GetForegroundWindow();
}

HWND Window::GetHWND()
{
	return hWnd;
}