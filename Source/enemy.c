#include "enemy.h"

#include <stdio.h>
#include <math.h>

#include "globals.h"

int Enemy_Initialize(Path route)
{
    Enemy enemy;

    enemy.position = route.waypoints[0];

    enemy.fire = true;
    enemy.fireTimer = 0;

    enemy.body.position = enemy.position;
    enemy.body.radius = 10;

    enemy.route = route;
    enemy.t = 0;

    Path_Duration(&enemy.route);

    Vector model[5] = {
        {0, 10},
        {-10, -10},
        {0, -5},
        {10, -10},
        {0, 10}
    };
    for (int point = 0; point < 5; point++)
    {
        enemy.model[point] = model[point];
    }

    Enemy_Set_Route_Render(&enemy);

    enemy.edit = true;

    enemy.id = world.enemyCount++;
    world.enemies[enemy.id] = enemy;

    return enemy.id;
}

void Enemy_Update(Enemy* enemy, float dt)
{

    if (enemy->route.waypointCount <= 0)
    {
        Enemy_Destroy(enemy);
        return;
    }

    if (game.state == state_play)
    {
        enemy->t += dt / enemy->route.durations[(int)enemy->t + 1];

        if (enemy->t >= enemy->route.waypointCount - 3)
        {
            //Enemy_Destroy(enemy);
            enemy->t = 0;
            return;
        }
        
        if (!enemy->fire)
        {
            enemy->fireTimer += dt;
            enemy->fire = enemy->fireTimer > 0.2;
        }
        if (enemy->fire)
        {
            Vector velocity;
            for (int shot = 0; shot < 30; shot++)
            {
                float angle = shot / 30.f * 2 * F_PI;
                velocity.x = cosf(angle) * 70;
                velocity.y = sinf(angle) * 70;

                Bullet_Create(enemy->position, velocity, 6, 2, team_enemy);
            }

            enemy->fire = false;
            enemy->fireTimer = 0;
        }
    }

    if (game.state == state_editor)
    {
        if (enemy->edit)
        {
            enemy->t += input.scroll / 100;
        }

        if (enemy->t < 0)
        {
            enemy->t = 0;
        }
        if (enemy->t > enemy->route.waypointCount - 3)
        {
            enemy->t = (float) (enemy->route.waypointCount - 3);
        }
    }

    Vector pointOnCurve = Follow_Curve_Constant(enemy->route, enemy->t, false);
    enemy->position.x = pointOnCurve.x;
    enemy->position.y = pointOnCurve.y;

    for (int point = 0; point < ENEMY_MODEL_COUNT; point++)
    {
        enemy->render[point * 2] =  (enemy->position.x + enemy->model[point].x);
        enemy->render[point * 2 + 1] =  (enemy->position.y + enemy->model[point].y);
    }

    enemy->body.position = enemy->position;
}

void Enemy_Render(GPU_Target* target, Enemy* enemy)
{
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };
    SDL_Color green = { 0, 255, 0, 255 };
    SDL_Color red = { 255, 0, 0, 255 };
    
    // Draws Ship
    SDL_Color color = enemy->edit ? yellow : green;
    Renderer_Draw_Lines(target, enemy->render, ENEMY_MODEL_COUNT * 2, color);
    
    if (game.state == state_editor)
    {
        // Draw First Control Point
        color = enemy->edit ? green : white;
        Renderer_Draw_Point(target, enemy->route.waypoints[0].x, enemy->route.waypoints[0].y, color);

        // Draws Route
        color = white;
        Renderer_Draw_Points(target, enemy->routeRender, ROUTE_RENDER_COUNT, color);

        // Draw Last Control Point
        color = enemy->edit ? green : white;
        Renderer_Draw_Point(target, enemy->route.waypoints[enemy->route.waypointCount - 1].x, enemy->route.waypoints[enemy->route.waypointCount - 1].y, color);
    }

    // Draw Collision Shape
    color = red;
    for (int segment = 0; segment < 40; segment++)
    {
        float angle = segment / 40.f * 2 * F_PI;
        float pointX = enemy->position.x + enemy->body.radius * cosf(angle);
        float pointY = enemy->position.y + enemy->body.radius * sinf(angle);

        Renderer_Draw_Point(target, pointX, pointY, color);
    }
}

void Enemy_Set_Route_Render(Enemy* enemy)
{
    for (int point = 0; point < ROUTE_RENDER_COUNT; point++)
    {
        float t = 1.f * point * (enemy->route.waypointCount - 3) / ROUTE_RENDER_COUNT;
        Vector curvePoint = Follow_Curve_Constant(enemy->route, t, true);
        enemy->routeRender[point].x = curvePoint.x;
        enemy->routeRender[point].y = curvePoint.y;
    }
}

Uint32 Enemy_Reload(Uint32 interval, void* param)
{
    Enemy* enemy = (Enemy*) param;
    enemy->fire = true;

    return 0;
}

void Enemy_Destroy(Enemy* enemy)
{
    enemy->id = -1;
}
