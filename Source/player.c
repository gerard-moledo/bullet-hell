#include "player.h"

#include <math.h>

#include "SDL_timer.h"

#include "globals.h"

void Player_Initialize()
{
    Player player = { 0 };
    player.exists = true;

    player.position.x = 0;
    player.position.y = 0;

    player.speed = 150;
    player.velocity.x = 0;
    player.velocity.y = 0;

    player.reloadTime = 0.1f;

    player.body.position = player.position;
    player.body.radius = 1;

    Vector model[5] = {
        {0, -10},
        {-10, 10},
        {0, 5},
        {10, 10},
        {0, -10}
    };
    for (int point = 0; point < 5; point++)
    {
        player.model[point] = model[point];
    }

    world.player = player;
}

void Player_Update(Player* player, float dt)
{
    player->velocity.x = 0;
    player->velocity.y = 0;

    if (input.up) player->velocity.y = -player->speed;
    if (input.down) player->velocity.y = player->speed;
    if (input.left) player->velocity.x = -player->speed;
    if (input.right) player->velocity.x = player->speed;

    player->speedModifier = input.toggleCreep ? 0.5f : 1;

    player->position.x += player->velocity.x * player->speedModifier * dt;
    player->position.y += player->velocity.y * player->speedModifier * dt;

    if (player->position.x >= WORLD_PLAY_WIDTH / 2) player->position.x = WORLD_PLAY_WIDTH / 2;
    if (player->position.x <= -WORLD_PLAY_WIDTH / 2) player->position.x = -WORLD_PLAY_WIDTH / 2;
    if (player->position.y >= WORLD_PLAY_HEIGHT / 2) player->position.y = WORLD_PLAY_HEIGHT / 2;
    if (player->position.y <= -WORLD_PLAY_HEIGHT / 2) player->position.y = -WORLD_PLAY_HEIGHT / 2;


    if (input.shoot)
    {
        player->fireTimer += dt;
        player->fire = player->fireTimer >= player->reloadTime;

        if (player->fire)
        {
            Vector bulletVelocity = {0, -500};
            Bullet_Create(player->position, bulletVelocity, 4, 6, team_player);

            player->fire = false;
            player->fireTimer = 0;
        }
    }
    else
    {
        player->fire = true;
    }

    for (int point = 0; point < PLAYER_MODEL_COUNT; point++)
    {
        player->render[point*2] =  (player->position.x + player->model[point].x);
        player->render[point*2 + 1] =  (player->position.y + player->model[point].y);
    }

    player->body.position = player->position;
}

void Player_Render(GPU_Target* target, Player* player)
{
    //for (int segment = 0; segment < 80; segment++)
    //{
    //    float angle = segment / 80.f * 2 * F_PI;
    //    float pointX = player->position.x + player->body.radius * cosf(angle);
    //    float pointY = player->position.y + player->body.radius * sinf(angle);

    //    SDL_Color color = { 255, 0, 0, 255 };
    //    Renderer_Draw_Point(target, pointX, pointY, color);
    //}

    SDL_Color color = { 255, 255, 255, 255 };
    Renderer_Draw_Lines(target, player->render, PLAYER_MODEL_COUNT * 2, color);
}

Uint32 Player_Reload(Uint32 interval, void* param)
{
    Player* player = (Player*) param;

    player->fire = true;

    return 0;
}
