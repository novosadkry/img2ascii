#ifndef ASCII_H
#define ASCII_H

#include "image.h"

#define ASCII_MAP_SIZE (sizeof(ASCII_MAP) - 1)
extern char ASCII_MAP[];

char* ascii_convert(Image* img, int width, int height);

#endif