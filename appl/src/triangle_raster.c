#include "triangle_raster.h"

int min3(int a, int b, int c) 
{
    int result = a;
    if (b < result) result = b;
    if (c < result) result = c;
    return result;
}

int max3(int a, int b, int c) 
{
    int result = a;
    if (b > result) result = b;
    if (c > result) result = c;
    return result;
}

int det(vector2i_t v1, vector2i_t v2, vector2i_t p) 
{
    //V1 -> V2
    int vx = v2.x - v1.x;
    int vy = v2.y - v1.y;

    //V1 -> P
    int px = p.x - v1.x;
    int py = p.y - v1.y;

    return (vx * py) - (vy * px);
}


bool bbox_is_point_in_triangle(vector2i_t P, vector2i_t A, vector2i_t B, vector2i_t C)
{
    bool neg_ABP = det(A, B, P) < 0;
    bool neg_BCP = det(B, C, P) < 0;
    bool neg_CAP = det(C, A, P) < 0;
    return (neg_ABP == neg_BCP) && (neg_BCP == neg_CAP);
}

void bbox_triangle_raster(screen_t* screen, vector2i_t p1, vector2i_t p2, vector2i_t p3, color_t color)
{
    int x_min = min3(p1.x, p2.x, p3.x);
    int y_min = min3(p1.y, p2.y, p3.y);

    int x_max = max3(p1.x, p2.x, p3.x);
    int y_max = max3(p1.y, p2.y, p3.y);

    for(int y = y_min; y <= y_max; ++y) 
    {
        for(int x = x_min; x <= x_max; ++x) 
        {
            vector2i_t point = {x, y};
            if (bbox_is_point_in_triangle(point, p1, p2, p3)) 
            {
                screen_put_pixel(screen, x, y, color);
            }
        }
    }
}