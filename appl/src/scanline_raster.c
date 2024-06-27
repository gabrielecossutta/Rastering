#include "scanline_raster.h"
#include "color.h"
#include <float.h>


static void _sort_by_y(vertex_t** op1, vertex_t** op2, vertex_t** op3) 
{   
    vertex_t* temp;
    if ( (*op1)->screen_pos->y > (*op2)->screen_pos->y) {
        temp = *op1;
        *op1 = *op2;
        *op2 = temp;
    }

    if ( (*op2)->screen_pos->y > (*op3)->screen_pos->y) {
        temp = *op2;
        *op2 = *op3;
        *op3 = temp;
    }

    if ( (*op1)->screen_pos->y > (*op2)->screen_pos->y) {
        temp = *op1;
        *op1 = *op2;
        *op2 = temp;
    }
}

static float clampf(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}


static float _interpolate_scalar(float value_a, float value_b, float gradient) {
    return  value_a + (value_b - value_a) * gradient;         
}

static color_t _interpolate_color(color_t* color_a, color_t* color_b, float gradient) {
    color_t result;
    result.r = (unsigned char)_interpolate_scalar((float)color_a->r, (float)color_b->r, gradient);
    result.g = (unsigned char)_interpolate_scalar((float)color_a->g, (float)color_b->g, gradient);
    result.b = (unsigned char)_interpolate_scalar((float)color_a->b, (float)color_b->b, gradient);
    result.a = (unsigned char)_interpolate_scalar((float)color_a->a, (float)color_b->a, gradient);
    return result;
}

static vector2f_t _interpolate_vector2f(vector2f_t* v_a, vector2f_t* v_b, float gradient) {
    vector2f_t result;
    result.x = _interpolate_scalar(v_a->x, v_b->x, gradient);
    result.y = _interpolate_scalar(v_a->y, v_b->y, gradient);
    return result;
}

static vector3f_t _interpolate_vector3f(vector3f_t* v_a, vector3f_t* v_b, float gradient) {
    vector3f_t result;
    result.x = _interpolate_scalar(v_a->x, v_b->x, gradient);
    result.y = _interpolate_scalar(v_a->y, v_b->y, gradient);
    result.z = _interpolate_scalar(v_a->z, v_b->z, gradient);
    return result;
}


