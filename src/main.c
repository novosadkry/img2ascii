/*
* Kryštof Novosad
* 2.B, 30.05.2021
* PVA - img2ascii
*/

#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "ascii.h"

int main(int argc, char const *argv[])
{
    printf("========== img2ascii ==========\n");

    char path[255];
    printf("Zadejte cestu k souboru: ");
    scanf("%s", path);

    Image* img = image_load(path);

    printf("[W:%d H:%d] -> ", img->width, img->height);
    img = image_resize(img, 200, 100);
    printf("[W:%d H:%d]\n", img->width, img->height);

    ASCII* ascii = ascii_convert(img);

    free(ascii);

    free(img);
    return 0;
}
