#include "Mouse.h"

Mouse::Mouse(Window& window)
	:window(window), xPos(0), yPos(0), leftReleased(false), rightReleased(0), locked(0), DIMouse(nullptr), lastState()
{
}

bool Mouse::Initiate(LPDIRECTINPUT8 dInput)
{
	HRESULT hr;

	hr = dInput->CreateDevice(GUID_SysMouse, &DIMouse, NULL);
	if (FAILED(hr))
		return false;

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return false;

	hr = DIMouse->SetCooperativeLevel(window.GetHWND(), DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	if (FAILED(hr))
		return false;

	return true;
}

void Mouse::Shutdown()
{
	DIMouse->Unacquire();
}

void Mouse::ReadEvents()
{
	if (locked)
	{
		RECT rect;
		GetWindowRect(window.GetHWND(), &rect);
		SetCursorPos(rect.left + (rect.right - rect.left) / 2, 
			rect.top + (rect.bottom - rect.top) / 2);
	}

	LPARAM lp = window.GetlParam();
	xPos = LOWORD(lp);
	yPos = HIWORD(lp);

	DIMOUSESTATE currentState;
	DIMouse->Acquire();
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &currentState);

	leftReleased = !currentState.rgbButtons[0] && lastState.rgbButtons[0];
	rightReleased = !currentState.rgbButtons[1] && lastState.rgbButtons[1];

	lastState = currentState;

}

void Mouse::Lock(bool lock)
{
	locked = lock;
	if (lock)
		while (ShowCursor(FALSE) >= 0);
	else
		while (ShowCursor(TRUE) <= 0);
}

bool Mouse::IsLocked() const
{
	return locked;
}

bool Mouse::LeftDown() const
{
	return lastState.rgbButtons[0];
}

bool Mouse::RightDown() const
{
	return lastState.rgbButtons[1];
}

bool Mouse::LeftReleased() const
{
	return leftReleased;
}

bool Mouse::RightReleased() const
{
	return rightReleased;
}

UINT Mouse::GetXPos() const
{
	return xPos;
}

UINT Mouse::GetYPos() const
{
	return yPos;
}

INT Mouse::GetDeltaX() const
{
	return (INT)lastState.lX;
}

INT Mouse::GetDeltaY() const
{
	return (INT)lastState.lY;
}
