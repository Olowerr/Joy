#pragma once
#include<Windows.h>
#include<assert.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

class Mouse
{
public:
	Mouse() = default;
	void Initiate(LPDIRECTINPUT8 dInput, HWND hWnd);
	void Shutdown();

	void ReadEvents(HWND hWnd /*, WPARAM wParam, LPARAM lParam */ );

	void Lock(bool lock);

	bool LeftDown() const;
	bool RightDown() const;

	bool LeftReleased() const;
	bool RightReleased() const;

	UINT GetXPos() const;
	UINT GetYPos() const;

	INT GetDeltaX() const;
	INT GetDeltaY() const;

private:
	UINT xPos, yPos;
	bool leftReleased, rightReleased;

	BOOL locked;

	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE lastState;
};