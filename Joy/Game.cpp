#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:window(Backend::Get().GetWindow())
{
	Backend::Get().Initiate(hInstance, cmdShow, Win_Width, Win_Height);
	render.initiate();

}

void Game::Shutdown()
{
	Backend::Get().Shutdown();
}

void Game::Run()
{
	while (window.IsOpen())
	{
		Backend::Get().Process();

		// temp
		if (Backend::Get().GetKeyboard().KeyDown(DIK_DELETE))
			break;

		

	}
}
