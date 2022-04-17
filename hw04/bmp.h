#pragma once

#include "basic.h"

typedef struct _sBmpHeader {
    char bm[2];
    uint32_t size;
    uint32_t reserve;
    uint32_t offset;
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t bitmap_size;
    uint32_t hres;
    uint32_t vres;
    uint32_t used;
    uint32_t important;
} __attribute__((__packed__)) sBmpHeader;

typedef struct _sPixel16 {
    uint16_t blue: 5;
    uint16_t green: 6;
    uint16_t red: 5;
} __attribute__((__packed__)) sPixel16;

typedef struct _sPixel24 {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} __attribute__((__packed__)) sPixel24;

typedef struct _sPixel32 {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha : 5;
    uint8_t reserve : 3;
} __attribute__((__packed__)) sPixel32;

typedef struct _sPixelMask {
    uint32_t r, g, b, a;
} __attribute__ ((__packed__)) sPixelMask;

typedef struct _sBmpFile {
	sBmpHeader *pHeader;
	sPixelMask *pPixelmask;
	void *pBitmap;
} sBmpFile;


sBmpHeader *read_bmpheader(FILE *stream);

sPixelMask *read_bmpmask(FILE *stream);

void *read_bmpbitmap(FILE *stream, size_t size);

sBmpFile *read_bmp(const char *filename);

void free_bmp(sBmpFile *target);

void print_bmp_header(sBmpHeader *pHeader);
