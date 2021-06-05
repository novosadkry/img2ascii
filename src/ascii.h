#ifndef ASCII_H
#define ASCII_H

#include "image.h"

#define ASCII_MAP_SIZE (sizeof(ASCII_MAP) - 1)
extern char ASCII_MAP[];

typedef struct {
    uint32_t width, height;
    char data[];
} ASCII;

ASCII* ascii_convert(Image* img);

#endif