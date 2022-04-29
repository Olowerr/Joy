#pragma once
#include "MainMenu.h"
#include "EasyLevel.h"
#include "playground.h"

class Game
{
public:
	Game(HINSTANCE hInstance, int cmdShow);
	void Shutdown();

	void Run();

#ifdef _DEBUG
	const UINT Win_Width = 16 * 100;
	const UINT Win_Height = 9 * 100;
#else
	const UINT Win_Width = 1920;
	const UINT Win_Height = 1080;
#endif


private:

	// Don't change order 
	Backend& system;
	UIRenderer uiRender;
	TempMeshStorage meshStorage;
	ObjectRender objRender;
	DecalShadow decalShadow;
	Window& window;

	MainMenu menu;
	EasyLevel easy;
};