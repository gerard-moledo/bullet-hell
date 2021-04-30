#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "SDL_gpu.h"

#include "util.h"
#include "world.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define CAMERA_OFFSET_X -400
#define CAMERA_OFFSET_Y -300

typedef struct {
    SDL_Window* window;
    GPU_Target* renderTarget;

    VectorInt renderWindowSize;

    float playerBatch[PLAYER_STRIDE];

    float enemiesBatch[WORLD_MAX_ENEMIES * ENEMY_STRIDE];
    unsigned short enemiesBatchIndex[WORLD_MAX_ENEMIES * ENEMY_INDEX_SIZE];

    float playerBulletsBatch[WORLD_MAX_PLAYER_BULLETS * BULLET_STRIDE];
    unsigned short playerBulletsBatchIndex[WORLD_MAX_PLAYER_BULLETS * BULLET_INDEX_SIZE];

    float enemyBulletsBatch[WORLD_MAX_ENEMY_BULLETS * BULLET_STRIDE];
    unsigned short enemyBulletsBatchIndex[WORLD_MAX_ENEMY_BULLETS * BULLET_INDEX_SIZE];

    Vector renderCenter;
    Vector cameraPosition;
    float zoom;
} Renderer;

extern Renderer renderer;

void Renderer_Initialize();
void Renderer_Destroy();
void Renderer_Reset_Camera();
void Renderer_Clip();

void Renderer_Draw_Point(GPU_Target* target, float x, float y, SDL_Color color);
void Renderer_Draw_Points(GPU_Target* target, Vector points[], int pointCount, SDL_Color color);
void Renderer_Draw_Lines(GPU_Target* target, float points[], int pointCount, SDL_Color color);
void Renderer_Draw_Rectangle(GPU_Target* target, GPU_Rect rect, SDL_Color color, bool isFilled);

void Renderer_Render();

Vector Renderer_World_To_Screen_TransformF(float worldX, float worldY, bool isRender);
Vector Renderer_World_To_Screen_TransformV(Vector worldPosition, bool isRender);
Vector Renderer_Screen_To_World_TransformF(float screenX, float screenY, bool isRender);
Vector Renderer_Screen_To_World_TransformV(Vector screenPoint, bool isRender);

#endif
