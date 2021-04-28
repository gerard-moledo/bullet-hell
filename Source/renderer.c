#include "renderer.h"

#include <stdio.h>

#include "globals.h"

Renderer renderer = { NULL };

void Renderer_Initialize()
{
    renderer.renderCenter.x = RENDER_TEXTURE_WIDTH / 2;
    renderer.renderCenter.y = RENDER_TEXTURE_HEIGHT / 2;

    renderer.renderWindowSize.x = WORLD_PLAY_WIDTH;
    renderer.renderWindowSize.y = WORLD_PLAY_HEIGHT;

    renderer.zoom = 1;

    renderer.renderImage = GPU_CreateImage(RENDER_TEXTURE_WIDTH, RENDER_TEXTURE_HEIGHT, GPU_FORMAT_RGBA);
    renderer.renderImage->anchor_x = 0;
    renderer.renderImage->anchor_y = 0;
    GPU_LoadTarget(renderer.renderImage);
}

void Renderer_Reset_Camera()
{
    renderer.cameraPosition.x = 0;
    renderer.cameraPosition.y = 0;
    renderer.zoom = 1;
}

// ====================
// Draw Routines
// ====================

void Renderer_Draw_Point(GPU_Target* target, float x, float y, SDL_Color color)
{
    Vector gamePoint; 
    gamePoint.x = x;
    gamePoint.y = y;
    Vector renderPoint = Renderer_Game_To_Screen_TransformV(gamePoint, true);

    GPU_Pixel(target,  renderPoint.x,  renderPoint.y, color);
}

void Renderer_Draw_Points(GPU_Target* target, Vector points[], int pointCount, SDL_Color color)
{
    for (int point = 0; point < pointCount; point++)
    {
        Vector renderPoint = Renderer_Game_To_Screen_TransformV(points[point], true);

        GPU_Pixel(target,  renderPoint.x,  renderPoint.y, color);
    }
}

void Renderer_Draw_Lines(GPU_Target* target, float points[], int pointCount, SDL_Color color)
{
    for (int point = 0; point < pointCount; point += 2)
    {
        Vector renderPoint = Renderer_Game_To_Screen_TransformF(points[point], points[point + 1], true);
        points[point] = renderPoint.x;
        points[point + 1] = renderPoint.y;
    }

    GPU_Polygon(target, pointCount / 2, points, color);
}

void Renderer_Draw_Rectangle(GPU_Target* target, GPU_Rect rect, SDL_Color color, bool isFilled)
{
    Vector topLeft = Renderer_Game_To_Screen_TransformF(rect.x, rect.y, true);
    Vector bottomRight = Renderer_Game_To_Screen_TransformF(rect.x + rect.w, rect.y + rect.h, true);
    
    if (isFilled)
    {
        GPU_RectangleFilled(target, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, color);
    }
    else
    {
        GPU_Rectangle(target, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, color);
    }
}


// ==========================
// RENDER
// ==========================

void Renderer_Render()
{
    GPU_ClearRGB(renderer.renderImage->target, 0, 0, 0);

    // RENDER
    World_Render(renderer.renderImage->target);
    if (game.state == state_editor)
    {
        Editor_Render(renderer.renderImage->target);
    }

    GPU_ClearRGB(game.gpu_target, 150, 150, 150);

    // Render to screen
    GPU_Rect gameRect;
    gameRect.x =  (float) (renderer.renderCenter.x - renderer.cameraPosition.x);
    gameRect.y =  (float) (renderer.renderCenter.y - renderer.cameraPosition.y);
    gameRect.w =  (float) renderer.renderWindowSize.x;
    gameRect.h =  (float) renderer.renderWindowSize.y;

    Vector screenPosition;
    screenPosition.x =  ((WINDOW_WIDTH - renderer.renderWindowSize.x) / 2.f);
    screenPosition.y =  ((WINDOW_HEIGHT - renderer.renderWindowSize.y) / 2.f);

    GPU_Rect renderRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    GPU_Blit(renderer.renderImage, &gameRect, game.gpu_target, screenPosition.x, screenPosition.y);


    GPU_Flip(game.gpu_target);
}


// =======================
// Conversion Functions
// =======================

Vector Renderer_Screen_To_Game_TransformF(float screenX, float screenY, bool isRender)
{
    Vector origin = isRender ? renderer.renderCenter : renderer.cameraPosition;

    Vector gamePosition;
    gamePosition.x = (screenX - origin.x - renderer.renderWindowSize.x / 2) / renderer.zoom;
    gamePosition.y = (screenY - origin.y - renderer.renderWindowSize.y / 2) / renderer.zoom;
    return gamePosition;
}

Vector Renderer_Game_To_Screen_TransformF(float gameX, float gameY, bool isRender)
{
    Vector origin = isRender ? renderer.renderCenter : renderer.cameraPosition;

    Vector screenPoint;
    screenPoint.x = (gameX * renderer.zoom) + origin.x + renderer.renderWindowSize.x / 2;
    screenPoint.y = (gameY * renderer.zoom) + origin.y + renderer.renderWindowSize.y / 2;

    return screenPoint;
}

Vector Renderer_Screen_To_Game_TransformV(Vector screenPoint, bool isRender)
{
    Vector origin = isRender ? renderer.renderCenter : renderer.cameraPosition;

    Vector gamePosition;
    gamePosition.x =  (screenPoint.x - origin.x - renderer.renderWindowSize.x / 2) / renderer.zoom;
    gamePosition.y =  (screenPoint.y - origin.y - renderer.renderWindowSize.y / 2) / renderer.zoom;
    return gamePosition;
}

Vector Renderer_Game_To_Screen_TransformV(Vector gamePosition, bool isRender)
{
    Vector origin = isRender ? renderer.renderCenter : renderer.cameraPosition;

    Vector screenPoint;
    screenPoint.x = (gamePosition.x * renderer.zoom) + origin.x + renderer.renderWindowSize.x / 2;
    screenPoint.y = (gamePosition.y * renderer.zoom) + origin.y + renderer.renderWindowSize.y / 2;

    return screenPoint;
}

void Renderer_Destroy()
{
    GPU_FreeTarget(renderer.renderImage->target);
    GPU_FreeImage(renderer.renderImage);
}