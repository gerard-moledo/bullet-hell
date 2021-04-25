#include "util.h"

#include "SDL.h"

#define EDITOR_POINT_COUNT 1000

typedef struct {
    Path tempPath;
    SDL_Point pathPoints[EDITOR_POINT_COUNT];
    int pointToEdit;
    int enemyToEdit;
    int activeEnemy;

    VectorInt lastMousePos;
} Editor;

extern Editor editor;

void Editor_Initialize();
void Editor_Update(double dt);
void Editor_Select_Edit_Point(VectorInt mousePos, bool isEnemyRoute);
void Editor_Select_Enemy(VectorInt mousePos);
void Editor_Place(double x, double y, bool isEnemyRoute);
void Editor_Drag(double x, double y);
void Editor_Drag_Enemy(Vector mouseGamePos, VectorInt mousePos);
void Editor_Delete_Point(bool isEnemyRoute);
void Editor_Delete_Enemy();

void Editor_Render();
void Editor_Render_Path();
void Editor_Render_Edit();