#include "image.h"
#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

Image* load_image(const char* path)
{
    Image* img = malloc(sizeof(*img));
    if (!img) return NULL;

    // Otevřeme soubor v binárním módu
    FILE* file = fopen(path, "rb");
    if (!file)
    {
        free(img);
        return NULL;
    }

    // Přečteme první hlavičku
    BitMapHeader header;
    fread(&header, sizeof(header), 1, file);

    // Zkontrolujeme zdali hlavička souboru je platná
    // (tj. obsahuje písmena 'BM' [0x4D42 v hex])
    if (header.signature != 0x4D42)
    {
        fclose(file);
        free(img);

        return NULL;
    }

    // Přečteme druhou hlavičku
    BitMapInfoHeader info;
    fread(&info, sizeof(info), 1, file);

    // Alokujeme dostatek paměti pro data
    Pixel* data = malloc(sizeof(*data) * info.width * info.height);
    if (!data)
    {
        fclose(file);
        free(img);

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
        fread(data + (rowsRead * info.width), sizeof(Pixel), info.width, file);
        fseek(file, padding, SEEK_CUR); // Přeskočíme přebytečné bajty

        rowsRead++;
    }

    img->width = info.width;
    img->height = info.height;
    img->data = data;

    return img;
}

void image_free(Image* img)
{
    free(img->data);
    free(img);
}
