#pragma once
#include "MainMenu.h"
#include "EasyLevel.h"
#include "UIRender.h"

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
	ObjectRender objRender;
	MeshStorage meshStorage;
	Window& window;

	MainMenu menu;
	EasyLevel easy;
};