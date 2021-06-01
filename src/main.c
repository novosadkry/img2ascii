/*
* Kryštof Novosad
* 2.B, 30.05.2021
* PVA - img2ascii
*/

#include <stdio.h>
#include "image.h"
#include "ascii.h"

int main(int argc, char const *argv[])
{
    printf("========== img2ascii ==========\n");

    char path[255];
    printf("Zadejte cestu k souboru: ");
    scanf("%s", path);

    Image* img = image_load(path);
    printf("W:%d H:%d\n", img->width, img->height);

    ascii_convert(img, 0, 0);

    image_free(img);
    return 0;
}
