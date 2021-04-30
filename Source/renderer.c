#include "renderer.h"

#include <stdio.h>

#include "globals.h"

Renderer renderer = { NULL };

void Renderer_Initialize()
{
    renderer.renderCenter.x = WINDOW_WIDTH / 2;
    renderer.renderCenter.y = WINDOW_HEIGHT / 2;

    renderer.renderWindowSize.x = WORLD_PLAY_WIDTH;
    renderer.renderWindowSize.y = WORLD_PLAY_HEIGHT;

    renderer.zoom = 1;

    renderer.window = SDL_CreateWindow("Bullet Hell", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);

    GPU_SetInitWindow(SDL_GetWindowID(renderer.window));
    renderer.renderTarget = GPU_Init(0, 0, 0);

    renderer.renderTarget->camera.x = CAMERA_OFFSET_X;
    renderer.renderTarget->camera.y = CAMERA_OFFSET_Y;
    renderer.renderTarget->camera.use_centered_origin = 0;
}

void Renderer_Reset_Camera()
{
    renderer.renderTarget->camera.x = CAMERA_OFFSET_X;
    renderer.renderTarget->camera.y = CAMERA_OFFSET_Y;
    renderer.renderTarget->camera.zoom_x = 1;
    renderer.renderTarget->camera.zoom_y = 1;
}

void Renderer_Clip()
{
    Vector playTopLeft = Renderer_World_To_Screen_TransformF((WINDOW_WIDTH - renderer.renderWindowSize.x) / 2 + CAMERA_OFFSET_X,
                                                             (WINDOW_HEIGHT - renderer.renderWindowSize.y) / 2 + CAMERA_OFFSET_Y , true);
    GPU_Rect viewport;
    viewport.x = playTopLeft.x;
    viewport.y = playTopLeft.y;
    viewport.w = renderer.renderWindowSize.x;
    viewport.h = renderer.renderWindowSize.y;
    GPU_SetClipRect(renderer.renderTarget, viewport);
}

// ====================
// Draw Routines
// ====================

void Renderer_Draw_Point(GPU_Target* target, float x, float y, SDL_Color color)
{

    GPU_Pixel(target, x, y, color);
}

void Renderer_Draw_Points(GPU_Target* target, Vector points[], int pointCount, SDL_Color color)
{
    for (int point = 0; point < pointCount; point++)
    {
        GPU_Pixel(target, points[point].x, points[point].y, color);
    }
}

void Renderer_Draw_Lines(GPU_Target* target, float points[], int pointCount, SDL_Color color)
{
    GPU_Polygon(target, pointCount / 2, points, color);
}

