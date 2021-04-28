#ifndef RENDERER_H
#define RENDERER_H

#include "SDL.h"
#include "SDL_gpu.h"

#include "util.h"
#include "world.h"

#define RENDER_TEXTURE_WIDTH    1600
#define RENDER_TEXTURE_HEIGHT   1200

typedef struct {
    GPU_Image* renderImage;

    VectorInt renderWindowSize;

    Vector renderCenter;
    Vector cameraPosition;
    double zoom;
} Renderer;

extern Renderer renderer;

void Renderer_Initialize();
void Renderer_Destroy();
void Renderer_Reset_Camera();

void Renderer_Draw_Point(GPU_Target* target, double x, double y, SDL_Color color);
void Renderer_Draw_Points(GPU_Target* target, Vector points[], int pointCount, SDL_Color color);
void Renderer_Draw_Lines(GPU_Target* target, float points[], int pointCount, SDL_Color color);
void Renderer_Draw_Rectangle(GPU_Target* target, GPU_Rect rect, SDL_Color color, bool isFilled);

void Renderer_Render();

Vector Renderer_Screen_To_Game_TransformF(double screenX, double screenY, bool isRender);
Vector Renderer_Game_To_Screen_TransformF(double gameX, double gameY, bool isRender);
Vector Renderer_Screen_To_Game_TransformV(Vector screenPoint, bool isRender);
Vector Renderer_Game_To_Screen_TransformV(Vector gamePosition, bool isRender);

#endif