static void _interpolate_row(gpu_t* gpu, int y, vertex_t* left_edge_v1, vertex_t* left_edge_v2, vertex_t* right_edge_v1, vertex_t* right_edge_v2)
{
    vector2i_t* left_edge_p1 = left_edge_v1->screen_pos;
    vector2i_t* left_edge_p2 = left_edge_v2->screen_pos;

    vector2i_t* right_edge_p1 = right_edge_v1->screen_pos;
    vector2i_t* right_edge_p2 = right_edge_v2->screen_pos;

    float left_gradient_y = 1.f;
    if (left_edge_p1->y != left_edge_p2->y) 
    {
        left_gradient_y = (float)(y - left_edge_p1->y) / (float)(left_edge_p2->y - left_edge_p1->y);
    }

    float right_gradient_y = 1.f;
    if (right_edge_p1->y != right_edge_p2->y) 
    {
        right_gradient_y = (float)(y - right_edge_p1->y) / (float)(right_edge_p2->y - right_edge_p1->y);
    }

    int left_x = (int)_interpolate_scalar( (float)left_edge_p1->x, (float)left_edge_p2->x, left_gradient_y);
    int right_x = (int)_interpolate_scalar( (float)right_edge_p1->x, (float)right_edge_p2->x, right_gradient_y);

    float left_z = _interpolate_scalar(left_edge_v1->z_pos, left_edge_v2->z_pos, left_gradient_y);
    float right_z = _interpolate_scalar(right_edge_v1->z_pos, right_edge_v2->z_pos, right_gradient_y);

    color_t left_color;
    color_t right_color;
    if (gpu->flags & GPU_FLAG_COLOR_MODE) {
        left_color = _interpolate_color(left_edge_v1->color, left_edge_v2->color, left_gradient_y);
        right_color = _interpolate_color(right_edge_v1->color, right_edge_v2->color, right_gradient_y);
    }

    vector2f_t left_uv;
    vector2f_t right_uv;
    if (gpu->flags & GPU_FLAG_TEXTURE_MODE) {
        left_uv = _interpolate_vector2f(left_edge_v1->text_coord, left_edge_v2->text_coord, left_gradient_y);
        right_uv = _interpolate_vector2f(right_edge_v1->text_coord, right_edge_v2->text_coord, right_gradient_y);
    }

    vector3f_t left_world_pos = _interpolate_vector3f(left_edge_v1->world_pos, left_edge_v2->world_pos, left_gradient_y);
    vector3f_t right_world_pos = _interpolate_vector3f(right_edge_v1->world_pos, right_edge_v2->world_pos, right_gradient_y);

    vector3f_t left_world_norm = _interpolate_vector3f(left_edge_v1->world_norm, left_edge_v2->world_norm, left_gradient_y);
    vector3f_t right_world_norm = _interpolate_vector3f(right_edge_v1->world_norm, right_edge_v2->world_norm, right_gradient_y);

    for(int x = left_x; x <= right_x; ++x) 
    {
        float gradient_x = 1.f;
        if (left_x < right_x) 
        {
            gradient_x = (float)(x - left_x)/ (float)(right_x - left_x);
        }

        float sample_z = _interpolate_scalar(left_z, right_z, gradient_x);

        color_t sampled_color;
        
        if (gpu->flags & GPU_FLAG_COLOR_MODE) {
            sampled_color = _interpolate_color(&left_color, &right_color, gradient_x);
        }

        if (gpu->flags & GPU_FLAG_TEXTURE_MODE) {
            vector2f_t sampled_uv = _interpolate_vector2f(&left_uv, &right_uv, gradient_x);
            texture_t* tex = gpu->texture;

            int tex_x = (int)(sampled_uv.x * (float)tex->width);
            int tex_y = (int)((1.f - sampled_uv.y) * (float)tex->height);

            int tex_index = (tex_y * tex->width + tex_x) * tex->channels;
            sampled_color.r = tex->data[tex_index + 0];
            sampled_color.g = tex->data[tex_index + 1];
            sampled_color.b = tex->data[tex_index + 2];
            sampled_color.a = tex->data[tex_index + 3];
        }

        //Compute Phong

        //1. Ambient
        float ambient_intensity = 0.1f;
        color_t ambient = color_mult(&sampled_color, ambient_intensity);

        //2. Diffuse
        vector3f_t world_pos = _interpolate_vector3f(&left_world_pos, &right_world_pos, gradient_x);
        vector3f_t dir_to_light = vector3f_sub(gpu->point_light_pos, world_pos);
        dir_to_light = vector3f_norm(&dir_to_light);

        vector3f_t world_norm = _interpolate_vector3f(&left_world_norm, &right_world_norm, gradient_x);
        world_norm = vector3f_norm(&world_norm);

        float cosLN = vector3f_dot(&dir_to_light, &world_norm);
        float lambert = clampf(cosLN, 0.f, 1.f);
        color_t diffuse = color_mult(&sampled_color, lambert);

        //3. Specular
        vector3f_t dir_to_eye = vector3f_sub(gpu->camera_pos, world_pos);
        dir_to_eye = vector3f_norm(&dir_to_eye);

        vector3f_t dir_light_to_point = vector3f_mult(dir_to_light, -1.f);
        vector3f_t dir_to_light_refl = vector3f_refl(&dir_light_to_point, &world_norm);
        float cosER = vector3f_dot(&dir_to_light_refl, &dir_to_eye);
        float specular_value = clampf(cosER, 0.f, 1.f);

        color_t specular_color = (color_t){255, 255, 255, 255};
        color_t specular = color_mult(&specular_color, powf(specular_value, 50.f));

        color_t phong = (color_t){0, 0, 0, 0};
        phong = color_add(&phong, &ambient);
        phong = color_add(&phong, &diffuse);
        phong = color_add(&phong, &specular);

        phong = color_clamp(&phong);

        screen_put_pixel_with_depth(gpu->screen, x, y, sample_z, phong);
    }
}

void scanline_raster(gpu_t* gpu, vertex_t* v1, vertex_t* v2, vertex_t* v3) 
{
   
    _sort_by_y(&v1, &v2, &v3);

    vector2i_t* op1 = v1->screen_pos;
    vector2i_t* op2 = v2->screen_pos;
    vector2i_t* op3 = v3->screen_pos;

    //TODO: Understand default value to assign when Y on the divider are equal
    float inv_slope_p1p2 = (float)(op2->x - op1->x) / (float)(op2->y - op1->y);
    float inv_slope_p1p3 = (float)(op3->x - op1->x) / (float)(op3->y - op1->y);

    // <|
    if (inv_slope_p1p2 < inv_slope_p1p3) 
    {
        for(int y = op1->y; y <= op3->y; ++y) 
        {
            if (y < op2->y) //phase1: upper triangle: left: p1p2 right: p1p3
            {
                _interpolate_row(gpu, y, v1, v2, v1, v3);
            } 
            else //phase2: lower triangle: left: p2p3 right: p1p3
            {
                _interpolate_row(gpu, y, v2, v3, v1, v3);
            }
        }
    } 
    else // |>
    {
        for(int y = op1->y; y <= op3->y; ++y) 
        {
            if (y < op2->y) //phase1: upper triangle: left: p1p3 right: p1p2
            {
                _interpolate_row(gpu, y, v1, v3, v1, v2);
            } 
            else //phase2: lower triangle: left: p1p3 right: p2p3
            {
                _interpolate_row(gpu, y, v1, v3, v2, v3);
            }
        }
    }
}