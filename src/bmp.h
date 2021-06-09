
// http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

#ifndef BMP_H
#define BMP_H

#include <stdio.h>
#include <stdint.h>
#include "image.h"

// Nastaví těsné rozložení paměti pro struktury
#pragma pack(push, 1)

/**
 * Struktura reprezentující první hlavičku BMP
*/
typedef struct {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
} BitMapHeader;

/**
 * Struktura reprezentující druhou hlavičku BMP
*/
typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint32_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerM;
    int32_t yPixelsPerM;
    uint32_t colorsUsed;
    uint32_t importantColors;
} BitMapInfoHeader;

/**
 * Načte obrázek typu BMP
 *
 * @param file Cílový soubor
 * @param out Návratový obrázek
 * @returns 1 pokud proběhne úspěšně, jinak 0
*/
int bmp_read(FILE* file, Image** out);

// Vypne těsné rozložení
#pragma pack(pop)

#endif