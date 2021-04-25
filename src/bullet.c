#include "world.h"

#include <math.h>

#include "SDL.h"

#include "util.h"
#include "game.h"
#include "renderer.h"
#include "world.h"

void Bullet_Create(Vector position, Vector velocity, Team team)
{
    if (team == team_player && world.playerBulletsCount >= WORLD_MAX_PLAYER_BULLETS)
        return;
    if (team == team_enemy && world.enemyBulletsCount >= WORLD_MAX_ENEMY_BULLETS)
        return;

    Bullet bullet = {
        0,
        team,
        position,
        velocity,
        {position, 2}
    };

    Vector model[5] = {
        {0, -2},
        {-2, 0},
        {0, 2},
        {2, 0},
        {0, -2}
    };
    for (int point = 0; point < 5; point++)
    {
        bullet.model[point] = model[point];
    }

    int maxBullets = bullet.team == team_player ? WORLD_MAX_PLAYER_BULLETS : WORLD_MAX_ENEMY_BULLETS;

    if (bullet.team == team_player)
    {
        bullet.id = world.playerBulletsCount++;
        world.playerBullets[bullet.id] = bullet;
    }
    if (bullet.team == team_enemy)
    {
        bullet.id = world.enemyBulletsCount++;
        world.enemyBullets[bullet.id] = bullet;
    }
}

void Bullet_Update(Bullet* bullet, double dt)
{
    bullet->position.x += bullet->velocity.x * dt;
    bullet->position.y += bullet->velocity.y * dt;

    if (bullet->position.y < -300 || bullet->position.y > 300 || bullet->position.x < -200 || bullet->position.x > 200)
    {
        bullet->id = -1;
        return;
    }

    bullet->body.position = bullet->position;
}

void Bullet_Render(Bullet* bullet)
{
//    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
//    for (int segment = 0; segment < 40; segment++)
//    {
//        double angle = segment / 40. * 2 * PI;
//        double pointX = bullet->position.x + bullet->body.radius * cos(angle);
//        double pointY = bullet->position.y + bullet->body.radius * sin(angle);
//
//        Renderer_Draw_Point(Round_To_Int(pointX), Round_To_Int(pointY));
//    }

    SDL_SetRenderDrawColor(game.renderer, 0, 255, 0, 255);
    VectorInt point = Renderer_Game_To_Screen_Transform(bullet->position, true);
    SDL_RenderDrawPoint(game.renderer, point.x, point.y);
    //Renderer_Draw_Lines(bullet->render, 5);
 }
