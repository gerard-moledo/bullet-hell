#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

#include "util.h"

typedef struct {
    bool updateState;

    bool up, down, left, right;
    bool shoot;
    bool toggleCreep;

    bool leftClick;
    bool leftMouseDown;
    bool rightClick;
    bool rightMouseDown;
    bool mouseMoved;
    bool centerCamera;
    
    bool shiftHeld;
    bool ctrlHeld;

    Sint32 scroll;

    bool dropEnemy;
} Input;

extern Input input;

void Input_Refresh();
void Input_Poll_Events();
void Input_Handle_Key(SDL_KeyboardEvent e);
void Input_Handle_Mouse(SDL_MouseButtonEvent e);
void Input_Handle_MouseMotion(SDL_MouseMotionEvent e);
void Input_Handle_MouseWheel(SDL_MouseWheelEvent e);
#endif
