#include <Windows.h>
#include<crtdbg.h>

#include "Game.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevHInstance, _In_ LPWSTR lpwstr, _In_ int showCmd)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game(hInstance, showCmd);
	game.Run();

	game.Shutdown();

	return 0;
}