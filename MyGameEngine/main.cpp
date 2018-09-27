#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "Game.h"

/*#ifdef _DEBUG   
#ifndef DBG_NEW      
#define DBG_NEW myNew ( _NORMAL_BLOCK , __FILE__ , __LINE__ )     
#define myNew DBG_NEW   
#endif
#endif*/

int main(int argc, char *argv[])
{
#if _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
	//_crtBreakAlloc = 48;    // Break at allocation number 48.
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

	Game game;

	game.run();

	_CrtDumpMemoryLeaks();

	return 0;
}