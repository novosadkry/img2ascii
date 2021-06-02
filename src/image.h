#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef uint8_t Byte;

typedef struct {
    Byte r, g, b;
} Pixel;

typedef struct {
    uint32_t width, height;
    Pixel data[];
} Image;

Image* image_load(const char* path);
void image_free(Image* image);

void image_resize(Image** img, uint32_t width, uint32_t height);

#endif