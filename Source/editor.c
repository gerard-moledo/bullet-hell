#include "editor.h"

#include <stdio.h>
#include <math.h>

#include "globals.h"

Editor editor = {0};

void Editor_Initialize()
{
    editor.tempPath = Path_Initialize(0, NULL, NULL);
    for (int duration = 0; duration < MAX_PATH_COUNT; duration++)
    {
        editor.tempPath.durations[duration] = 1;
    }

    editor.pointToEdit = -1;
    editor.enemyToEdit = -1;
    editor.activeEnemy = -1;
}

void Editor_Update(float dt)
{
    VectorInt mousePos;
    Uint32 mouseButtonMask = SDL_GetMouseState(&mousePos.x, &mousePos.y);
    Vector mouseGamePos = Renderer_Screen_To_World_TransformF((float) mousePos.x, (float) mousePos.y, false);

    // SELECT POINTS / ENEMIES
    if (!input.shiftHeld)
    {
        Editor_Select_Edit_Point(mousePos, false);
    }
    if (input.shiftHeld)
    {
        Editor_Select_Edit_Point(mousePos, true);
        Editor_Select_Enemy(mousePos);
    }

    // PLACE POINT / EDIT ENEMY
    if (input.leftClick)
    {
        if (!input.shiftHeld)
        {
            if (editor.pointToEdit < 0)
            {
                Editor_Place(mouseGamePos.x, mouseGamePos.y, false);
            }
        }
        if (input.shiftHeld)
        {
            if (editor.enemyToEdit >= 0)
            {
                for (int enemy = 0; enemy < world.enemyCount; enemy++)
                {
                    world.enemies[enemy].edit = false;
                }

                world.enemies[editor.enemyToEdit].edit = true;
                editor.activeEnemy = editor.enemyToEdit;
            }
            if (editor.pointToEdit < 0 && editor.enemyToEdit < 0 && editor.activeEnemy >= 0)
            {
                Editor_Place(mouseGamePos.x, mouseGamePos.y, true);
            }
        }
    }

    // DRAG POINT
    if (input.leftMouseDown)
    {
        if (editor.pointToEdit >= 0 && input.shiftHeld)
        {
            Editor_Drag_Enemy(mouseGamePos, mousePos);
        }
        if (editor.pointToEdit >= 0 && !input.shiftHeld)
        {
            editor.enemyToEdit = -1;

            Editor_Drag(mouseGamePos.x, mouseGamePos.y);
        }
    }

    // DELETE POINT / ENEMY
    if (input.rightClick)
    {
        if (editor.pointToEdit >= 0)
        {
            Editor_Delete_Point(false);
        }
        if (input.shiftHeld)
        {
            bool deletedPoint = false;
            if (editor.pointToEdit >= 0)
            {
                Editor_Delete_Point(true);
                deletedPoint = true;
            }
            if (editor.enemyToEdit >= 0 && !deletedPoint)
            {
                Editor_Delete_Enemy();
            }
        }
    }

    // PAN CAMERA
    if (input.mouseMoved)
    {
        if (mouseButtonMask & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        {
            renderer.renderTarget->camera.x -= mousePos.x - editor.lastMousePos.x;
            renderer.renderTarget->camera.y -= mousePos.y - editor.lastMousePos.y;
        }
    }

    // ZOOM CAMERA
    if (input.ctrlHeld && input.scroll)
    {
        renderer.renderTarget->camera.zoom_x += Sign((float) input.scroll) / 50.f;
        if (renderer.renderTarget->camera.zoom_x <= 1 / 100.) renderer.renderTarget->camera.zoom_x = 1 / 50.f;
        renderer.renderTarget->camera.zoom_y += Sign((float)input.scroll) / 50.f;
        if (renderer.renderTarget->camera.zoom_y <= 1 / 100.) renderer.renderTarget->camera.zoom_y = 1 / 50.f;
    }

    // RESET CAMERA
    if (input.centerCamera)
    {
        Renderer_Reset_Camera();
    }

    // CREATE ENEMY
    if (input.dropEnemy)
    {
        if (editor.tempPath.waypointCount > 0)
        {
            if (editor.enemyToEdit < 0)
            {
                for (int enemy = 0; enemy < world.enemyCount; enemy++)
                {
                    world.enemies[enemy].edit = false;
                }
            }

            editor.activeEnemy = Enemy_Initialize(editor.tempPath);

            editor.tempPath.waypointCount = 0;
        }
    }

    // ADJUST PATH IF ALTERED
    if (input.leftMouseDown || input.rightClick || input.centerCamera || input.ctrlHeld && input.scroll)
    {
        if (editor.activeEnemy >= 0)
        {
            if (world.enemies[editor.activeEnemy].route.waypointCount >= 4)
            {
                Path_Lengths(&world.enemies[editor.activeEnemy].route);

                Enemy_Set_Route_Render(&world.enemies[editor.activeEnemy]);
            }
        }

        if (editor.tempPath.waypointCount >= 4)
        {
            Path_Lengths(&editor.tempPath);

            for (int point = 0; point < EDITOR_POINT_COUNT; point++)
            {
                float t = 1.f * point * (editor.tempPath.waypointCount - 3) / (EDITOR_POINT_COUNT - 1);
                Vector curvePoint = Follow_Curve_Constant(editor.tempPath, t, true);
                editor.pathPoints[point].x = curvePoint.x;
                editor.pathPoints[point].y = curvePoint.y;
            }
        }
    }

    editor.lastMousePos = mousePos;
}

void Editor_Select_Edit_Point(VectorInt mousePos, bool isEnemyRoute)
{
    Path pathToEdit = isEnemyRoute ? world.enemies[editor.activeEnemy].route : editor.tempPath;

    float minDistance = 50;
    for (int point = 0; point < pathToEdit.waypointCount; point++)
    {
        Vector screenPoint = Renderer_World_To_Screen_TransformF( pathToEdit.waypoints[point].x,  pathToEdit.waypoints[point].y, false);
        float distance = Distance((float) mousePos.x, (float) mousePos.y, screenPoint.x, screenPoint.y);
        if (distance < 50)
        {
            if (distance < minDistance)
            {
                if (!input.leftMouseDown || isEnemyRoute)
                {
                    editor.pointToEdit = point;
                }
                minDistance = distance;
            }
        }
    }
    if (minDistance == 50)
    {
        editor.pointToEdit = -1;
    }
}

void Editor_Select_Enemy(VectorInt mousePos)
{
    float minDistance = 50;
    for (int enemy = 0; enemy < world.enemyCount; enemy++)
    {
        Vector screenEnemyPos = Renderer_World_To_Screen_TransformF( world.enemies[enemy].position.x,  world.enemies[enemy].position.y, false);
        float distance = Distance((float) mousePos.x, (float) mousePos.y, screenEnemyPos.x, screenEnemyPos.y);
        if (distance < 50)
        {
            if (distance < minDistance)
            {
                if (!input.leftMouseDown)
                {
                    editor.enemyToEdit = enemy;
                    editor.activeEnemy = enemy;
                }
                minDistance = distance;
            }
        }
    }
    if (minDistance == 50)
    {
        editor.enemyToEdit = -1;
    }
}


//=============================
// EDITOR TOOLS
//=============================

void Editor_Place(float x, float y, bool isEnemyRoute)
{
    Path* pathToEdit = isEnemyRoute ? &world.enemies[editor.activeEnemy].route : &editor.tempPath;

    if (pathToEdit->waypointCount < MAX_PATH_COUNT)
    {

        if (pathToEdit->waypointCount >= 4)
        {
            pathToEdit->waypoints[pathToEdit->waypointCount - 1].x = x;
            pathToEdit->waypoints[pathToEdit->waypointCount - 1].y = y;
            editor.pointToEdit = pathToEdit->waypointCount - 1;
            pathToEdit->waypointCount++;

            pathToEdit->waypoints[pathToEdit->waypointCount - 1].x = x + 50;
            pathToEdit->waypoints[pathToEdit->waypointCount - 1].y = y;
        }

        if (pathToEdit->waypointCount == 2)
        {
            pathToEdit->waypoints[pathToEdit->waypointCount].x = x;
            pathToEdit->waypoints[pathToEdit->waypointCount].y = y;

            pathToEdit->waypoints[0].x = pathToEdit->waypoints[pathToEdit->waypointCount - 1].x - 50;
            pathToEdit->waypoints[0].y = pathToEdit->waypoints[pathToEdit->waypointCount - 1].y;

            pathToEdit->waypoints[pathToEdit->waypointCount + 1].x = x + 50;
            pathToEdit->waypoints[pathToEdit->waypointCount + 1].y = y;

            editor.pointToEdit = 2;
            pathToEdit->waypointCount = 4;
        }

        if (pathToEdit->waypointCount == 0)
        {
            pathToEdit->waypoints[0].x = x;
            pathToEdit->waypoints[0].y = y;
            pathToEdit->waypoints[1].x = x;
            pathToEdit->waypoints[1].y = y;
            pathToEdit->waypointCount = 2;
            editor.pointToEdit = 1;
        }
    }
}

void Editor_Drag(float x, float y)
{
    editor.tempPath.waypoints[editor.pointToEdit].x = x;
    editor.tempPath.waypoints[editor.pointToEdit].y = y;
}

void Editor_Drag_Enemy(Vector mouseGamePos, VectorInt mousePos)
{
    Editor_Select_Edit_Point(mousePos, true);

    world.enemies[editor.activeEnemy].route.waypoints[editor.pointToEdit].x = mouseGamePos.x;
    world.enemies[editor.activeEnemy].route.waypoints[editor.pointToEdit].y = mouseGamePos.y;
    Path_Lengths(&world.enemies[editor.activeEnemy].route);
 }

void Editor_Delete_Point(bool isEnemyRoute)
{
    Path* pathToEdit = isEnemyRoute ? &world.enemies[editor.activeEnemy].route : &editor.tempPath;

    for (int point = editor.pointToEdit; point < pathToEdit->waypointCount - 1; point++)
    {
        pathToEdit->waypoints[point] = pathToEdit->waypoints[point + 1];
    }

    pathToEdit->waypointCount--;
    if (pathToEdit->waypointCount < 4) pathToEdit->waypointCount = 0;
}

void Editor_Delete_Enemy()
{
    Enemy_Destroy(&world.enemies[editor.enemyToEdit]);
    if (editor.activeEnemy == editor.enemyToEdit)
    {
        editor.activeEnemy = -1;
    }
}


// ==========================
// RENDERING
// ==========================
void Editor_Render(GPU_Target* target)
{
    Editor_Render_Path(target);
    Editor_Render_Edit(target);
}

void Editor_Render_Path(GPU_Target* target)
{
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color green = { 0, 255, 0, 255 };

    if (editor.tempPath.waypointCount > 0)
    {
        // Draw First Control Point

        SDL_Color color = editor.pointToEdit == 0 ? green : white;

        Renderer_Draw_Point(target, editor.tempPath.waypoints[0].x, editor.tempPath.waypoints[0].y, color);
    }

    if (editor.tempPath.waypointCount >= 4)
    {   
        // Draw Path
        Renderer_Draw_Points(target, editor.pathPoints, EDITOR_POINT_COUNT, white);


        // Draw Last Control Point
        SDL_Color color = editor.pointToEdit == 0 ? green : white;
        Renderer_Draw_Point(target, editor.tempPath.waypoints[editor.tempPath.waypointCount - 1].x, editor.tempPath.waypoints[editor.tempPath.waypointCount - 1].y, color);
    }
}

void Editor_Render_Edit(GPU_Target* target)
{
    Vector point = { WORLD_WIDTH };


    if (editor.pointToEdit >= 0 && !input.shiftHeld)
    {
        point = editor.tempPath.waypoints[editor.pointToEdit];
    }
    if (editor.enemyToEdit >= 0 && input.shiftHeld)
    {
        point = world.enemies[editor.enemyToEdit].position;
    }
    if (editor.pointToEdit >= 0 && input.shiftHeld)
    {
        point = world.enemies[editor.activeEnemy].route.waypoints[editor.pointToEdit];
    }

    if (point.x != WORLD_WIDTH)
    {
        for (int segment = 0; segment < 40; segment++)
        {
            float angle = segment / 40.f * 2 * F_PI;
            float pointX = point.x + 50 / renderer.zoom * cosf(angle);
            float pointY = point.y + 50 / renderer.zoom * sinf(angle);

            SDL_Color yellow = { 255, 255, 0, 255 };
            //Renderer_Draw_Point(target,  pointX,  pointY, yellow);
            GPU_Pixel(target, pointX, pointY, yellow);
        }
    }

}
