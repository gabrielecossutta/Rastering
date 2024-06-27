#ifndef LINE_RASTER_H
#define LINE_RASTER_H
#include "screen.h"
#include "color.h"

void dda_line_raster(screen_t* screen, int x1, int y1, int x2, int y2, color_t color);

#endif