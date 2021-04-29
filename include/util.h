#ifndef UTIL_H
#define UTIL_H

typedef enum { false = 0, true } bool;
typedef enum { toggle_up = 1, toggle_down = -1, toggle_none = 0 } Toggle;

#define F_PI 3.1415927f


typedef struct {
    float x, y;
} Vector;

typedef struct {
    int x, y;
} VectorInt;

#define MAX_PATH_COUNT 20

typedef struct {
    int waypointCount;
    Vector waypoints[MAX_PATH_COUNT];
    float lengths[MAX_PATH_COUNT];
    float durations[MAX_PATH_COUNT];

    float length;
    float duration;
} Path;

typedef struct {
    Vector position;
    float radius;
} BodyCircle;

Path Path_Initialize(int waypointCount, Vector waypoints[], float durations[]);
float Path_Lengths(Path* path);
float Path_Duration(Path* path);

int Round_To_Int(float num);
int Sign(float num);
float Distance(float x1, float y1, float x2, float y2);
float DistanceI(int x1, int y1, int x2, int y2);
float Lerp(float start, float finish, float t);
float Map(float s0, float s1, float f0, float f1, float t);

float Hermite(float p0, float p1, float p2, float p3, float t);
float Hermite_Derivative(float p0, float p1, float p2, float p3, float t);
float Arc_Length(Path path, int arcStart, float t);
float Simpsons_Rule(Path path, int arcStart, float t);
float Simpsons_Rule_Compound(Path path, int arcStart, float t);

Vector Follow_Curve(Path path, float t);
Vector Follow_Curve_Constant(Path path, float t, bool dragging);

bool Check_Collision(BodyCircle pos1, BodyCircle pos2);

#endif
