#ifndef TRIANGLE_RASTER_H
#define TRIANGLE_RASTER_H
#include "vector.h"
#include "screen.h"
#include <stdbool.h>

void bbox_triangle_raster(screen_t* screen, vector2i_t p1, vector2i_t p2, vector2i_t p3, color_t color);
bool bbox_is_point_in_triangle(vector2i_t point, vector2i_t p1, vector2i_t p2, vector2i_t p3);

#endif //TRIANGLE_RASTER_H

