#ifndef GAME_H
#define GAME_H

#include "SDL.h"

#include "util.h"

typedef enum {state_editor = 0, state_play, state_LENGTH} State;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;

    bool running;
    bool pause;
    State state;
} Game;

extern Game game;

void Game_Initialize();
void Game_Run();
void Game_Quit();

void Game_Update_State();

#endif
