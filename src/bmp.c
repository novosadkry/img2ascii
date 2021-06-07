#include "bmp.h"

#include <stdlib.h>
#include <stdbool.h>

int bmp_read(FILE* file, Image** out)
{
    // Přečteme první hlavičku
    BitMapHeader header;
    fread(&header, sizeof(header), 1, file);

    // Zkontrolujeme zdali hlavička souboru je platná
    // (tj. obsahuje písmena 'BM' [42 4D v hex])
    if (header.signature != 0x4D42)
        return 0;

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
    if (!img) return 0;

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

    *out = img;
    return 1;
}