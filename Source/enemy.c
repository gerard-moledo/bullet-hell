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

void Enemy_Update(Enemy* enemy, double dt)
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
            for (int shot = 0; shot < 50; shot++)
            {
                double angle = shot / 50. * 2 * M_PI;
                velocity.x = cos(angle) * 100;
                velocity.y = sin(angle) * 100;

                Bullet_Create(enemy->position, velocity, team_enemy);
            }

            enemy->fire = false;
            enemy->fireTimer = 0;
        }
    }

    if (game.state == state_editor)
    {
        if (enemy->edit)
        {
            enemy->t += (double)input.scroll / 100;
        }

        if (enemy->t < 0)
        {
            enemy->t = 0;
        }
        if (enemy->t > enemy->route.waypointCount - 3)
        {
            enemy->t = enemy->route.waypointCount - 3;
        }
    }

    Vector pointOnCurve = Follow_Curve_Constant(enemy->route, enemy->t, false);
    enemy->position.x = pointOnCurve.x;
    enemy->position.y = pointOnCurve.y;

    for (int point = 0; point < 5; point++)
    {
        enemy->render[point].x = Round_To_Int(enemy->position.x + enemy->model[point].x);
        enemy->render[point].y = Round_To_Int(enemy->position.y + enemy->model[point].y);
    }

    enemy->body.position = enemy->position;
}

void Enemy_Render(Enemy* enemy)
{
    // Draws Ship
    if (enemy->edit)
    {
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
    }
    Renderer_Draw_Lines(enemy->render, 5);

    if (game.state == state_editor)
    {
        // Draw First Control Point
        if (enemy->edit)
        {
            SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        }
        Renderer_Draw_Point(Round_To_Int(enemy->route.waypoints[0].x), Round_To_Int(enemy->route.waypoints[0].y));

        // Draws Route
        SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        Renderer_Draw_Points(enemy->routeRender, ROUTE_RENDER_COUNT);

        // Draw Last Control Point
        if (enemy->edit)
        {
            SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
        }
        Renderer_Draw_Point(Round_To_Int(enemy->route.waypoints[enemy->route.waypointCount - 1].x), Round_To_Int(enemy->route.waypoints[enemy->route.waypointCount - 1].y));
    }

    // Draw Collision Shape
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    for (int segment = 0; segment < 40; segment++)
    {
        double angle = segment / 40. * 2 * M_PI;
        double pointX = enemy->position.x + enemy->body.radius * cos(angle);
        double pointY = enemy->position.y + enemy->body.radius * sin(angle);

        Renderer_Draw_Point(Round_To_Int(pointX), Round_To_Int(pointY));
    }
}

void Enemy_Set_Route_Render(Enemy* enemy)
{
    for (int point = 0; point < ROUTE_RENDER_COUNT; point++)
    {
        double t = 1. * point * (enemy->route.waypointCount - 3) / ROUTE_RENDER_COUNT;
        Vector curvePoint = Follow_Curve_Constant(enemy->route, t, true);
        enemy->routeRender[point].x = Round_To_Int(curvePoint.x);
        enemy->routeRender[point].y = Round_To_Int(curvePoint.y);
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
