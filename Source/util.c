#include "util.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

int Round_To_Int(float num)
{
    return (int) (num < 0 ? num - 0.5 : num + 0.5);
}

int Sign(float num)
{
    return num > 0 ? 1 : -1;
}

float Distance(float x1, float y1, float x2, float y2)
{
    return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

float DistanceI(int x1, int y1, int x2, int y2)
{
    return Distance((float) x1, (float) y1, (float) x2, (float) y2);
}

float Lerp(float start, float finish, float t)
{
    return start + t * (finish - start);
}

float Map(float s0, float s1, float f0, float f1, float t)
{
    return f0 + (t - s0) * (f1 - f0) / (s1 - s0);
}



Vector Vector_Add(Vector v1, Vector v2)
{
    Vector result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}


Vector Vector_Subtract(Vector v1, Vector v2)
{
    Vector result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    return result;
}



Path Path_Initialize(int waypointCount, Vector waypoints[], float durations[])
{
    Path path;
    path.waypointCount = waypointCount;
    for (int point = 0; point < path.waypointCount; point++)
    {
        path.waypoints[point] = waypoints[point];
        path.durations[point] = durations[point];
    }

    Path_Lengths(&path);
    Path_Duration(&path);

    return path;
}

float Path_Lengths(Path* path)
{
    for (int point = 0; point < path->waypointCount; point++)
    {
        if (point < 2 || point > path->waypointCount - 2)
        {
            path->lengths[point] = 0;
        }
        else
        {
            path->lengths[point] = Simpsons_Rule_Compound(*path, 0, (float) (point - 1));
        }
    }

    if (path->waypointCount >= 2)
    {
        path->length = path->lengths[path->waypointCount - 2];
    }
    return path->length;
}

float Path_Duration(Path* path)
{
    path->duration = 0;

    for (int point = 0; point < path->waypointCount; point++)
    {
        path->duration += path->durations[point];
    }

    return path->duration;
}


float Hermite(float p0, float p1, float p2, float p3, float t)
{
    float tSquared = t * t;
    float tCubed = t * tSquared;
    float tension = 1;

    return  (-.5f * tension * tCubed + tension * tSquared - .5f * tension * t) * p0 +
        (1 + .5f * tSquared * (tension - 6) + .5f * tCubed * (4 - tension)) * p1 +
        (.5f * tCubed * (tension - 4) + .5f * tension * t - (tension - 3) * tSquared) * p2 +
        (-.5f * tension * tSquared + .5f * tension * tCubed) * p3;
}

float Hermite_Derivative(float p0, float p1, float p2, float p3, float t)
{
    float tSquared = t * t;
    float tension = 1;

    float P0 = (-1.5f * tension * tSquared + 2 * tension * t - .5f * tension) * p0;
    float P1 = (1 * t * (tension - 6) + 1.5f * tSquared * (4 - tension)) * p1;
    float P2 = (1.5f * tSquared * (tension - 4) + .5f * tension - 2 * (tension - 3) * t) * p2;
    float P3 = (-1 * tension * t + 1.5f * tension * tSquared) * p3;
    return P0 + P1 + P2 + P3;
}

float Arc_Length(Path path, int arcStart, float t)
{
    Vector p1 = path.waypoints[arcStart - 1];
    Vector p2 = path.waypoints[arcStart];
    Vector p3 = path.waypoints[arcStart + 1];
    Vector p4 = path.waypoints[arcStart + 2];

    float dx = Hermite_Derivative( p1.x, p2.x, p3.x, p4.x, t);
    float dy = Hermite_Derivative( p1.y, p2.y, p3.y, p4.y, t);

    return sqrtf(dx*dx + dy*dy);

}

float Simpsons_Rule(Path path, int arcStart, float t)
{
    int n = 10;
    float h = t / n;

    float fx0 = Arc_Length(path, arcStart, 0) + Arc_Length(path, arcStart, t);
    float fx4 = 0;
    float fx2 = 0;

    for (int i4 = 1; i4 < n; i4 += 2)
    {
        fx4 += Arc_Length(path, arcStart, i4 * h);
    }
    for (int i2 = 2; i2 < n; i2 += 2)
    {
        fx2 += Arc_Length(path, arcStart, i2 * h);
    }

    return h/3 * (fx0 + 2 * fx2 + 4 * fx4);;
}

float Simpsons_Rule_Compound(Path path, int arcStart, float t)
{
    float distance = 0;

    for (int arc = arcStart; arc < (int) t; arc++)
    {
        distance += Simpsons_Rule(path, arc + 1, 1);
    }

    if ((int) t != t)
    {
        distance += Simpsons_Rule(path, (int) t + 1, t - (int) t);
    }

    return distance;
}



Vector Follow_Curve(Path path, float t)
{
    int current = (int) t + 1;

    Vector p1 = path.waypoints[current - 1];
    Vector p2 = path.waypoints[current];
    Vector p3 = path.waypoints[current + 1];
    Vector p4 = path.waypoints[current + 2];

    Vector point;
    point.x = Hermite(p1.x, p2.x, p3.x, p4.x, t + 1 - current);
    point.y = Hermite(p1.y, p2.y, p3.y, p4.y, t + 1 - current);

    return point;
}

Vector Follow_Curve_Constant(Path path, float t, bool dragging)
{
    if (t == 0) return path.waypoints[1];

    float tMapped = t;
    for (int point = 2; point < path.waypointCount - 1; point++)
    {
        float pathRatio = path.lengths[point] / path.length * (path.waypointCount - 3);
        float previousPathRatio = path.lengths[point - 1] / path.length * (path.waypointCount - 3);
        if (t < pathRatio)
        {
            tMapped = Map(previousPathRatio, pathRatio, (float) (point - 2), (float) (point - 1), t);
            break;
        }
    }

    float desiredDistance = t * path.length / (path.waypointCount - 3);
    float tLast = tMapped;
    float tConstant = tLast;

    float pathLength;
    float arcLength;

    int count = 0;

    do {
        tLast = tConstant;

        if (fabs(tLast) > (path.waypointCount - 3))
        {
            tLast = (tLast + tMapped) / 2;
            break;
        }
        pathLength = Simpsons_Rule_Compound(path, 0, tLast) - desiredDistance;

        arcLength = Arc_Length(path, (int) tLast + 1, tLast - (int) tLast);
        if (fabs(pathLength) > arcLength || (fabs(pathLength) / arcLength > 0.1 && fabs(fabs(pathLength) - arcLength) < 20))
        {
            tLast = (tLast + tMapped) / 2;
            break;
        }
        tConstant = tLast - pathLength / arcLength;
        count++;

        if (count >= 10)
        {
            if (count >= 100)
            {
                tLast = (tLast + tMapped) / 2;
                break;
            }
        }

    } while (fabs(tLast - tConstant) > 0.03 );
    return Follow_Curve(path, tConstant);
}

bool Check_Collision(BodyCircle body1, BodyCircle body2)
{
    return (body1.position.x - body2.position.x) * (body1.position.x - body2.position.x) +
           (body1.position.y - body2.position.y) * (body1.position.y - body2.position.y)
           <
           (body1.radius + body2.radius) * (body1.radius + body2.radius);
}
