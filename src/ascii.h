#ifndef ASCII_H
#define ASCII_H

#include "image.h"

#define ASCII_MAP_SIZE (sizeof(ASCII_MAP) - 1)
extern char ASCII_MAP[]; // Definováno v ascii.c

/**
 * Struktura reprezentující obraz ASCII
 *
 * Obsahuje informace o výšce a šířce obrazu
 * a jednotlivé znaky
*/
typedef struct {
    uint32_t width, height;
    char data[];
} ASCII;

/**
 * Převede obrázek na ASCII obraz
 *
 * @param img Obrázek
*/
ASCII* ascii_convert(Image* img);

/**
 * Vypíše obraz ASCII do konzole
 *
 * @param ascii ASCII obraz
*/
void ascii_print(ASCII* ascii);

/**
 * Uloží obraz ASCII do souboru
 *
 * @param ascii ASCII obraz
 * @param path Cesta a název souboru
*/
void ascii_save(ASCII* ascii, const char* path);

#endif