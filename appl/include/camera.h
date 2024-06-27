#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"
#include <stdbool.h>

typedef struct camera_t 
{
    vector3f_t position;
    float vertical_fov;
    int width;
    int height;
} camera_t;

camera_t* camera_new(float vertical_fov, int width, int height);

void camera_free(camera_t* camera);

vector2i_t camera_world_to_screen_point(camera_t* camera, vector3f_t wp);
vector3f_t camera_world_to_camera_space(camera_t* camera, vector3f_t wp);

bool camera_is_triangle_facing_camera(camera_t* camera, vector3f_t* v1, vector3f_t* v2, vector3f_t* v3);

bool camera_is_triangle_in_frustum_simple(camera_t* camera, vector2i_t* sp1, vector2i_t* sp2, vector2i_t* sp3);

#endif //CAMERA_H