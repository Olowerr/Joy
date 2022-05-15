#pragma once
#include "Window.h"

class Mouse
{
public:
	Mouse(Window& window);
	bool Initiate(LPDIRECTINPUT8 dInput);
	void Shutdown();

	void ReadEvents();

	void Lock(bool lock);
	bool IsLocked() const;

	bool LeftDown() const;
	bool RightDown() const;

	bool LeftReleased() const;
	bool RightReleased() const;

	UINT GetXPos() const;
	UINT GetYPos() const;

	// prolly not needed
	INT GetDeltaX() const;
	INT GetDeltaY() const;

private:
	Window& window;

	UINT xPos, yPos;
	bool leftReleased, rightReleased;

	BOOL locked;

	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE lastState;
};