#ifndef MAIN_H
#define MAIN_H

#include "image.h"
#include "ascii.h"

/**
 * Vypustí ze vstupu všechny přebývající znaky
 * @see https://stackoverflow.com/questions/19206368/alternative-to-c-library-function-fflushstdin
*/
void flush_stdin();

/**
 * Zeptá se uživatele na nové rozlišení obrázku
*/
void ask_for_resolution(uint32_t*);

/**
 * Zeptá se uživatele na požadovaný způsob výstupu
*/
void ask_for_output(ASCII*);

/**
 * Zeptá se uživatele na zdrojový obrázek
*/
Image* ask_for_image();

#endif