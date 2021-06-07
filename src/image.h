#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct {
    uint8_t r, g, b;
} Pixel;

typedef struct {
    uint32_t width, height;
    Pixel data[];
} Image;

/**
 * Vypočítá průměrnou svítivost pixelu
 *
 * @param p Pixel
 * @returns Hodnotu v rozmezí 0-255
*/
float pixel_avg(Pixel p);

/**
 * Vypočítá relativní pozorovanou svítivost pixelu
 * @see https://en.wikipedia.org/wiki/Relative_luminance
 *
 * @param p Pixel
 * @returns Hodnotu v rozmezí 0-255
*/
float pixel_luminance(Pixel p);

/**
 * Načte obrázek ze souboru
 *
 * @param path Cesta k souboru
*/
Image* image_load(const char* path);

/**
 * Změní velikost obrázku
 *
 * @param img Obrázek
 * @param newWidth Nová šířka
 * @param newHeight Nová výška
 * @returns Nový obrázek s daným rozměrem, starý obrázek je uvolněn z paměti
*/
Image* image_resize(Image* img, uint32_t width, uint32_t height);

/**
 * Přetočí obrázek
 *
 * @param img Obrázek
*/
void image_flip(Image* img);

#endif