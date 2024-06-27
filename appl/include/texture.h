#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct {
    int channels;
    int width;
    int height;
    unsigned char* data;
} texture_t;

texture_t* texture_load(const char* path);

void texture_free(texture_t* tex);

#endif