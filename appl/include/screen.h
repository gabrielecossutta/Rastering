#ifndef SCREEN_H
#define SCREEN_H
#include "SDL.h"
#include "color.h"

typedef struct screen_t {
    int width;
    int height;
    int channels;
    unsigned char* color_buffer;
    size_t color_buffer_size;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    float* depth_buffer;
    size_t depth_buffer_size;
} screen_t;


screen_t* screen_new(int width, int height, SDL_Renderer* renderer);
void screen_free(screen_t* screen);
void screen_put_pixel(screen_t* screen, int x, int y, color_t color);
void screen_put_pixel_with_depth(screen_t* screen, int x, int y, float z, color_t color);
void screen_blit(screen_t* screen);
void screen_clear(screen_t* screen);
void screen_clear_color(screen_t* screen, color_t color);

#endif //SCREEN_H