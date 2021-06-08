#ifndef MAIN_H
#define MAIN_H

#include <ctype.h>

#include "image.h"
#include "ascii.h"

void flush_stdin();
void ask_for_resolution(uint32_t*);
void ask_for_output(ASCII*);
Image* ask_for_image();

#endif