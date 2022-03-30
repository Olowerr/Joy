#include "Game.h"

Game::Game(HINSTANCE hInstance, int cmdShow)
	:window(Backend::Get().GetWindow())
{
	bool succeeded = false;
	
	succeeded = Backend::Get().Initiate(hInstance, cmdShow, Win_Width, Win_Height);
	assert(succeeded);

}

void Game::Shutdown()
{
	Backend::Get().Shutdown();
}

void Game::Run()
{
	while (window.IsOpen())
	{
		window.ProcessMessages();

		// Game
		
	}
}
