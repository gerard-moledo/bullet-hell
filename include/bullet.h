#ifndef BULLET_H
#define BULLET_H

#include "SDL_gpu.h"

#include "util.h"

#define BULLET_VERTEX_COUNT 4
#define BULLET_VERTEX_SIZE 5
#define BULLET_INDEX_SIZE 6
#define BULLET_STRIDE (BULLET_VERTEX_COUNT * BULLET_VERTEX_SIZE)

typedef enum { team_player, team_enemy } Team;


typedef struct {
    int id;
    Team team;

    Vector position;
    Vector velocity;
    float radius;

    BodyCircle body;
} Bullet;

void Bullet_Create(Vector position, Vector velocity, float radius, float bodyRadius, Team team);
void Bullet_Update(Bullet* bullet, float dt);
void Bullet_Render(GPU_Target* target, Bullet* bullet);

#endif
