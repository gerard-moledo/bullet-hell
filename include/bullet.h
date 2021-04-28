#ifndef BULLET_H
#define BULLET_H

#include "SDL_gpu.h"

#include "util.h"

typedef enum { team_player, team_enemy } Team;

typedef struct {
    int id;
    Team team;

    Vector position;
    Vector velocity;
    float radius;

    BodyCircle body;

    Vector model[5];
} Bullet;

void Bullet_Create(Vector position, Vector velocity, float radius, float bodyRadius, Team team);
void Bullet_Update(Bullet* bullet, float dt);
void Bullet_Render(GPU_Target* target, Bullet* bullet);

#endif
