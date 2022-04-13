#include "basic.h"
#include "bmp.h"

int main() {
	sBmpFile *pBmpFile;
	char infilename[1024], outfilename[1024];
	
	//enter input file
	printf("Please enter the input image name: ");
	while(!fgets_n(infilename, 1024, stdin));
	if(read_bmp(infilename, &pBmpFile) == -1) {
		return 1;
	}
	
	//enter output file
	printf("Please enter the output image name: ");
	while(!fgets_n(outfilename, 1024, stdin));
	FILE *pFile2 = fopen(outfilename, "wb");
	
	if (pFile2 == NULL) {
		printf("File could not be opened!\n");
		return 1;
	}
	
	const sPixelMask mask = {.r = 0xf800, .g = 0x07e0, .b = 0x001f, .a = 0x0000};

	sBmpHeader *pHeader = pBmpFile->pHeader;
	pHeader->bpp = 16;
	pHeader->offset += sizeof(mask);
	pHeader->bitmap_size = pHeader->bitmap_size / 24 * 16;
	pHeader->size = pHeader->offset + pHeader->bitmap_size;
	pHeader->compression = 3;
	fwrite(pHeader, 1, sizeof(sBmpHeader), pFile2);
	fwrite(&mask, 1, sizeof(mask), pFile2);

	
	// printf("============header=============\n");
	// print_bmp_header(&header);
	// printf("============header2=============\n");
	// print_bmp_header(&header2);
	

	sPixel24 *pixels = pBmpFile->pBitmap;
	for (i32 i = 0; i < pHeader->height; i++) {
		for (i32 j = 0; j < pHeader->width; j++) {
			sPixel16 tmp = {
				.blue = pixels[i * pHeader->width + j].blue >> (8-5),
				.green = pixels[i * pHeader->width + j].green >> (8-6),
				.red = pixels[i * pHeader->width + j].red >> (8-5),
			};
			fwrite(&tmp, 1, sizeof(tmp), pFile2);
		}
	}

	printf("Done!\n");
	free_bmp(pBmpFile);
	fclose(pFile2);

	return 0;
}