#pragma once
#include<Windows.h>

class Window
{
public:
	Window();
	bool Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height);

	void ProcessMessages();
	HWND GetHWND();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT mesasge, WPARAM wParam, LPARAM lParam);

	bool IsActive() const;
	bool IsOpen() const;

private:
	HWND hWnd;

	bool open;
};