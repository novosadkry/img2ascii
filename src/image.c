#include "image.h"
#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

float pixel_avg(Pixel p)
{
    return (p.r + p.g + p.b) / 3.0f;
}

float pixel_luminance(Pixel p)
{
    return (p.r * 0.2126f) + (p.g * 0.7152f) + (p.b * 0.0722f);
}

Image* image_load(const char* path)
{
    // Otevřeme soubor v binárním módu
    FILE* file = fopen(path, "rb");
    if (!file) return NULL;

    Image* img = NULL;

    // Přečteme data z BMP
    if (bmp_read(file, &img));

    // else if (...)

    // Soubor zavřeme
    fclose(file);
    return img;
}

Image* image_resize(Image* img, uint32_t newWidth, uint32_t newHeight)
{
    uint32_t oldWidth = img->width;
    uint32_t oldHeight = img->height;

    // Poměr mezi starým a novým rozměrem
    float xRatio = (float) oldWidth / newWidth;
    float yRatio = (float) oldHeight / newHeight;

    // Celkový počet pixelů v novém obrázku
    size_t pixelCount = newWidth * newHeight;

    // Alokujeme pamět pro nový obrázek + data
    Image* resized = malloc(sizeof(*resized) + sizeof(Pixel) * pixelCount);
    if (!resized) return NULL;

    // Pro každou pozici v novém obrázku
    // zjisti její pozici v starém obrázku
    for (uint32_t y = 0; y < newHeight; y++)
    {
        for (uint32_t x = 0; x < newWidth; x++)
        {
            uint32_t scaledX = x * xRatio;
            uint32_t scaledY = y * yRatio;

            uint32_t new = (newWidth * y) + x;
            uint32_t old = (oldWidth * scaledY) + scaledX;

            resized->data[new] = img->data[old];
        }
    }

    // Uvolníme starý obrázek
    free(img);

    // Nastavíme nové rozměry
    resized->width = newWidth;
    resized->height = newHeight;

    return resized;
}

void image_flip(Image* img)
{
    // Délka řádku v bajtech
    size_t rowSize = sizeof(Pixel) * img->width;

    // Dočasná paměť
    Pixel* tmp = malloc(rowSize);
    if (!tmp) return;

    // Pro každý řádek v první a druhé polovině
    for (uint32_t row = 0; row < img->height / 2; row++)
    {
        // Najdeme jejich počátky
        Pixel* dst = img->data + (row * img->width);
        Pixel* src = img->data + ((img->height - row - 1) * img->width);

        // Prohodíme je
        memcpy(tmp, dst, rowSize);
        memcpy(dst, src, rowSize);
        memcpy(src, tmp, rowSize);
    }

    // Uvolníme dočasnou paměť
    free(tmp);
}
