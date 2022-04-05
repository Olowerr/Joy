#pragma once
#include "Window.h"

class Keyboard
{
public:
	Keyboard(Window& window);
	bool Initiate(LPDIRECTINPUT8 dInput, HINSTANCE hInst);
	void Shutdown();

	void ReadEvents();

	// DIK_key
	bool KeyDown(int key) const;

	// DIK_key
	bool KeyReleased(int key) const;

private:
	Window& window;

	bool keysDown[256];
	bool keysReleased[256];

	IDirectInputDevice8* DIKeyboard;
};