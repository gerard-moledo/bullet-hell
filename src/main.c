#include <stdio.h>

#include "SDL.h"

#include "game.h"

int main(int argc, char* argv[])
{
    Game_Initialize();
    Game_Run();
    Game_Quit();

    return 0;
}
