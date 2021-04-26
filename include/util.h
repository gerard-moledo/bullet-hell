#ifndef UTIL_H
#define UTIL_H

typedef enum { false = 0, true } bool;
typedef enum { toggle_up = 1, toggle_down = -1, toggle_none = 0 } Toggle;

#ifndef _MATH_DEFINES_DEFINED
#define _MATH_DEFINES_DEFINED
#define M_PI 3.14159265358979323846264338327950288
#endif

typedef struct {
    double x, y;
} Vector;

typedef struct {
    int x, y;
} VectorInt;

#define MAX_PATH_COUNT 20

typedef struct {
    int waypointCount;
    Vector waypoints[MAX_PATH_COUNT];
    double lengths[MAX_PATH_COUNT];
    double durations[MAX_PATH_COUNT];

    double length;
    double duration;
} Path;

typedef struct {
    Vector position;
    double radius;
} BodyCircle;

Path Path_Initialize(int waypointCount, Vector waypoints[], double durations[]);
double Path_Lengths(Path* path);
double Path_Duration(Path* path);

int Round_To_Int(double num);
double Distance(double x1, double y1, double x2, double y2);
double Lerp(double start, double finish, double t);
double Map(double s0, double s1, double f0, double f1, double t);

double Hermite(double p0, double p1, double p2, double p3, double t);
double Hermite_Derivative(double p0, double p1, double p2, double p3, double t);
double Arc_Length(Path path, int arcStart, double t);
double Simpsons_Rule(Path path, int arcStart, double t);
double Simpsons_Rule_Compound(Path path, int arcStart, double t);

Vector Follow_Curve(Path path, double t);
Vector Follow_Curve_Constant(Path path, double t, bool dragging);

bool Check_Collision(BodyCircle pos1, BodyCircle pos2);

#endif
