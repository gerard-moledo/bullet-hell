#include "world.h"

#include <stdio.h>

#include "globals.h"

World world = { 0 };

void World_Initialize()
{
    for (int entity = 0; entity < WORLD_MAX_ENEMY_BULLETS; entity++)
    {
        if (entity < WORLD_MAX_ENEMIES)
        {
            world.enemies[entity].id = -1;
        }
        
        if (entity < WORLD_MAX_PLAYER_BULLETS)
        {
            world.playerBullets[entity].id = -1;
        }

        world.enemyBullets[entity].id = -1;
    }

    Player_Initialize();
}

void World_Clean()
{
    // ===================================
    // CLEAN PLAYER BULLETS
    // ===================================
    int initialPlayerBulletsCount = world.playerBulletsCount;
    world.playerBulletsCount = 0;
    for (int index = 0; index < initialPlayerBulletsCount; index++)
    {
        if (world.playerBullets[index].id == -1)
        {
            for (int lastEntity = initialPlayerBulletsCount - 1; lastEntity > index; lastEntity--)
            {
                if (world.playerBullets[lastEntity].id != -1)
                {
                    world.playerBullets[index] = world.playerBullets[lastEntity];
                    world.playerBullets[index].id = index;
                    world.playerBullets[lastEntity].id = -1;
                    world.playerBulletsCount++;
                    break;
                }
            }
            //world.playerBulletsCount--;
        }
        else
        {
            world.playerBulletsCount++;
        }
    }
    // ===================================
    // CLEAN ENEMIES
    // ===================================
    int initialEnemyCount = world.enemyCount;
    world.enemyCount = 0;
    for (int index = 0; index < initialEnemyCount; index++)
    {
        if (world.enemies[index].id == -1)
        {
            for (int lastEntity = initialEnemyCount - 1; lastEntity > index; lastEntity--)
            {
                if (world.enemies[lastEntity].id != -1)
                {
                    world.enemies[index] = world.enemies[lastEntity];
                    world.enemies[index].id = index;
                    world.enemies[lastEntity].id = -1;
                    world.enemyCount++;
                    break;
                }
            }
        }
        else
        {
            world.enemyCount++;
        }
    }
    // ===================================
    // CLEAN ENEMY BULLETS
    // ===================================
    int initialEnemyBulletsCount = world.enemyBulletsCount;
    world.enemyBulletsCount = 0;
    for (int index = 0; index < initialEnemyBulletsCount; index++)
    {
        if (world.enemyBullets[index].id == -1)
        {
            for (int lastEntity = initialEnemyBulletsCount - 1; lastEntity > index; lastEntity--)
            {
                if (world.enemyBullets[lastEntity].id != -1)
                {
                    world.enemyBullets[index] = world.enemyBullets[lastEntity];
                    world.enemyBullets[index].id = index;
                    world.enemyBullets[lastEntity].id = -1;
                    world.enemyBulletsCount++;
                    break;
                }
            }
        }
        else
        {
            world.enemyBulletsCount++;
        }
    }
}

void World_Update(float dt)
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
    if (world.player.exists)
    {
        Player_Render(target, &world.player);
    }

    for (int bullet = 0; bullet < world.playerBulletsCount; bullet++)
    {
        Bullet_Render(target, &world.playerBullets[bullet]);
    }

    for (int bullet = 0; bullet < world.enemyBulletsCount; bullet++)
    {
        Bullet_Render(target, &world.enemyBullets[bullet]);
    }

    for (int enemy = 0; enemy < world.enemyCount; enemy++)
    {
        Enemy_Render(target, &world.enemies[enemy]);
    }
}