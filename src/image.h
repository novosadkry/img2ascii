#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef uint8_t Byte;

typedef struct {
    Byte r, g, b;
} Pixel;

typedef struct {
    uint32_t width, height;
    Pixel* data;
} Image;

Image load_image(const char* path);

#endif