#include "texture.h"
#include "SDL_image.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

texture_t* texture_load(const char* path) 
{
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL) {
        puts("FAILED TO LOAD IMAGE!");
        return NULL;
    }

    texture_t* text = (texture_t*)malloc(sizeof(texture_t));
    text->width = surface->w;
    text->height = surface->h;
    text->channels = surface->format->BytesPerPixel;

    size_t size = text->width * text->height * text->channels;
    text->data = (unsigned char*)malloc(size);

    memcpy(text->data, surface->pixels, size);

    SDL_FreeSurface(surface);
    return text;
}

void texture_free(texture_t* tex)
{
    free(tex->data);
}