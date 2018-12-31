#include <iostream>
#if _MSC_VER
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include "Game.h"

int main(int argc, char* argv[])
{
#if _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif

	Game game;

	game.run();

	//_CrtDumpMemoryLeaks(); IT WILL BE CALLED AUTOMATICALLY AT THE END OF PROGRAM

	return 0;
}
