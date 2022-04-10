#pragma once
#include "MainMenu.h"
#include "EasyLevel.h"

class Game
{
public:
	Game(HINSTANCE hInstance, int cmdShow);
	void Shutdown();

	void Run();

	const UINT Win_Width = 16 * 100;
	const UINT Win_Height = 9 * 100;

private:

	// Don't change order 
	Backend& system;
	UIRenderer uiRender;
	MeshStorage meshStorage;
	ObjectRender objRender;
	Window& window;

	MainMenu menu;
	EasyLevel easy;
};