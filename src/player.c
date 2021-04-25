#include "player.h"

#include <math.h>

#include "SDL.h"
#include "SDL_timer.h"

#include "globals.h"

void Player_Initialize()
{
    Player player;
    player.exists = true;

    player.position.x = 0;
    player.position.y = 0;

    player.speed = 200;
    player.velocity.x = 0;
    player.velocity.y = 0;

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

void Player_Update(Player* player, double dt)
{
    player->velocity.x = 0;
    player->velocity.y = 0;

    if (input.up) player->velocity.y = -player->speed;
    if (input.down) player->velocity.y = player->speed;
    if (input.left) player->velocity.x = -player->speed;
    if (input.right) player->velocity.x = player->speed;

    player->speedModifier = input.toggleCreep ? 0.25 : 1;

    player->position.x += player->velocity.x * player->speedModifier * dt;
    player->position.y += player->velocity.y * player->speedModifier * dt;

    if (input.shoot)
    {
        if (player->fire)
        {
            Vector bulletVelocity = {0, -500};
            Bullet_Create(player->position, bulletVelocity, team_player);
            player->fire = false;

            player->fireTimer = SDL_AddTimer(100, Player_Reload, player);
        }
    }
    else
    {
        player->fire = true;
        SDL_RemoveTimer(player->fireTimer);
    }

    for (int point = 0; point < 5; point++)
    {
        player->render[point].x = Round_To_Int(player->position.x + player->model[point].x);
        player->render[point].y = Round_To_Int(player->position.y + player->model[point].y);
    }

    player->body.position = player->position;
}

void Player_Render(Player* player)
{
    SDL_SetRenderDrawColor(game.renderer, 255, 0, 0, 255);
    for (int segment = 0; segment < 40; segment++)
    {
        double angle = segment / 40. * 2 * PI;
        double pointX = player->position.x + player->body.radius * cos(angle);
        double pointY = player->position.y + player->body.radius * sin(angle);

        Renderer_Draw_Point(Round_To_Int(pointX), Round_To_Int(pointY));
    }

    SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
    Renderer_Draw_Lines(player->render, PLAYER_MODEL_COUNT);
}

Uint32 Player_Reload(Uint32 interval, void* param)
{
    Player* player = (Player*) param;

    player->fire = true;

    return 0;
}
