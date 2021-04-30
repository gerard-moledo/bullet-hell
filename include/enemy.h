#ifndef ENEMY_H
#define ENEMY_H

#include <time.h>

#include "SDL.h"
#include "SDL_gpu.h"

#include "util.h"

#define ROUTE_RENDER_COUNT 100
#define ENEMY_MODEL_COUNT 4
#define ENEMY_VERTEX_SIZE 5
#define ENEMY_INDEX_SIZE 8
#define ENEMY_STRIDE (ENEMY_MODEL_COUNT * ENEMY_VERTEX_SIZE)

typedef struct {
    int id;

    Vector position;
    float speed;

    bool fire;
    float fireTimer;

    BodyCircle body;

    Path route;
    float t;
    Vector routeRender[ROUTE_RENDER_COUNT];

    Vector model[ENEMY_MODEL_COUNT];
    Vector render[ENEMY_MODEL_COUNT];

    bool edit;
} Enemy;

int Enemy_Initialize(Path route);
void Enemy_Update(Enemy* enemy, float dt);
void Enemy_Render(GPU_Target* target, Enemy* enemy);

void Enemy_Set_Route_Render(Enemy* enemy);

Uint32 Enemy_Reload(Uint32 interval, void* param);

void Enemy_Destroy(Enemy* enemy);

#endif
