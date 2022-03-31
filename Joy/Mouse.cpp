#include "Mouse.h"

void Mouse::Initiate(LPDIRECTINPUT8 dInput, HWND hWnd)
{
	HRESULT hr;

	hr = dInput->CreateDevice(GUID_SysMouse, &DIMouse, NULL);
	assert(!FAILED(hr));

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	assert(!FAILED(hr));

	hr = DIMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
	assert(!FAILED(hr));
}

void Mouse::Shutdown()
{
	DIMouse->Unacquire();
}

void Mouse::ReadEvents(HWND hWnd /*, WPARAM wParam, LPARAM lParam */ )
{
}

void Mouse::Lock(bool lock)
{
}

bool Mouse::LeftDown() const
{
	return false;
}

bool Mouse::RightDown() const
{
	return false;
}

bool Mouse::LeftReleased() const
{
	return false;
}

bool Mouse::RightReleased() const
{
	return false;
}

UINT Mouse::GetXPos() const
{
	return 0;
}

UINT Mouse::GetYPos() const
{
	return 0;
}

INT Mouse::GetDeltaX() const
{
	return 0;
}

INT Mouse::GetDeltaY() const
{
	return 0;
}
