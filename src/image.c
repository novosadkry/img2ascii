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

    // Alokujeme dostatek paměti pro celý obrázek + data
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

    // Nastavíme rozměry
    img->width = info.width;
    img->height = info.height;

    // Přetočíme
    if (flip) image_flip(img);

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
