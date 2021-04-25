#ifndef BULLET_H
#define BULLET_H

#include "util.h"

typedef enum { team_player, team_enemy } Team;

typedef struct {
    int id;
    Team team;

    Vector position;
    Vector velocity;

    BodyCircle body;

    Vector model[5];
    SDL_Point render[5];
} Bullet;

void Bullet_Create(Vector position, Vector velocity, Team team);
void Bullet_Update(Bullet* bullet, double dt);
void Bullet_Render(Bullet* bullet);

#endif
