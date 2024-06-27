#ifndef VECTOR_H
#define VECTOR_H

typedef struct vector2i_t 
{
    int x;
    int y;
} vector2i_t;

typedef struct vector2f_t 
{
    float x;
    float y;
} vector2f_t;

typedef struct vector3f_t 
{
    float x;
    float y;
    float z;
} vector3f_t;

vector3f_t vector3f_sub(vector3f_t v1, vector3f_t v2);

vector3f_t vector3f_rotate_y(vector3f_t v1, float angle_degrees);

vector3f_t vector3f_mult(vector3f_t v1, float scalar);

vector3f_t vector3f_norm(vector3f_t* v);

float vector3f_magn(vector3f_t* v);

float vector3f_dot(vector3f_t* v1, vector3f_t* v2);

vector3f_t vector3f_refl(vector3f_t* i, vector3f_t* n);

vector3f_t vector3f_cross(vector3f_t* v1, vector3f_t* v2);

#endif //VECTOR_H