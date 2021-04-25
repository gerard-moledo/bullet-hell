#include "input.h"

#include "SDL.h"

#include "util.h"
#include "game.h"
#include "renderer.h"

Input input = { 0 };

void Input_Refresh()
{
    input.updateState = false;

    input.leftClick = false;
    input.rightClick = false;
    input.mouseMoved = false;

    input.centerCamera = false;

    input.scroll = 0;
    input.dropEnemy = false;
}

void Input_Poll_Events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                Input_Handle_Key(e.key);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                Input_Handle_Mouse(e.button);
                break;
            case SDL_MOUSEMOTION:
                Input_Handle_MouseMotion(e.motion);
                break;
            case SDL_MOUSEWHEEL:
                Input_Handle_MouseWheel(e.wheel);
                break;
            case SDL_QUIT:
                game.running = false;
                break;
        }
    }
}

void Input_Handle_Key(SDL_KeyboardEvent e)
{
    if (e.repeat) return;

    if (e.type == SDL_KEYDOWN)
    {
        if (game.state == state_play)
        {
            if (e.keysym.sym == SDLK_w) input.up = true;
            if (e.keysym.sym == SDLK_s) input.down = true;
            if (e.keysym.sym == SDLK_a) input.left = true;
            if (e.keysym.sym == SDLK_d) input.right = true;

            if (e.keysym.sym == SDLK_LSHIFT) input.toggleCreep = !input.toggleCreep;
        }
        if (game.state == state_editor)
        {
            if (e.keysym.sym == SDLK_c) input.centerCamera = true;

            if (e.keysym.sym == SDLK_LSHIFT) input.shiftHeld = true;
            if (e.keysym.sym == SDLK_LCTRL) input.ctrlHeld = true;

            if (e.keysym.sym == SDLK_SPACE) input.dropEnemy = true;
        }



        if (e.keysym.sym == SDLK_TAB) input.updateState = true;
        if (e.keysym.sym == SDLK_ESCAPE) game.running = false;
    }
    if (e.type == SDL_KEYUP)
    {
        if (game.state == state_play)
        {
            if (e.keysym.sym == SDLK_w) input.up = false;
            if (e.keysym.sym == SDLK_s) input.down = false;
            if (e.keysym.sym == SDLK_a) input.left = false;
            if (e.keysym.sym == SDLK_d) input.right = false;
        }
        if (game.state == state_editor)
        {
            if (e.keysym.sym == SDLK_LSHIFT) input.shiftHeld = false;
            if (e.keysym.sym == SDLK_LCTRL) input.ctrlHeld = false;
        }
    }
}

void Input_Handle_Mouse(SDL_MouseButtonEvent e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        if (game.state == state_play)
        {
            if (e.button == SDL_BUTTON_LEFT) input.shoot = true;
        }
        if (game.state == state_editor)
        {

            if (e.button == SDL_BUTTON_LEFT)
            {
                input.leftClick = true;
                input.leftMouseDown = true;
            }

            if (e.button == SDL_BUTTON_RIGHT)
            {
                input.rightClick = true;
                input.rightMouseDown = true;
            }
        }

        //if (e.button == SDL_BUTTON_MIDDLE) game.pause = !game.pause;
    }

    if (e.type == SDL_MOUSEBUTTONUP)
    {
        if (game.state == state_play)
        {
            if (e.button == SDL_BUTTON_LEFT) input.shoot = false;
        }
        if (game.state == state_editor)
        {
            if (e.button == SDL_BUTTON_LEFT) input.leftMouseDown = false;
            if (e.button == SDL_BUTTON_RIGHT) input.rightMouseDown = false;
        }
    }
}

void Input_Handle_MouseMotion(SDL_MouseMotionEvent e)
{
    if (game.state == state_editor)
    {
        input.mouseMoved = true;
    }
}

void Input_Handle_MouseWheel(SDL_MouseWheelEvent e)
{
    if (game.state == state_editor)
    {

        input.scroll = e.y;
    }
}