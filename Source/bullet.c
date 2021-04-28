#include "world.h"

#include <math.h>

#include "globals.h"

void Bullet_Create(Vector position, Vector velocity, double radius, double bodyRadius, Team team)
{
    if (team == team_player && world.playerBulletsCount >= WORLD_MAX_PLAYER_BULLETS)
        return;
    if (team == team_enemy && world.enemyBulletsCount >= WORLD_MAX_ENEMY_BULLETS)
        return;

    Bullet bullet;
    bullet.id = 0;
    bullet.team = team;
    bullet.position = position;
    bullet.velocity = velocity;
    bullet.radius = radius;
    bullet.body.position = position;
    bullet.body.radius = bodyRadius;

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

void Bullet_Render(GPU_Target* target, Bullet* bullet)
{
    GPU_Rect rect;
    rect.x = bullet->position.x - bullet->radius / 2;
    rect.y = bullet->position.y - bullet->radius / 2;
    rect.w = bullet->radius;
    rect.h = bullet->radius;
    SDL_Color color = { 0, 255, 255, 255 };
    Renderer_Draw_Rectangle(target, rect, color, true);
}
