#include "Game.h"

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

		// Game
		
	}
}
