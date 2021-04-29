#include "game.h"

#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "globals.h"

Game game = { 0 };

void Game_Initialize()
{
    SDL_Init(SDL_INIT_VIDEO);
    
    game.window = SDL_CreateWindow("Bullet Hell", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    
    GPU_SetInitWindow(SDL_GetWindowID(game.window));

#ifdef __EMSCRIPTEN
    GPU_SetPreInitFlags(GPU_INIT_DISABLE_VSYNC);
#endif

    game.gpu_target = GPU_Init(0, 0, 0);

    game.previousFrameTime = clock();
    game.running = true;
    game.pause = false;
    game.state = state_editor;

    Renderer_Initialize();
    World_Initialize();
    Editor_Initialize();

    Game_Update_State();
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
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
#endif

    clock_t currentTime = clock();
    float elapsedTime = (float) (currentTime -  game.previousFrameTime) / CLOCKS_PER_SEC;
    game.previousFrameTime = currentTime;

#ifndef __EMSCRIPTEN__
    printf("Bullets: %d, Elapsed Time: %f\n", world.enemyBulletsCount, elapsedTime);
#endif

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

        Renderer_Render();
    }
}

void Game_Quit()
{
    Renderer_Destroy();

    GPU_Quit();
    SDL_Quit();
}

void Game_Update_State()
{
    game.state = ++game.state % state_LENGTH;

    world.enemies[editor.activeEnemy].edit = false;

    renderer.renderWindowSize.x = game.state == state_play ? WORLD_PLAY_WIDTH : WINDOW_WIDTH;
    renderer.renderWindowSize.y = game.state == state_play ? WORLD_PLAY_HEIGHT: WINDOW_HEIGHT;
    Renderer_Reset_Camera();
}
