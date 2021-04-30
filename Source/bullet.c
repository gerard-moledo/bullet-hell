#include "world.h"

#include <math.h>
#include <stdlib.h>

#include "globals.h"

void Bullet_Create(Vector position, Vector velocity, float radius, float bodyRadius, Team team)
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

    bullet.model[0].x = 0;       bullet.model[0].y = -radius;
    bullet.model[1].x = radius;  bullet.model[1].y = 0;
    bullet.model[2].x = 0;       bullet.model[2].y = radius;
    bullet.model[3].x = -radius; bullet.model[3].y = 0;

    bullet.color.r = rand() % 50 + 155;
    bullet.color.g = rand() % 100 + 85;
    bullet.color.b = rand() % 150 + 75;

    int maxBullets = bullet.team == team_player ? WORLD_MAX_PLAYER_BULLETS : WORLD_MAX_ENEMY_BULLETS;

    unsigned short* teamBatchIndex = renderer.enemyBulletsBatchIndex;
    if (bullet.team == team_player)
    {
        bullet.id = world.playerBulletsCount++;
        world.playerBullets[bullet.id] = bullet;
        teamBatchIndex = renderer.playerBulletsBatchIndex;
    }
    if (bullet.team == team_enemy)
    {
        bullet.id = world.enemyBulletsCount++;
        world.enemyBullets[bullet.id] = bullet;
        teamBatchIndex = renderer.enemyBulletsBatchIndex;
    }

    teamBatchIndex[bullet.id * BULLET_INDEX_SIZE + 0] = bullet.id * BULLET_VERTEX_COUNT + 0;
    teamBatchIndex[bullet.id * BULLET_INDEX_SIZE + 1] = bullet.id * BULLET_VERTEX_COUNT + 1;
    teamBatchIndex[bullet.id * BULLET_INDEX_SIZE + 2] = bullet.id * BULLET_VERTEX_COUNT + 2;
    teamBatchIndex[bullet.id * BULLET_INDEX_SIZE + 3] = bullet.id * BULLET_VERTEX_COUNT + 2;
    teamBatchIndex[bullet.id * BULLET_INDEX_SIZE + 4] = bullet.id * BULLET_VERTEX_COUNT + 3;
    teamBatchIndex[bullet.id * BULLET_INDEX_SIZE + 5] = bullet.id * BULLET_VERTEX_COUNT + 0;
}

void Bullet_Update(Bullet* bullet, float dt)
{
    bullet->position.x += bullet->velocity.x * dt;
    bullet->position.y += bullet->velocity.y * dt;

    if (bullet->position.y < -300 || bullet->position.y > 300 || bullet->position.x < -200 || bullet->position.x > 200)
    {
        bullet->id = -1;
        return;
    }

    bullet->body.position = bullet->position;

    for (int point = 0; point < BULLET_VERTEX_COUNT; point++)
    {
        bullet->render[point].x = (bullet->position.x + bullet->model[point].x);
        bullet->render[point].y = (bullet->position.y + bullet->model[point].y);
    }
}

void Bullet_Render(GPU_Target* target, Bullet* bullet)
{
    float* teamBulletsBatch = bullet->team == team_player ? renderer.playerBulletsBatch : renderer.enemyBulletsBatch;
    for (int point = 0; point < BULLET_VERTEX_COUNT; point++)
    {
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 0] = bullet->render[point].x;
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 1] = bullet->render[point].y;
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 2] = bullet->color.r / 255.f;
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 3] = bullet->color.g / 255.f;
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 4] = bullet->color.b / 255.f;
    }
}
