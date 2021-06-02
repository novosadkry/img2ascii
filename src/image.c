#include "image.h"
#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

    // Přetočíme data pouze pokud je výška kladná
    bool flip = info.height > 0;
    info.height = abs(info.height);

    // Alokujeme dostatek paměti pro celý Image + data
    Image* img = malloc(sizeof(*img) + sizeof(Pixel) * info.width * info.height);
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

    // ? Přehodit přímo při čtení ?
    if (flip)
    {
        size_t rowSize = sizeof(Pixel) * info.width;
        Pixel* tmp = malloc(rowSize);

        for (uint32_t row = 0; row < info.height / 2; row++)
        {
            Pixel* dst = img->data + (row * info.width);
            Pixel* src = img->data + ((info.height - row - 1) * info.width);

            // Prohodíme hodnoty
            memcpy(tmp, dst, rowSize);
            memcpy(dst, src, rowSize);
            memcpy(src, tmp, rowSize);
        }

        free(tmp);
    }

    img->width = info.width;
    img->height = info.height;

    fclose(file);
    return img;
}

void image_free(Image* img)
{
    free(img);
}

void image_resize(Image** img, uint32_t width, uint32_t height)
{
    float xRatio = (*img)->width / width;
    float yRatio = (*img)->height / height;

    Image* resized = malloc(sizeof(*resized) + sizeof(Pixel) * width * height);

    for (uint32_t y = 0; y < height; y++)
    {
        for (uint32_t x = 0; x < width; x++)
        {
            resized->data[y * width + x] = (*img)->data[(int)(y * yRatio * width) + (int)(x * xRatio)];
        }
    }

    image_free(*img);

    resized->width = width;
    resized->height = height;
    *img = resized;
}
