#ifndef EDITOR_H
#define EDITOR_H

#include "util.h"

#include "SDL.h"
#include "SDL_gpu.h"

#define EDITOR_POINT_COUNT 1000

typedef struct {
    Path tempPath;
    Vector pathPoints[EDITOR_POINT_COUNT];
    int pointToEdit;
    int enemyToEdit;
    int activeEnemy;

    VectorInt lastMousePos;
} Editor;

extern Editor editor;

void Editor_Initialize();
void Editor_Update(float dt);
void Editor_Select_Edit_Point(VectorInt mousePos, bool isEnemyRoute);
void Editor_Select_Enemy(VectorInt mousePos);
void Editor_Place(float x, float y, bool isEnemyRoute);
void Editor_Drag(float x, float y);
void Editor_Drag_Enemy(Vector mouseGamePos, VectorInt mousePos);
void Editor_Delete_Point(bool isEnemyRoute);
void Editor_Delete_Enemy();

void Editor_Render(GPU_Target* target);
void Editor_Render_Path(GPU_Target* target);
void Editor_Render_Edit(GPU_Target* target);

#endif