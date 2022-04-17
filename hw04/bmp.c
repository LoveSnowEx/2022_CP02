#include "basic.h"
#include "bmp.h"

sBmpHeader *read_bmpheader(FILE *stream) {
	sBmpHeader *pHeader;
	pHeader = malloc(sizeof(sBmpHeader));
	size_t res = fread(pHeader, 1, sizeof(sBmpHeader), stream);
	if(res != sizeof(sBmpHeader) || strncmp(pHeader->bm, "BM", 2) != 0) {
		printf("error: file is not bmp format\n");
		free(pHeader);
		return NULL;
	}
	return pHeader;
}

sPixelMask *read_bmpmask(FILE *stream) {
	sPixelMask *pPixelMask;
	pPixelMask = malloc(sizeof(sPixelMask));
	size_t res = fread(pPixelMask, 1, sizeof(sPixelMask), stream);
	if(res != sizeof(sPixelMask)) {
		printf("error: failed to read pixelmask\n");
		free(pPixelMask);
		return NULL;
	}
	return pPixelMask;
}

void *read_bmpbitmap(FILE *stream, size_t size) {
	void *pBitmap = malloc(size);
	size_t res = fread(pBitmap, 1, size, stream);
	if(res != size) {
		printf("error: failed to read bitmap\n");
		free(pBitmap);
		return NULL;
	}
	return pBitmap;
}

sBmpFile *read_bmp(const char *rest) {
	sBmpFile *pBmpFile;
	sBmpHeader *pHeader;
	sPixelMask *pPixelMask;
	void *pBitmap;

	// open file
	FILE *pFile = fopen(rest, "rb");
	if (pFile == NULL) {
		printf("error: failed to open file\n");
		return NULL;
	}

	// read header
	pHeader = read_bmpheader(pFile);
	if(pHeader == NULL) {
		fclose(pFile);
		return NULL;
	}
	
	// read pixel mask
	if(pHeader->compression == 0) pPixelMask = NULL;
	else {
		pPixelMask = read_bmpmask(pFile);
		if(pPixelMask == NULL) {
			free(pHeader);
			fclose(pFile);
			return NULL;
		}
	}
	
	//read bitmap
	pBitmap = read_bmpbitmap(pFile, pHeader->bitmap_size);
	if(pBitmap == NULL) {
		if(pPixelMask) free(pPixelMask);
		free(pHeader);
		fclose(pFile);
		return NULL;
	}

	pBmpFile = malloc(sizeof(sBmpFile));
	pBmpFile->pHeader = pHeader;
	pBmpFile->pPixelmask = pPixelMask;
	pBmpFile->pBitmap = pBitmap;
	fclose(pFile);
	return pBmpFile;
}

void free_bmp(sBmpFile *target) {
	if(!target) return;
	if(target->pHeader) free(target->pHeader);
	if(target->pPixelmask) free(target->pPixelmask);
	if(target->pBitmap) free(target->pBitmap);
	target->pHeader = NULL;
	target->pPixelmask = NULL;
	target->pBitmap = NULL;
	free(target);
}

void print_bmp_header(sBmpHeader *pHeader) {
    printf("ID: %c%c\n", pHeader->bm[0], pHeader->bm[1]);
    printf("Size: %u\n", pHeader->size);
    printf("Reserve: %u\n", pHeader->reserve);
    printf("Offset: %u\n", pHeader->offset);
    printf("Header Size: %u\n", pHeader->header_size);
    printf("Width: %d\n", pHeader->width);
    printf("Height: %d\n", pHeader->height);
    printf("Planes: %u\n", pHeader->planes);
    printf("Bits Per Pixel: %u\n", pHeader->bpp);
    printf("Compression: %u\n", pHeader->compression);
    printf("Bitmap Data Size: %u\n", pHeader->bitmap_size);
    printf("H-Resolution: %u\n", pHeader->hres);
    printf("V-Resolution: %u\n", pHeader->vres);
    printf("Used Colors: %u\n", pHeader->used);
    printf("Important Colors: %u\n", pHeader->important);
}
