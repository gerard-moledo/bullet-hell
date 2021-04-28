#include "world.h"

#include <stdio.h>

#include "globals.h"

World world = {0};

void World_Initialize()
{
    Player_Initialize();
}

void World_Clean()
{
    // ===================================
    // CLEAN PLAYER BULLETS
    // ===================================
    for (int index = 0; index < world.playerBulletsCount; index++)
    {
        if (world.playerBullets[index].id == -1)
        {
            for (int lastEntity = world.playerBulletsCount - 1; lastEntity > index; lastEntity--)
            {
                if (world.playerBullets[lastEntity].id != -1)
                {
                    world.playerBullets[lastEntity].id = index;
                    world.playerBullets[index] = world.playerBullets[lastEntity];
                    break;
                }
            }
            world.playerBulletsCount--;
        }
    }
    // ===================================
    // CLEAN ENEMIES
    // ===================================
    for (int index = 0; index < world.enemyCount; index++)
    {
        if (world.enemies[index].id == -1)
        {
            for (int lastEntity = world.enemyCount - 1; lastEntity > index; lastEntity--)
            {
                if (world.enemies[lastEntity].id != -1)
                {
                    world.enemies[lastEntity].id = index;
                    world.enemies[index] = world.enemies[lastEntity];
                    break;
                }
            }
            world.enemyCount--;
        }
    }
    // ===================================
    // CLEAN ENEMY BULLETS
    // ===================================
    for (int index = 0; index < world.enemyBulletsCount; index++)
    {
        if (world.enemyBullets[index].id == -1)
        {
            for (int lastEntity = world.enemyBulletsCount - 1; lastEntity > index; lastEntity--)
            {
                if (world.enemyBullets[lastEntity].id != -1)
                {
                    world.enemyBullets[lastEntity].id = index;
                    world.enemyBullets[index] = world.enemyBullets[lastEntity];
                    break;
                }
            }
            world.enemyBulletsCount--;
        }
    }
}

void World_Update(double dt)
{
    if (game.state == state_editor)
    {
        Editor_Update(dt);
    }

    if (world.player.exists)
    {
        Player_Update(&world.player, dt);
    }

    for (int enemy = 0; enemy < world.enemyCount; enemy++)
    {
        Enemy_Update(&world.enemies[enemy], dt);
    }

    for (int bullet = 0; bullet < world.playerBulletsCount; bullet++)
    {
        Bullet_Update(&world.playerBullets[bullet], dt);
    }
    for (int bullet = 0; bullet < world.enemyBulletsCount; bullet++)
    {
        Bullet_Update(&world.enemyBullets[bullet], dt);
    }
}

void World_Physics_Update()
{
    for (int bullet = 0; bullet < world.playerBulletsCount; bullet++)
    {
        for (int enemy = 0; enemy < world.enemyCount; enemy++)
        {
            if (Check_Collision(world.playerBullets[bullet].body, world.enemies[enemy].body))
            {
                world.playerBullets[bullet].id = -1;
                world.enemies[enemy].id = -1;
            }
        }
    }
    for (int bullet = 0; bullet < world.enemyBulletsCount; bullet++)
    {
        if (Check_Collision(world.enemyBullets[bullet].body, world.player.body))
        {
            world.enemyBullets[bullet].id = -1;
            world.player.position.x = 0;
            world.player.position.y = 0;
        }
    }
}

void World_Render(GPU_Target* target)
{
    for (int bullet = 0; bullet < world.playerBulletsCount; bullet++)
    {
        Bullet_Render(target, &world.playerBullets[bullet]);
    }

    for (int enemy = 0; enemy < world.enemyCount; enemy++)
    {
        if (world.enemies[enemy].id != -1)
        {
                Enemy_Render(target, &world.enemies[enemy]);
        }
    }

    for (int bullet = 0; bullet < world.enemyBulletsCount; bullet++)
    {
        Bullet_Render(target, &world.enemyBullets[bullet]);
    }

    if (world.player.exists)
    {
        Player_Render(target, &world.player);
    }

    // Render Screen Border
    SDL_Color color = { 255, 255, 255, 255 };
    GPU_Rect border = { -WORLD_PLAY_WIDTH / 2 + 1, -WORLD_PLAY_HEIGHT / 2, WORLD_PLAY_WIDTH - 1, WORLD_PLAY_HEIGHT - 1 };
    Renderer_Draw_Rectangle(target, border, color, false);
}