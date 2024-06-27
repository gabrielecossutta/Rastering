#include "line_raster.h"

/// @brief rasterize a line from p1 (x1,y2) to p2 (x2,y2)
void dda_line_raster(screen_t* screen, int x1, int y1, int x2, int y2, color_t color) 
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int steps = (dx > dy) ? dx : dy;

    int sx = (x1 > x2) ? -1 : 1;
    int sy = (y1 > y2) ? -1 : 1;

    float step_x = (float)dx * (float)sx / (float)steps;
    float step_y = (float)dy * (float)sy / (float)steps;

    float x = (float)x1;
    float y = (float)y1;
    for (int i = 0; i < steps; i++)
    {
        //SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
        //SDL_RenderDrawPoint(r, x, y);
        screen_put_pixel(screen, x, y, color);

        x += step_x;
        y += step_y;
    }
}