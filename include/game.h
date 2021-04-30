#ifndef GAME_H
#define GAME_H

#include <time.h>

#include "SDL.h"
#include "SDL_gpu.h"

#include "util.h"

typedef enum {state_editor = 0, state_play, state_LENGTH} State;

typedef struct {
    bool running;
    clock_t previousFrameTime;
    float stackedTime;
    int frames;

    bool pause;
    State state;
} Game;

extern Game game;

void Game_Initialize();
void Game_Run();
void Game_Run_Emscripten();
void Game_Quit();

void Game_Loop();
void Game_Update_State();

#endif
