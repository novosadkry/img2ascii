#include "ascii.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Mapa znaků
 *
 * Seřazená podle intenzity sestupně zleva
 * tzn. světlé barvy vlevo
 *      tmavé barvy vpravo
 *
 * (lze libovolně upravit)
*/
char ASCII_MAP[] = " .,:;ox%#@";

ASCII* ascii_convert(Image* img)
{
    uint32_t width = img->width + 1; // + nový řádek
    uint32_t height = img->height;

    // Alokujeme paměť pro celý obraz + nulový znak
    ASCII* out = malloc(sizeof(*out) + width * height + 1);
    if (!out) return NULL;

    // Pro každý pixel v obrázku zjistíme jeho inzenzitu,
    // podle které vybereme odpovídající znak
    int i = 0;
    for (uint32_t y = 0; y < img->height; y++)
    {
        for (uint32_t x = 0; x < img->width; x++)
        {
            Pixel p = img->data[y * img->width + x];

            // Vypočteme intenzitu/svítivost
            int intensity = pixel_luminance(p);

            // Převedeme intenzitu na znak
            int value = (ASCII_MAP_SIZE / 256.0f) * (255 - intensity);

            // Znak zapíšeme a posuneme pozici
            out->data[i++] = ASCII_MAP[value];
        }

        // Zapíšeme nový řádek
        out->data[i++] = '\n';
    }

    // Zapíšeme nakonec nulový znak
    // (potřebný pro str-funkce)
    out->data[i] = '\0';

    // Nastavíme rozměry
    out->width = width;
    out->height = height;

    return out;
}

void ascii_print(ASCII* ascii)
{
    // Vypíšeme všechny znaky
    fputs(ascii->data, stdout);
}

void ascii_save(ASCII* ascii, const char* path)
{
    // Otevřeme soubor pro zápis
    FILE* file = fopen(path, "w");
    if (!file) return;

    // Vypíšeme všechny znaky
    fputs(ascii->data, file);

    // Zavřeme soubor
    fclose(file);
}
