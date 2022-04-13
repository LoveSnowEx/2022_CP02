#include "basic.h"
#include "bmp.h"

const sPixel24 white = {
	.blue = 255,
	.green = 255,
	.red = 255,
};

int main() {
	sBmpFile *pBmpFile;
	char buf[1024], buf2[1024], infilename[1024], outfilename[1024];
	
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

	//enter center pos
	int center_x, center_y;
	printf("Please enter the center: ");
	while(!fgets_n(buf, 1024, stdin));
	sscanf(buf, "(%d,%d)", &center_x, &center_y);
	sprintf(buf2, "(%d,%d)", center_x, center_y);
	if(strcmp(buf, buf2) != 0) {
		printf("error: invalid center input.\n");
		return 1;
	}

	//enter radius
	int radius;
	printf("Please enter the radius: ");
	while(!fgets_n(buf, 1024, stdin));
	sscanf(buf, "%d", &radius);
	sprintf(buf2, "%d", radius);
	if(strcmp(buf, buf2) != 0 || radius < 0) {
		printf("error: invalid radius input.\n");
		return 1;
	}

	// write header
	sBmpHeader *pHeader = pBmpFile->pHeader;
	fwrite(pHeader, 1, sizeof(sBmpHeader), pFile2);

	
	// printf("============header=============\n");
	// print_bmp_header(&header);
	// printf("============header2=============\n");
	// print_bmp_header(&header2);
	
	sPixel24 *pixels = pBmpFile->pBitmap;
	for (i32 i = 0; i < pHeader->height; i++) {
		for (i32 j = 0; j < pHeader->width; j++) {
			i32 x = j, y = pHeader->height-1-i;
			if ((x-center_x)*(x-center_x)+(y-center_y)*(y-center_y) <= radius*radius) {
				fwrite(&pixels[i * pHeader->width + j], 1, sizeof(sPixel24), pFile2);
			}
			else {
				fwrite(&white, 1, sizeof(sPixel24), pFile2);
			}
		}
	}
	
	free_bmp(pBmpFile);
	fclose(pFile2);

	printf("Done.\n");

	return 0;
}