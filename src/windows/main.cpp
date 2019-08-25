#include <iostream>
#if _MSC_VER
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <cstdlib>
#include <crtdbg.h>
#endif
#include "../Game.h"

int main(int argc, char* argv[])
{
#if _MSC_VER
    //_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    //_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
    //No arguments - Exit
    if (argc != 2)
    {
        printf("NOT ENOUGH ARGUMENTS");
        return 1;
    }
    //printf("GAME DIRECTORY %s", argv[1]);

    Game game(argv[1]);

    game.run();

    //_CrtDumpMemoryLeaks(); IT WILL BE CALLED AUTOMATICALLY AT THE END OF PROGRAM

    return 0;
}
