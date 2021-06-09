/*
* Kryštof Novosad
* 2.B, 30.05.2021
* PVA - img2ascii
*/

#include "main.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    printf("========== img2ascii ==========\n");

    // Zeptáme se uživatele na zdrojový obrázek
    Image* img = ask_for_image();

    // Zeptáme se uživatele na rozlišení
    uint32_t newRes[2];
    ask_for_resolution(newRes);

    // Pokud zadá jiné rozlišení než originální, změníme velikost
    if (newRes[0] != img->width || newRes[1] != img->height)
        img = image_resize(img, newRes[0], newRes[1]);

    // Obrázek převedeme do ASCII
    ASCII* ascii = ascii_convert(img);

    // Zeptáme se uživatele na způsob výstupu
    ask_for_output(ascii);

    // Uvolníme struktury z paměti
    free(ascii);
    free(img);

    return 0;
}

void ask_for_resolution(uint32_t out[2])
{
    while (1)
    {
        for (int i = 0; i < 2; i++) out[i] = 0;

        printf("Zadejte cilove rozliseni (napr. 200x100): ");
        scanf("%ux%u", out, out + 1);
        flush_stdin();

        if (out[0] != 0 && out[1] != 0)
            break;
    }
}

void ask_for_output(ASCII* ascii)
{
    while (1)
    {
        char select;
        printf("Chcete vypsat vystup do [K]onzole nebo [S]ouboru? ");
        scanf("%c", &select);
        flush_stdin();

        if (toupper(select) == 'K')
        {
            ascii_print(ascii);
            break;
        }

        if (toupper(select) == 'S')
        {
            ascii_save(ascii, "out.txt");
            printf("Vystup byl ulozen do souboru 'out.txt'\n");
            break;
        }
    }
}

Image* ask_for_image()
{
    while (1)
    {
        char path[256];
        printf("Zadejte cestu k souboru: ");
        scanf("%255[^\n]", path);
        flush_stdin();

        Image* img = image_load(path);
        if (img) return img;

        perror("Neplatny soubor");
        errno = 0;
    }
}

void flush_stdin()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}