void Renderer_Draw_Rectangle(GPU_Target* target, GPU_Rect rect, SDL_Color color, bool isFilled)
{   
    if (isFilled)
    {
        GPU_RectangleFilled(target, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
    }
    else
    {
        GPU_Rectangle(target, rect.x, rect.y, rect.x + rect.w, rect.y + rect.h, color);
    }
}


// ==========================
// RENDER
// ==========================

void Renderer_Render()
{
    if (game.state == state_play)
    {
        GPU_UnsetClip(renderer.renderTarget);
        GPU_ClearRGB(renderer.renderTarget, 100, 100, 100);
        Renderer_Clip();
    }
    GPU_ClearRGB(renderer.renderTarget, 0, 0, 0);

    // ====================
    // CALL RENDER ROUTINES
    // ====================
    World_Render(renderer.renderTarget);
    if (game.state == state_editor)
    {
        Editor_Render(renderer.renderTarget);
    }


    // =================================
    // ACTUAL RENDERING OF BATCHED ITEMS
    // =================================
    GPU_PrimitiveBatch(NULL, renderer.renderTarget, GPU_LINE_LOOP,
                       PLAYER_MODEL_COUNT, renderer.playerBatch,
                       0, 0,
                       GPU_BATCH_XY | GPU_BATCH_RGB);
    GPU_PrimitiveBatch(NULL, renderer.renderTarget, GPU_LINES, 
                       world.enemyCount * ENEMY_MODEL_COUNT, renderer.enemiesBatch, 
                       world.enemyCount * 8, renderer.enemiesBatchIndex, 
                       GPU_BATCH_XY | GPU_BATCH_RGB);
    GPU_PrimitiveBatch(NULL, renderer.renderTarget, GPU_TRIANGLES, 
                       world.playerBulletsCount * BULLET_VERTEX_COUNT, renderer.playerBulletsBatch, 
                       world.playerBulletsCount * BULLET_INDEX_SIZE, renderer.playerBulletsBatchIndex,
                       GPU_BATCH_XY | GPU_BATCH_RGB);
    GPU_PrimitiveBatch(NULL, renderer.renderTarget, GPU_TRIANGLES, 
                       world.enemyBulletsCount * BULLET_VERTEX_COUNT, renderer.enemyBulletsBatch, 
                       world.enemyBulletsCount * BULLET_INDEX_SIZE, renderer.enemyBulletsBatchIndex,
                       GPU_BATCH_XY | GPU_BATCH_RGB);

    // Render Screen Border
    SDL_Color color = { 255, 255, 255, 255 };
    float border[8] = { 
        -WORLD_PLAY_WIDTH / 2, -WORLD_PLAY_HEIGHT / 2,
        WORLD_PLAY_WIDTH / 2, -WORLD_PLAY_HEIGHT / 2,
        WORLD_PLAY_WIDTH / 2, WORLD_PLAY_HEIGHT / 2,
        -WORLD_PLAY_WIDTH / 2, WORLD_PLAY_HEIGHT / 2
    };
    unsigned short index[8] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0
    };
    GPU_PrimitiveBatch(NULL, renderer.renderTarget, GPU_LINE_LOOP,
                       4, border, 8, index, GPU_BATCH_XY);

    GPU_Flip(renderer.renderTarget);
}


// =======================
// Conversion Functions
// =======================
Vector Renderer_World_To_Screen_TransformF(float worldX, float worldY, bool isRender)
{
    Vector cameraCenter;
    cameraCenter.x = -renderer.renderTarget->camera.x;
    cameraCenter.y = -renderer.renderTarget->camera.y;

    Vector origin = isRender ? renderer.renderCenter : cameraCenter;

    Vector screenPoint;
    screenPoint.x = (worldX * renderer.renderTarget->camera.zoom_x) + origin.x;
    screenPoint.y = (worldY * renderer.renderTarget->camera.zoom_y) + origin.y;

    return screenPoint;
}

Vector Renderer_World_To_Screen_TransformV(Vector worldPoint, bool isRender)
{
    Vector cameraCenter;
    cameraCenter.x = -renderer.renderTarget->camera.x;
    cameraCenter.y = -renderer.renderTarget->camera.y;

    Vector origin = isRender ? renderer.renderCenter : cameraCenter;

    Vector screenPoint;
    screenPoint.x = (worldPoint.x * renderer.renderTarget->camera.zoom_x) + origin.x;
    screenPoint.y = (worldPoint.y * renderer.renderTarget->camera.zoom_y) + origin.y;

    return screenPoint;
}

Vector Renderer_Screen_To_World_TransformF(float screenX, float screenY, bool isRender)
{
    Vector cameraCenter;
    cameraCenter.x = -renderer.renderTarget->camera.x;
    cameraCenter.y = -renderer.renderTarget->camera.y;

    Vector origin = isRender ? renderer.renderCenter : cameraCenter;

    Vector worldPosition;
    worldPosition.x = (screenX - cameraCenter.x) / renderer.renderTarget->camera.zoom_x;
    worldPosition.y = (screenY - cameraCenter.y) / renderer.renderTarget->camera.zoom_y;
    return worldPosition;
}

Vector Renderer_Screen_To_World_TransformV(Vector screenPoint, bool isRender)
{
    Vector cameraCenter;
    cameraCenter.x = -renderer.renderTarget->camera.x;
    cameraCenter.y = -renderer.renderTarget->camera.y;

    Vector origin = isRender ? renderer.renderCenter : cameraCenter;

    Vector worldPosition;
    worldPosition.x = (screenPoint.x - origin.x) / renderer.renderTarget->camera.zoom_x;
    worldPosition.y = (screenPoint.y - origin.y) / renderer.renderTarget->camera.zoom_y;
    return worldPosition;
}

void Renderer_Destroy()
{
    GPU_FreeTarget(renderer.renderTarget);
}