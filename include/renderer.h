#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"

#include "util.h"
#include "world.h"

#define RENDER_TEXTURE_WIDTH    8000
#define RENDER_TEXTURE_HEIGHT   6000

typedef struct {
    SDL_Texture* renderTexture;
    VectorInt renderWindowSize;

    SDL_Rect bulletPlayerBatch[WORLD_MAX_PLAYER_BULLETS];
    SDL_Rect bulletEnemyBatch[WORLD_MAX_ENEMY_BULLETS];

    VectorInt renderCenter;
    VectorInt cameraPosition;
    double zoom;
} Renderer;

extern Renderer renderer;

void Renderer_Initialize();
void Renderer_Destroy();
void Renderer_Reset_Camera();

void Renderer_Draw_Point(int x, int y);
void Renderer_Draw_Points(SDL_Point points[], int pointCount);
void Renderer_Draw_Lines(SDL_Point points[], int pointCount);

void Renderer_Batch();
void Renderer_Render();

Vector Renderer_Screen_To_Game_Transform(VectorInt screenPoint, bool isRender);
SDL_Point Renderer_Screen_To_Game_Transform_SDLPoint(SDL_Point screenPoint, bool isRender);
VectorInt Renderer_Game_To_Screen_Transform(Vector gamePosition, bool isRender);
VectorInt Renderer_Game_To_Screen_Transform_SDLPoint(SDL_Point renderPoint, bool isRender);

#endif
