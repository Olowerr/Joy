#include <Windows.h>
#include<crtdbg.h>

#define STB_IMAGE_IMPLEMENTATION

#include "Game.h"
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include "StoredData.h"


#define CONSOLE_ENABLE

#ifdef CONSOLE_ENABLE
void RedirectIOToConsole() {

    //Create a console for this application
    AllocConsole();

    // Get STDOUT handle
    HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
    FILE* COutputHandle = _fdopen(SystemOutput, "w");

    // Get STDERR handle
    HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
    int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
    FILE* CErrorHandle = _fdopen(SystemError, "w");

    //make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
    std::ios::sync_with_stdio(true);

    // Redirect the CRT standard input, output, and error handles to the console
    freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
    freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

    //Clear the error state for each of the C++ standard stream objects. We need to do this, as
    //attempts to access the standard streams before they refer to a valid target will cause the
    //iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
    //to always occur during startup regardless of whether anything has been read from or written to
    //the console or not.
    std::wcout.clear();
    std::cout.clear();
    std::wcerr.clear();
    std::cerr.clear();

}
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevHInstance, _In_ LPWSTR lpwstr, _In_ int showCmd)
{
    
#ifdef CONSOLE_ENABLE
    RedirectIOToConsole();
#endif // CONSOLE_ENABLE


    JOY::data.StoreAll("../Resources/JOYFiles/big3new.joy");
    JOY::data.StoreAll("big3new.joy");

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Game game(hInstance, showCmd);
	game.Run();
    

    
    
	game.Shutdown();

	return 0;
}