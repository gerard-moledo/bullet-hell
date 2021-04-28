#ifndef WORLD_H
#define WORLD_H

#include "util.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"

#define WORLD_MAX_PLAYER_BULLETS 1000
#define WORLD_MAX_ENEMY_BULLETS 10000
#define WORLD_MAX_ENEMIES 1000

#define WORLD_PLAY_WIDTH 400
#define WORLD_PLAY_HEIGHT 590

typedef enum { entities_playerBullets, entities_enemies, entities_enemyBullets } EntityType;

typedef struct {
    Player player;
    Bullet playerBullets[WORLD_MAX_PLAYER_BULLETS];
    int playerBulletsCount;
    Enemy enemies[WORLD_MAX_ENEMIES];
    int enemyCount;
    Bullet enemyBullets[WORLD_MAX_ENEMY_BULLETS];
    int enemyBulletsCount;
} World;

extern World world;

void World_Initialize();
void World_Clean();
void World_Update(float dt);
void World_Physics_Update();
void World_Render(GPU_Target* target);

#endif
