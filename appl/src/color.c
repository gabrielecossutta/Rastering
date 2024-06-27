#include "color.h"

color_t color_mult(color_t* color, float scalar)
{
    color_t result;
    result.r = (int)((float)color->r * scalar);
    result.g = (int)((float)color->g * scalar);
    result.b = (int)((float)color->b * scalar);
    result.a = (int)((float)color->a * scalar);
    return result;
}

color_t color_add(color_t* color1, color_t* color2)
{
    color_t result;
    result.r = color1->r + color2->r;
    result.g = color1->g + color2->g;
    result.b = color1->b + color2->b;
    result.a = color1->a + color2->a;
    return result;
}

color_t color_clamp(color_t* color)
{
    color_t result;
    result.r = color->r > 255 ? 255 : color->r;
    result.g = color->g > 255 ? 255 : color->g;
    result.b = color->b > 255 ? 255 : color->b;
    result.a = color->a > 255 ? 255 : color->a;
    return result;
}