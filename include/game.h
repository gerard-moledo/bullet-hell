#ifndef GAME_H
#define GAME_H

#include <time.h>

#include "SDL.h"

#include "util.h"

typedef enum {state_editor = 0, state_play, state_LENGTH} State;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;
    clock_t previousFrameTime;

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
