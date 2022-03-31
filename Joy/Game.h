#pragma once
#include "Backend.h"
#include "ObjectRender.h"
class Game
{
public:
	Game(HINSTANCE hInstance, int cmdShow);
	void Shutdown();

	void Run();

	const UINT Win_Width = 1024;
	const UINT Win_Height = 512;

private:
	Window& window;
	ObjectRender render;
};