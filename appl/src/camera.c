#include "camera.h"
#include <stdlib.h>
#include <math.h>
#include "common.h"


camera_t* camera_new(float vertical_fov, int width, int height)
{
    camera_t* camera = (camera_t*)malloc(sizeof(camera_t));
    camera->position = (vector3f_t){0, 0, 0};
    camera->vertical_fov = vertical_fov;
    camera->width = width;
    camera->height = height;
    return camera;
}

void camera_free(camera_t* camera)
{
    free(camera);
}

vector3f_t camera_world_to_camera_space(camera_t* camera, vector3f_t wp) 
{
    vector3f_t camera_point = vector3f_sub(wp, camera->position);
    return camera_point;
}


vector2i_t camera_world_to_screen_point(camera_t* camera, vector3f_t wp)
{
    vector3f_t camera_point = vector3f_sub(wp, camera->position);
    
    float plane_x = camera_point.x / -camera_point.z;
    float plane_y = camera_point.y / -camera_point.z;

    float half_fov = camera->vertical_fov * 0.5f;
    float half_fov_rads = half_fov * M_PI / 180.f;
    float tan_fov = tanf(half_fov_rads);

    plane_x /= tan_fov;
    plane_y /= tan_fov;

    float aspect = (float)camera->width / (float)camera->height;
    plane_x /= aspect;

    int screen_x = (plane_x + 1.f) * 0.5f * (float)camera->width;
    int screen_y = (1.f - (plane_y + 1.f) * 0.5f) * (float)camera->height;

    return (vector2i_t){screen_x, screen_y};
}

bool camera_is_triangle_facing_camera(camera_t* camera, vector3f_t* cp1, vector3f_t* cp2, vector3f_t* cp3)
{
    vector3f_t v12 = vector3f_sub(*cp2, *cp1);
    vector3f_t v13 = vector3f_sub(*cp3, *cp1);

    vector3f_t face_norm = vector3f_cross(&v12, &v13);
    vector3f_t cp1_to_camera = vector3f_mult(*cp1, -1.f);
    float dot = vector3f_dot(&cp1_to_camera, &face_norm);
    if (dot >= 0) return true;
    return false;
}

bool camera_is_triangle_in_frustum_simple(camera_t* camera, vector2i_t* sp1, vector2i_t* sp2, vector2i_t* sp3)
{
    if (sp1->x < 0 && sp2->x < 0 && sp3->x < 0) return false;
    if (sp1->y < 0 && sp2->y < 0 && sp3->y < 0) return false;

    if (sp1->x > camera->width && sp2->x > camera->width && sp3->x > camera->width) return false;
    if (sp1->y > camera->height && sp2->y > camera->height && sp3->y > camera->height) return false;

    return true;
}