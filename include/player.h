#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "SDL_gpu.h"

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
    double fireTimer;
    double reloadTime;

    Vector model[PLAYER_MODEL_COUNT];
    float render[PLAYER_MODEL_COUNT * 2];
} Player;

void Player_Initialize();
void Player_Update(Player* player, double dt);
void Player_Render(GPU_Target* target, Player* player);

Uint32 Player_Reload(Uint32 interval, void* param);

#endif
