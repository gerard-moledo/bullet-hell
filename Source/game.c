#include "game.h"

#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "globals.h"

Game game = {NULL};

void Game_Initialize()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    game.window = SDL_CreateWindow("Bullet Hell", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

#ifdef __EMSCRIPTEN__
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
#else
    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
#endif

    game.previousFrameTime = clock();
    game.running = true;
    game.pause = false;
    game.state = state_editor;
    Game_Update_State();

    Renderer_Initialize();

    World_Initialize();
    Editor_Initialize();
}

void Game_Run()
{
    while (game.running)
    {
        Game_Loop();
    }
}

void Game_Run_Emscripten()
{
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(Game_Loop, 0, 1);
#endif
}

void Game_Loop()
{
    clock_t currentTime = clock();
    double elapsedTime = ((double) currentTime - (double) game.previousFrameTime) / CLOCKS_PER_SEC;
    game.previousFrameTime = currentTime;

    //printf("Bullets: %d, Elapsed Time: %f\n", world.enemyBulletsCount, elapsedTime);

    Input_Refresh();
    Input_Poll_Events();

    if (input.updateState)
    {
        Game_Update_State();
    }

    if (!game.pause)
    {
        World_Update(elapsedTime);
        World_Physics_Update();
        World_Clean();

        Renderer_Batch();
        Renderer_Render();
    }
}

void Game_Quit()
{
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(game.window);

    SDL_Quit();
}

void Game_Update_State()
{
    game.state = ++game.state % state_LENGTH;

    world.enemies[editor.activeEnemy].edit = false;

    renderer.renderWindowSize.x = game.state == state_play ? WORLD_PLAY_WIDTH : WINDOW_WIDTH;
    Renderer_Reset_Camera();
}
