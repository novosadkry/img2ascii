#include "ascii.h"
#include <stdio.h>
#include <stdlib.h>

char ASCII_MAP[] = " .,:;ox%#@";

ASCII* ascii_convert(Image* img)
{
    uint32_t width = img->width + 1;
    uint32_t height = img->height;

    ASCII* out = malloc(sizeof(*out) + width * height);
    if (!out) return NULL;

    int i = 0;
    for (uint32_t y = 0; y < img->height; y++)
    {
        for (uint32_t x = 0; x < img->width; x++)
        {
            Pixel p = img->data[y * img->width + x];
            int intensity = pixel_luminance(p);

            int value = (ASCII_MAP_SIZE / 256.0f) * (255 - intensity);
            out->data[i++] = ASCII_MAP[value];
        }

        out->data[i++] = '\n';
    }

    out->width = width;
    out->height = height;
    return out;
}
