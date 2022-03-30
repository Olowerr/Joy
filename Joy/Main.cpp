#include <Windows.h>
#include <d3d11.h>
#include "Game.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevHInstance, _In_ LPWSTR lpwstr, _In_ int showCmd)
{
	Game game(hInstance, showCmd);
	game.Run();

	game.Shutdown();

	return 0;
}