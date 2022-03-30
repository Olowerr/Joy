#pragma once
#include<Windows.h>
#include <assert.h>
class Window
{
public:
	Window();
	bool Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height);

	void ProcessMessages();
	HWND GetHWND();
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT mesasge, WPARAM wParam, LPARAM lParam);

	bool IsActive() const;

private:
	HWND hWnd;
	RECT rect;

	bool open;
};