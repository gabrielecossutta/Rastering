#include "screen.h"
#include <stdlib.h>
#include <string.h>

screen_t* screen_new(int width, int height, SDL_Renderer* renderer)
{
    screen_t* screen = (screen_t*)malloc(sizeof(screen_t));
    screen->width = width;
    screen->height = height;
    screen->channels = 4;
    screen->color_buffer_size = width * height * screen->channels;
    screen->color_buffer = (unsigned char*)malloc(screen->color_buffer_size);
    screen->renderer = renderer;
    screen->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);
    screen->depth_buffer_size = width * height * sizeof(float);
    screen->depth_buffer = (float*)malloc(screen->depth_buffer_size);
    return screen;
}

void screen_free(screen_t* screen) 
{   
    SDL_DestroyTexture(screen->texture);
    free(screen->color_buffer);
    free(screen->depth_buffer);
    free(screen);
}

void screen_put_pixel(screen_t* screen, int x, int y, color_t color)
{
    screen_put_pixel_with_depth(screen, x, y, 0.f, color);
}

void screen_put_pixel_with_depth(screen_t* screen, int x, int y, float z, color_t color)
{
    if (x < 0 || x >= screen->width) return;
    if (y < 0 || y >= screen->height) return;

    int depth_index = y * screen->width + x;
    float prev_z = screen->depth_buffer[depth_index];
    if (prev_z > z) return;
    screen->depth_buffer[depth_index] = z;

    int index = (y * screen->width + x) * screen->channels;
    screen->color_buffer[index + 0] = color.r;
    screen->color_buffer[index + 1] = color.g;
    screen->color_buffer[index + 2] = color.b;
    screen->color_buffer[index + 3] = color.a;
}

void screen_blit(screen_t* screen)
{
    SDL_UpdateTexture(screen->texture, NULL, screen->color_buffer, screen->width * screen->channels);
    SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
}

void screen_clear(screen_t* screen)
{
    memset(screen->color_buffer, 0, screen->color_buffer_size);
    memset(screen->depth_buffer, 0xff, screen->depth_buffer_size);
}

void screen_clear_color(screen_t* screen, color_t color)
{
    //memset(screen->color_buffer, 0, screen->color_buffer_size);
    for(size_t i=0; i < screen->color_buffer_size; i+=4 ) {
        screen->color_buffer[i + 0] = color.r;
        screen->color_buffer[i + 1] = color.g;
        screen->color_buffer[i + 2] = color.b;
        screen->color_buffer[i + 3] = color.a;
    }

    memset(screen->depth_buffer, 0xff, screen->depth_buffer_size);
}