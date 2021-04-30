#include "world.h"

#include <math.h>

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
}

void Bullet_Render(GPU_Target* target, Bullet* bullet)
{
    SDL_Color color;
    color.r = (int) bullet->velocity.x / 3;
    color.g = (int) bullet->velocity.y / 2;
    color.b = (int) bullet->position.x / 4;

    Vector renderPoint = Renderer_Game_To_Screen_TransformV(bullet->position, true);
    Vector renderRect[BULLET_VERTEX_COUNT];
    renderRect[0].x = renderPoint.x - bullet->radius;
    renderRect[0].y = renderPoint.y - bullet->radius;
    renderRect[1].x = renderPoint.x + bullet->radius;
    renderRect[1].y = renderPoint.y - bullet->radius;
    renderRect[2].x = renderPoint.x + bullet->radius;
    renderRect[2].y = renderPoint.y + bullet->radius;
    renderRect[3].x = renderPoint.x - bullet->radius;
    renderRect[3].y = renderPoint.y + bullet->radius;

    float* teamBulletsBatch = bullet->team == team_player ? renderer.playerBulletsBatch : renderer.enemyBulletsBatch;
    for (int point = 0; point < BULLET_VERTEX_COUNT; point++)
    {
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 0] = renderRect[point].x;
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 1] = renderRect[point].y;
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 2] = color.r / 255.f;
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 3] = color.g / 255.f;
        teamBulletsBatch[bullet->id * BULLET_STRIDE + point * BULLET_VERTEX_SIZE + 4] = color.b / 255.f;
    }

}
