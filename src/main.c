/*
* Kryštof Novosad
* 2.B, 30.05.2021
* PVA - img2ascii
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "image.h"
#include "ascii.h"

void ask_for_resolution(uint32_t out[2])
{
    while (1)
    {
        fflush(stdin);
        for (int i = 0; i < 2; i++) out[i] = 0;

        printf("Zadejte cilove rozliseni (napr. 200x100): ");
        scanf("%ux%u", out, out + 1);

        if (out[0] != 0 && out[1] != 0)
            break;
    }
}

void ask_for_output(ASCII* ascii)
{
    while (1)
    {
        fflush(stdin);

        char select;
        printf("Chcete vypsat vystup do [K]onzole nebo [S]ouboru? ");
        scanf("%c", &select);

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
        fflush(stdin);

        char path[255];
        printf("Zadejte cestu k souboru: ");
        scanf("%s", path);

        Image* img = image_load(path);
        if (img) return img;

        printf("Neplatny soubor!\n");
    }
}

int main(int argc, char const *argv[])
{
    printf("========== img2ascii ==========\n");

    Image* img = ask_for_image();

    uint32_t newRes[2];
    ask_for_resolution(newRes);

    if (newRes[0] != img->width || newRes[1] != img->height)
        img = image_resize(img, newRes[0], newRes[1]);

    ASCII* ascii = ascii_convert(img);
    ask_for_output(ascii);

    free(ascii);
    free(img);

    return 0;
}
