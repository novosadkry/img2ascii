#ifndef ASCII_H
#define ASCII_H

#include "image.h"

#define ASCII_MAP_SIZE (sizeof(ASCII_MAP) - 1)
static char ASCII_MAP[] = " .,:;ox%#@";

void ascii_convert(Image* img);

#endif