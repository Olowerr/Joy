#pragma once
#include<Windows.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

class Window
{
public:
	Window();
	bool Initiate(HINSTANCE hInst, int showCmd, UINT width, UINT height);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT mesasge, WPARAM wParam, LPARAM lParam);
	void ProcessMessages();
	HWND GetHWND();
	LPARAM GetlParam();

	bool IsActive() const;
	bool IsOpen() const;

private:
	static LPARAM lParam;
	
	HWND hWnd;

	bool open;
};