#ifndef IMAGE_H
#define IMAGE_H

typedef unsigned char Byte;

typedef struct {
    Byte r, g, b;
} Pixel;

typedef struct {
    size_t width, height;
    Pixel* data;
} Image;

Image load_image(const char* path);

#endif