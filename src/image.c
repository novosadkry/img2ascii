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

    // Přečteme první hlavičku
    BitMapHeader header;
    fread(&header, sizeof(header), 1, file);

    // Zkontrolujeme zdali hlavička souboru je platná
    // (tj. obsahuje písmena 'BM' [42 4D v hex])
    if (header.signature != 0x4D42)
    {
        fclose(file);
        return NULL;
    }

    // Přečteme druhou hlavičku
    BitMapInfoHeader info;
    fread(&info, sizeof(info), 1, file);

    // Přetočíme data pokud je výška kladná
    bool flip = info.height > 0;
    info.height = abs(info.height);

    // Vypočítáme celkový počet pixelů
    size_t pixelCount = info.width * info.height;

    // Alokujeme dostatek paměti pro celý Image + data
    Image* img = malloc(sizeof(*img) + sizeof(Pixel) * pixelCount);
    if (!img)
    {
        fclose(file);
        return NULL;
    }

    // Posuneme pozici čtečky na začátek dat
    fseek(file, header.dataOffset, SEEK_SET);

    // Vypočítáme přebytečné bajty
    int padding = (4 - (info.width * sizeof(Pixel) % 4)) % 4;

    // Čteme po řádcích (scanlines)
    int rowsRead = 0;
    while (rowsRead < info.height)
    {
        fread(img->data + (rowsRead * info.width), sizeof(Pixel), info.width, file);
        fseek(file, padding, SEEK_CUR); // Přeskočíme přebytečné bajty

        rowsRead++;
    }

    // Prohodíme BGR na RGB
    for (size_t i = 0; i < pixelCount; i++)
    {
        uint8_t tmp = img->data[i].r;
        img->data[i].r = img->data[i].b;
        img->data[i].b = tmp;
    }

    // Nastavíme proměnné
    img->width = info.width;
    img->height = info.height;

    // Přetočíme
    if (flip) image_flip(img);

    fclose(file);
    return img;
}

void image_resize(Image** img, uint32_t width, uint32_t height)
{
    float xRatio = (*img)->width  / (float) width;
    float yRatio = (*img)->height / (float) height;

    size_t pixelCount = width * height;
    Image* resized = malloc(sizeof(*resized) + sizeof(Pixel) * pixelCount);

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            uint32_t i = y * yRatio * (*img)->width + x * xRatio;
            resized->data[y * width + x] = (*img)->data[i];
        }
    }

    image_free(*img);

    resized->width = width;
    resized->height = height;
    *img = resized;
}

void image_flip(Image* img)
{
    size_t rowSize = sizeof(Pixel) * img->width;
    Pixel* tmp = malloc(rowSize);

    for (uint32_t row = 0; row < img->height / 2; row++)
    {
        Pixel* dst = img->data + (row * img->width);
        Pixel* src = img->data + ((img->height - row - 1) * img->width);

        // Prohodíme hodnoty
        memcpy(tmp, dst, rowSize);
        memcpy(dst, src, rowSize);
        memcpy(src, tmp, rowSize);
    }

    free(tmp);
}

void image_free(Image* img)
{
    free(img);
}
