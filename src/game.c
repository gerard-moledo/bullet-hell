#include "game.h"

#include <stdio.h>
#include <time.h>

#include "globals.h"

Game game = {NULL};

void Game_Initialize()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

    game.window = SDL_CreateWindow("Bullet Hell", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

    Renderer_Initialize();

    World_Initialize();
    Editor_Initialize();
}

void Game_Run()
{
    clock_t previousTime = clock();

    double stackedTime = 0;
    game.running = true;
    game.pause = false;
    game.state = state_editor;

    Game_Update_State();

    while (game.running)
    {
        clock_t currentTime = clock();
        double elapsedTime = ((double) currentTime - (double) previousTime) / CLOCKS_PER_SEC;
        previousTime = currentTime;
        stackedTime += elapsedTime;

        printf("%d, %f\n", world.enemyBulletsCount, elapsedTime);

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

            stackedTime = 0;
        }
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
