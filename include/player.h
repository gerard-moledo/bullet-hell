#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"

#include "util.h"

#define PLAYER_MODEL_COUNT 5

typedef struct {
    bool exists;

    Vector position;
    double speed;
    Vector velocity;
    double speedModifier;

    BodyCircle body;

    bool fire;
    SDL_TimerID fireTimer;

    Vector model[PLAYER_MODEL_COUNT];
    SDL_Point render[PLAYER_MODEL_COUNT];
} Player;

void Player_Initialize();
void Player_Update(Player* player, double dt);
void Player_Render(Player* player);

Uint32 Player_Reload(Uint32 interval, void* param);

#endif
