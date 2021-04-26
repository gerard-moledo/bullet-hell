#include <stdio.h>

#include "globals.h"

int main(int argc, char* argv[])
{
    Game_Initialize();
    
#ifdef __EMSCRIPTEN__
    Game_Run_Emscripten();
#endif
#ifndef __EMSCRIPTEN__
    Game_Run();
#endif

    Game_Quit();

    return 0;
}
