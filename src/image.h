#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct {
    uint8_t r, g, b;
} Pixel;

typedef struct {
    uint32_t width, height;
    Pixel data[];
} Image;

float pixel_avg(Pixel p);
float pixel_luminance(Pixel p);

Image* image_load(const char* path);
Image* image_resize(Image* img, uint32_t width, uint32_t height);
void image_flip(Image* img);
void image_free(Image* img);

#endif