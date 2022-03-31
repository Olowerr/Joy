#include "Game.h"
#include<iostream>
Game::Game(HINSTANCE hInstance, int cmdShow)
	:window(Backend::Get().GetWindow())
{
	Backend::Get().Initiate(hInstance, cmdShow, Win_Width, Win_Height);


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

#ifdef _DEBUG
		if (Backend::Get().GetKeyboard().KeyDown(DIK_DELETE))
			break;
#endif // _DEBUG

	}
}
