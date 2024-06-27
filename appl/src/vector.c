#include "vector.h"
#include <math.h>
#include "common.h"
#include <math.h>

vector3f_t vector3f_sub(vector3f_t v1, vector3f_t v2)
{
    vector3f_t r;
    r.x = v1.x - v2.x;
    r.y = v1.y - v2.y;
    r.z = v1.z - v2.z;
    return r;
}

/*
  x2 = cosA*x1 - sinA*y1
  y2 = sinA*x1 + cosA*y1
*/
vector3f_t vector3f_rotate_y(vector3f_t v1, float angle_degrees) 
{
    float rads = angle_degrees * M_PI / 180.f;

    vector3f_t r;
    r.x = cosf(rads) * v1.x - sinf(rads) * v1.z;
    r.y = v1.y;
    r.z = sinf(rads) * v1.x + cosf(rads) * v1.z;
    return r;
}

//TODO: Evaluate to use vector pointer (or eventually vector value type) for all vector operation
vector3f_t vector3f_mult(vector3f_t v1, float scalar) 
{
    vector3f_t r;
    r.x = v1.x * scalar;
    r.y = v1.y * scalar;
    r.z = v1.z * scalar;
    return r;
}

vector3f_t vector3f_norm(vector3f_t* v) 
{
    float inv_magn = 1.f / vector3f_magn(v);
    return vector3f_mult(*v, inv_magn);
}

float vector3f_magn(vector3f_t* v) 
{
    //return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    return sqrtf(vector3f_dot(v, v));
}

float vector3f_dot(vector3f_t* v1, vector3f_t* v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

vector3f_t vector3f_refl(vector3f_t* i, vector3f_t* n)
{
    //R = I – 2 * dot(I,N) * N
    float dot2 = 2.f * vector3f_dot(i, n);
    vector3f_t dot2n = vector3f_mult(*n, dot2);
    return vector3f_sub(*i, dot2n);
}

/*
cx = a->y * b->z - a->z * b->y;
cy = a->z * b->x - a->x * b->z;
cz = a->x * b->y - a->y * b->x;

*/
vector3f_t vector3f_cross(vector3f_t* v1, vector3f_t* v2)
{
    vector3f_t cross;
    cross.x = v1->y * v2->z - v1->z * v2->y;
    cross.y = v1->z * v2->x - v1->x * v2->z;
    cross.z = v1->x * v2->y - v1->y * v2->x;
    return cross;
}