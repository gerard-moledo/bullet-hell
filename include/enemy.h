#ifndef ENEMY_H
#define ENEMY_H

#include <time.h>

#include "SDL.h"
#include "SDL_gpu.h"

#include "util.h"

#define ROUTE_RENDER_COUNT 200
#define ENEMY_MODEL_COUNT 5

typedef struct {
    int id;

    Vector position;
    double speed;

    bool fire;
    double fireTimer;

    BodyCircle body;

    Path route;
    double t;
    Vector routeRender[ROUTE_RENDER_COUNT];

    Vector model[ENEMY_MODEL_COUNT];
    float render[ENEMY_MODEL_COUNT * 2];

    bool edit;
} Enemy;

int Enemy_Initialize(Path route);
void Enemy_Update(Enemy* enemy, double dt);
void Enemy_Render(GPU_Target* target, Enemy* enemy);

void Enemy_Set_Route_Render(Enemy* enemy);

Uint32 Enemy_Reload(Uint32 interval, void* param);

void Enemy_Destroy(Enemy* enemy);

#endif
