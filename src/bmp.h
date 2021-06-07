
// http://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm

#ifndef BMP_H
#define BMP_H

#include <stdint.h>

// Těsné rozložení paměti
#pragma pack(push, 1)

typedef struct {
    uint16_t signature;
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
} BitMapHeader;

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

#pragma pack(pop)

#endif