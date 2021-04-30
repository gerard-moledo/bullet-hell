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

    player.reloadTime = 0.05f;

    player.body.position = player.position;
    player.body.radius = 1;

    Vector model[PLAYER_MODEL_COUNT] = {
        {0, -10},
        {-10, 10},
        {0, 5},
        {10, 10}
    };
    for (int point = 0; point < PLAYER_MODEL_COUNT; point++)
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
            Vector bulletVelocity0 = { 0, -300 };
            Vector bulletVelocity1 = { 50, -300 };
            Vector bulletVelocity2 = { -50, -300 };
            Bullet_Create(player->position, bulletVelocity0, 2, 4, team_player);
            Bullet_Create(player->position, bulletVelocity1, 2, 4, team_player);
            Bullet_Create(player->position, bulletVelocity2, 2, 4, team_player);

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
        player->render[point].x =  (player->position.x + player->model[point].x);
        player->render[point].y =  (player->position.y + player->model[point].y);
    }

    player->body.position = player->position;
}

void Player_Render(GPU_Target* target, Player* player)
{
    SDL_Color color = { 255, 255, 0, 255 };
    for (int vertex = 0; vertex < PLAYER_MODEL_COUNT; vertex++)
    {
        Vector renderPoint = Renderer_Game_To_Screen_TransformV(player->render[vertex], true);
        renderer.playerBatch[vertex * PLAYER_VERTEX_SIZE + 0] = renderPoint.x;
        renderer.playerBatch[vertex * PLAYER_VERTEX_SIZE + 1] = renderPoint.y;
        renderer.playerBatch[vertex * PLAYER_VERTEX_SIZE + 2] = color.r / 255.f;
        renderer.playerBatch[vertex * PLAYER_VERTEX_SIZE + 3] = color.g / 255.f;
        renderer.playerBatch[vertex * PLAYER_VERTEX_SIZE + 4] = color.b / 255.f;
    }
}

Uint32 Player_Reload(Uint32 interval, void* param)
{
    Player* player = (Player*) param;

    player->fire = true;

    return 0;
}
