#pragma once
#include "Scene.h"

class Game
{
public:
	Game(HINSTANCE hInstance, int cmdShow);
	void Shutdown();

	void Run();

	const UINT Win_Width = 16 * 10;
	const UINT Win_Height = 9 * 10;

private:

	Window& window;
	
	MainScene mainScene;
};