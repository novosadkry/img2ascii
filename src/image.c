#include "image.h"
#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

Image load_image(const char* path)
{
    Image img;
    img.data = NULL; // Pro případ, že se soubor nepodaří přečíst

    // Otevřeme soubor v binárním módu
    FILE* file = fopen(path, "rb");
    if (!file)
        return img;

    // Přečteme první hlavičku
    BitMapHeader header;
    fread(&header, sizeof(header), 1, file);

    // Zkontrolujeme zdali hlavička souboru je platná
    // (tj. obsahuje písmena 'BM' [0x4D42 v hex])
    if (header.signature != 0x4D42)
    {
        fclose(file);
        return img;
    }

    // Přečteme druhou hlavičku
    BitMapInfoHeader info;
    fread(&info, sizeof(info), 1, file);

    // Alokujeme dostatek paměti pro data
    Pixel* data = (Pixel*)malloc(info.width * info.height * sizeof(Pixel));
    if (!data)
    {
        fclose(file);
        return img;
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

    img.width = info.width;
    img.height = info.height;
    img.data = data;

    return img;
}