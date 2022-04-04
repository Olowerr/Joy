#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:window(Backend::GetWindow())
{
	Backend::Initiate(hInstance, cmdShow, Win_Width, Win_Height);

}

void Game::Shutdown()
{
	mainScene.Shutdown();
	Backend::Shutdown();
}

void Game::Run()
{
	mainScene.Load();

	while (window.IsOpen())
	{
		Backend::Process();

		// temp
		if (Backend::GetKeyboard().KeyDown(DIK_DELETE))
			break;

		mainScene.Update();

		mainScene.Render();

	}

}
