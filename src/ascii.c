#include "ascii.h"
#include <stdio.h>

char* ascii_convert(Image* img, int width, int height)
{
    FILE* file = fopen("out.txt", "w");

    for (uint32_t y = 0; y < img->height; y++)
    {
        for (uint32_t x = 0; x < img->width; x++)
        {
            Pixel p = img->data[y * img->width + x];
            int intensity = pixel_luminance(p);

            int i = (ASCII_MAP_SIZE / 256.0f) * (255 - intensity);
            fputc(ASCII_MAP[i], file);
        }

        fputc('\n', file);
    }

    fclose(file);
    return NULL;
}
