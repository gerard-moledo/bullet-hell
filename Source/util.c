#include "util.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

Path Path_Initialize(int waypointCount, Vector waypoints[], double durations[])
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

double Path_Lengths(Path* path)
{
    for (int point = 0; point < path->waypointCount; point++)
    {
        if (point < 2 || point > path->waypointCount - 2)
        {
            path->lengths[point] = 0;
        }
        else
        {
            path->lengths[point] = Simpsons_Rule_Compound(*path, 0, (double) point - 1);
        }
    }

    if (path->waypointCount >= 2)
    {
        path->length = path->lengths[path->waypointCount - 2];
    }
    return path->length;
}

double Path_Duration(Path* path)
{
    path->duration = 0;

    for (int point = 0; point < path->waypointCount; point++)
    {
        path->duration += path->durations[point];
    }

    return path->duration;
}

int Round_To_Int(double num)
{
    return (int) (num < 0 ? num - 0.5 : num + 0.5);
}

double Distance(double x1, double y1, double x2, double y2)
{
    return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double Lerp(double start, double finish, double t)
{
    return start + t * (finish - start);
}

double Map(double s0, double s1, double f0, double f1, double t)
{
    return f0 + (t - s0) * (f1 - f0) / (s1 - s0);
}

double Hermite(double p0, double p1, double p2, double p3, double t)
{
    double tSquared = t * t;
    double tCubed = t * tSquared;
    double tension = 1;

    return  (-.5 * tension * tCubed + tension * tSquared - .5 * tension * t) * p0 +
        (1 + .5 * tSquared * (tension - 6) + .5 * tCubed * (4 - tension)) * p1 +
        (.5 * tCubed * (tension - 4) + .5 * tension * t - (tension - 3) * tSquared) * p2 +
        (-.5 * tension * tSquared + .5 * tension * tCubed) * p3;
}

double Hermite_Derivative(double p0, double p1, double p2, double p3, double t)
{
    double tSquared = t * t;
    double tension = 1;

    double P0 = (-1.5 * tension * tSquared + 2 * tension * t - .5 * tension) * p0;
    double P1 = (1 * t * (tension - 6) + 1.5 * tSquared * (4 - tension)) * p1;
    double P2 = (1.5 * tSquared * (tension - 4) + .5 * tension - 2 * (tension - 3) * t) * p2;
    double P3 = (-1 * tension * t + 1.5 * tension * tSquared) * p3;
    return P0 + P1 + P2 + P3;
}

double Arc_Length(Path path, int arcStart, double t)
{
    Vector p1 = path.waypoints[arcStart - 1];
    Vector p2 = path.waypoints[arcStart];
    Vector p3 = path.waypoints[arcStart + 1];
    Vector p4 = path.waypoints[arcStart + 2];

    double dx = Hermite_Derivative( p1.x, p2.x, p3.x, p4.x, t);
    double dy = Hermite_Derivative( p1.y, p2.y, p3.y, p4.y, t);

    return sqrt(dx*dx + dy*dy);

}

double Simpsons_Rule(Path path, int arcStart, double t)
{
    int n = 10;
    double h = t / n;

    double fx0 = Arc_Length(path, arcStart, 0) + Arc_Length(path, arcStart, t);
    double fx4 = 0;
    double fx2 = 0;

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

double Simpsons_Rule_Compound(Path path, int arcStart, double t)
{
    double distance = 0;

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



Vector Follow_Curve(Path path, double t)
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

Vector Follow_Curve_Constant(Path path, double t, bool dragging)
{
    if (t == 0) return path.waypoints[1];

    double tMapped = t;
    for (int point = 2; point < path.waypointCount - 1; point++)
    {
        double pathRatio = path.lengths[point] / path.length * (path.waypointCount - 3);
        double previousPathRatio = path.lengths[point - 1] / path.length * (path.waypointCount - 3);
        if (t < pathRatio)
        {
            tMapped = Map(previousPathRatio, pathRatio, point - 2, point - 1, t);
            break;
        }
    }

    double desiredDistance = t * path.length / (path.waypointCount - 3);
    double tLast = tMapped;
    double tConstant = tLast;

    double pathLength;
    double arcLength;

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
