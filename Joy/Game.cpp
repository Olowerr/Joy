#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:window(Backend::GetWindow())
{
	Backend::Initiate(hInstance, cmdShow, Win_Width, Win_Height);

}

void Game::Shutdown()
{
	easy.Shutdown();
	Backend::Shutdown();
}

void Game::Run()
{
	//temp
	easy.Load();

	while (window.IsOpen())
	{
		Backend::Process();

		// temp
		if (Backend::GetKeyboard().KeyDown(DIK_DELETE))
			break;

		easy.Update();

		easy.Render();

	}

}
