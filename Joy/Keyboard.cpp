#include "Keyboard.h"

Keyboard::Keyboard(Window& window)
	:window(window), keysDown{}, keysReleased{}, DIKeyboard(nullptr)
{
}

bool Keyboard::Initiate(LPDIRECTINPUT8 dInput,HINSTANCE hInst)
{
	HRESULT hr;

	hr = dInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, NULL);
	if (FAILED(hr))
		return false;

	hr = DIKeyboard->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))
		return false;

	hr = DIKeyboard->SetCooperativeLevel(window.GetHWND(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
		return false;

	return true;
}

void Keyboard::Shutdown()
{
	DIKeyboard->Unacquire();
}

void Keyboard::ReadEvents()
{
	DIKeyboard->Acquire();

	static bool currentKeys[256] = {};
	DIKeyboard->GetDeviceState(256, (LPVOID)&currentKeys);

	for (UINT i = 0; i < 256; i++)
		keysReleased[i] = !currentKeys[i] && keysDown[i];

	memcpy_s(keysDown, 256, currentKeys, 256);
}

bool Keyboard::KeyDown(int key) const
{
    return keysDown[key];
}

bool Keyboard::KeyReleased(int key) const
{
    return keysReleased[key];
}
