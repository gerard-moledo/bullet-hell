#ifndef ENEMY_H
#define ENEMY_H

#include <time.h>

#include "SDL.h"

#include "util.h"

#define ROUTE_RENDER_COUNT 200

typedef struct {
    int id;

    Vector position;
    double speed;

    bool fire;
    double fireTimer;

    BodyCircle body;

    Path route;
    double t;
    SDL_Point routeRender[ROUTE_RENDER_COUNT];

    Vector model[5];
    SDL_Point render[5];

    bool edit;
} Enemy;

int Enemy_Initialize(Path route);
void Enemy_Update(Enemy* enemy, double dt);
void Enemy_Render(Enemy* enemy);

void Enemy_Set_Route_Render(Enemy* enemy);

Uint32 Enemy_Reload(Uint32 interval, void* param);

void Enemy_Destroy(Enemy* enemy);

#endif
