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

    renderer.renderTexture = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, RENDER_TEXTURE_WIDTH, RENDER_TEXTURE_HEIGHT);
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

void Renderer_Draw_Point(int x, int y)
{
    Vector gamePoint; 
    gamePoint.x = x;
    gamePoint.y = y;
    VectorInt renderPoint = Renderer_Game_To_Screen_Transform(gamePoint, true);

    SDL_RenderDrawPoint(game.renderer, renderPoint.x, renderPoint.y);
}

void Renderer_Draw_Points(SDL_Point points[], int pointCount)
{
    for (int point = 0; point < pointCount; point++)
    {
        VectorInt renderPoint = Renderer_Game_To_Screen_Transform_SDLPoint(points[point], true);

        points[point].x = renderPoint.x;
        points[point].y = renderPoint.y;
    }

    SDL_RenderDrawPoints(game.renderer, points, pointCount);

    for (int point = 0; point < pointCount; point++)
    {
        SDL_Point gamePoint = Renderer_Screen_To_Game_Transform_SDLPoint(points[point], true);

        points[point].x = gamePoint.x;
        points[point].y = gamePoint.y;
    }
}

void Renderer_Draw_Lines(SDL_Point points[], int pointCount)
{
    for (int point = 0; point < pointCount; point++)
    {
        VectorInt renderPoint = Renderer_Game_To_Screen_Transform_SDLPoint(points[point], true);

        points[point].x = renderPoint.x;
        points[point].y = renderPoint.y;
    }

    SDL_RenderDrawLines(game.renderer, points, pointCount);

    for (int point = 0; point < pointCount; point++)
    {
        SDL_Point gamePoint = Renderer_Screen_To_Game_Transform_SDLPoint(points[point], true);

        points[point].x = gamePoint.x;
        points[point].y = gamePoint.y;
    }
}


// ==========================
// BATCH & RENDER
// ==========================
void Renderer_Batch()
{
    for (int id = 0; id < world.playerBulletsCount; id++)
    {
        VectorInt renderPoint = Renderer_Game_To_Screen_Transform(world.playerBullets[id].position, true);
        SDL_Rect renderRect;
        renderRect.x = renderPoint.x;
        renderRect.y = renderPoint.y;
        renderRect.w = 2;
        renderRect.h = 2;
        renderer.bulletPlayerBatch[id] = renderRect;
    }
    for (int id = 0; id < world.enemyBulletsCount; id++)
    {
        VectorInt renderPoint = Renderer_Game_To_Screen_Transform(world.enemyBullets[id].position, true);
        SDL_Rect renderRect;
        renderRect.x = renderPoint.x;
        renderRect.y = renderPoint.y;
        renderRect.w = 2;
        renderRect.h = 2;
        renderer.bulletEnemyBatch[id] = renderRect;
    }
}

void Renderer_Render()
{
    // Target Texture Render
    SDL_SetRenderTarget(game.renderer, renderer.renderTexture);

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game.renderer);


    // RENDER
    World_Render();
    if (game.state == state_editor)
    {
        Editor_Render();
    }


    // Target Screen Render
    SDL_SetRenderTarget(game.renderer, NULL);

    SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
    SDL_RenderClear(game.renderer);


    // Render to screen
    SDL_Rect gameRect;
    gameRect.x = renderer.renderCenter.x - renderer.cameraPosition.x;
    gameRect.y = renderer.renderCenter.y - renderer.cameraPosition.y;
    gameRect.w = renderer.renderWindowSize.x;
    gameRect.h = renderer.renderWindowSize.y;

    SDL_Rect screenRect;
    screenRect.x = (WINDOW_WIDTH - renderer.renderWindowSize.x) / 2;
    screenRect.y = (WINDOW_HEIGHT - renderer.renderWindowSize.y) / 2;
    screenRect.w = renderer.renderWindowSize.x;
    screenRect.h = renderer.renderWindowSize.y;
    SDL_RenderCopy(game.renderer, renderer.renderTexture, &gameRect, &screenRect);


    SDL_RenderPresent(game.renderer);
}


// =======================
// Conversion Functions
// =======================
Vector Renderer_Screen_To_Game_Transform(VectorInt screenPoint, bool isRender)
{
    VectorInt origin = isRender ? renderer.renderCenter : renderer.cameraPosition;

    Vector gamePosition;
    gamePosition.x = (double) (screenPoint.x - origin.x - renderer.renderWindowSize.x / 2) / renderer.zoom;
    gamePosition.y = (double) (screenPoint.y - origin.y - renderer.renderWindowSize.y / 2) / renderer.zoom;
    return gamePosition;
}

SDL_Point Renderer_Screen_To_Game_Transform_SDLPoint(SDL_Point renderPoint, bool isRender)
{
    VectorInt screenPoint;
    screenPoint.x = renderPoint.x;
    screenPoint.y = renderPoint.y;

    Vector gamePosition = Renderer_Screen_To_Game_Transform(screenPoint, isRender);
    SDL_Point gamePoint;
    gamePoint.x = Round_To_Int(gamePosition.x);
    gamePoint.y = Round_To_Int(gamePosition.y);

    return gamePoint;
}

VectorInt Renderer_Game_To_Screen_Transform(Vector gamePosition, bool isRender)
{
    VectorInt origin = isRender ? renderer.renderCenter : renderer.cameraPosition;

    VectorInt screenPoint;
    double x = (gamePosition.x * renderer.zoom) + origin.x + renderer.renderWindowSize.x / 2;
    double y = (gamePosition.y * renderer.zoom) + origin.y + renderer.renderWindowSize.y / 2;
    screenPoint.x = Round_To_Int(x);
    screenPoint.y = Round_To_Int(y);

    return screenPoint;
}

VectorInt Renderer_Game_To_Screen_Transform_SDLPoint(SDL_Point renderPoint, bool isRender)
{
    Vector gamePosition;
    gamePosition.x = (double) renderPoint.x;
    gamePosition.y = (double) renderPoint.y;

    return Renderer_Game_To_Screen_Transform(gamePosition, isRender);
}


void Renderer_Destroy()
{
    SDL_DestroyTexture(renderer.renderTexture);
}