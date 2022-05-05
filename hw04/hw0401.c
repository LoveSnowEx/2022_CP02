#include "basic.h"
#include "bmp.h"
#include "mmap.h"

i32 to_gray(i32 red, i32 green, i32 blue) { return (red*19595 + green*38469 + blue*7472) >> 16; }

const sPixel24 white = {
	.blue = 255,
	.green = 255,
	.red = 255,
};

const sPixel24 black = {
	.blue = 0,
	.green = 0,
	.red = 0,
};

int main(int argc, char *argv[]) {
	char buf[BUFSIZ/2], filename[BUFSIZ], filename_layer1[BUFSIZ], filename_layer2[BUFSIZ], filename_overlap[BUFSIZ];
	
	// check arg
	if(argc != 2) {
		printf("error: invalid option count");
		return 1;
	}

	// read source file
	strcpy(filename, argv[1]);
	sBmpFile *pBmpFile = read_bmp(filename);
	if(pBmpFile == NULL) {
		return 1;
	}

	sBmpHeader *pHeader = pBmpFile->pHeader;
	sPixel24 (*pBitmap)[pHeader->width] = (sPixel24(*)[pHeader->width])pBmpFile->pBitmap;	
	pHeader->width *= 2;
	pHeader->height *= 2;
	pHeader->bitmap_size *= 4;
	pHeader->size = pHeader->offset + pHeader->bitmap_size;

	// build output filename
	sscanf(filename, "%s.bmp", buf);
	snprintf(filename_layer1, sizeof(filename_layer1), "%s_layer1.bmp", buf);
	snprintf(filename_layer2, sizeof(filename_layer2), "%s_layer2.bmp", buf);
	snprintf(filename_overlap, sizeof(filename_overlap), "%s_overlap.bmp", buf);

	// mmap output files
	i32 fd[3];
	// struct stat dstsb[3];
	char *pLayer1, *pLayer2, *pOverlap;

	fd[0] = open(filename_layer1, O_RDWR|O_CREAT);
	fd[1] = open(filename_layer2, O_RDWR|O_CREAT);
	fd[2] = open(filename_overlap, O_RDWR|O_CREAT);
	if(fd[0] == -1 || fd[1] == -1 || fd[2] == -1) {
		printf("error: failed to open output file descripteor\n");
		return 1;
	}
	
	ftruncate(fd[0], pHeader->size);
	ftruncate(fd[1], pHeader->size);
	ftruncate(fd[2], pHeader->size);


	pLayer1 = mmap(NULL, pHeader->size, PROT_READ|PROT_WRITE, MAP_SHARED, fd[0], 0);
	pLayer2 = mmap(NULL, pHeader->size, PROT_READ|PROT_WRITE, MAP_SHARED, fd[1], 0);
	pOverlap = mmap(NULL, pHeader->size, PROT_READ|PROT_WRITE, MAP_SHARED, fd[2], 0);

	if(pLayer1 == NULL || pLayer2 == NULL || pOverlap == NULL) {
		printf("error: failed to open output files\n");
		return 1;
	}

	// copy header
	
	memcpy(pLayer1, pHeader, sizeof(sBmpHeader));
	memcpy(pLayer2, pHeader, sizeof(sBmpHeader));
	memcpy(pOverlap, pHeader, sizeof(sBmpHeader));
	

	sPixel24 (*pixels[3])[pHeader->width];
	pixels[0] = (sPixel24(*)[pHeader->width])(pLayer1 + pHeader->offset);
	pixels[1] = (sPixel24(*)[pHeader->width])(pLayer2 + pHeader->offset);
	pixels[2] = (sPixel24(*)[pHeader->width])(pOverlap + pHeader->offset);

	srand(time(NULL));
	for(i32 i = 0; i < pHeader->height/2; ++i) {
		for(i32 j = 0; j < pHeader->width/2; ++j) {
			i32 red = pBitmap[i][j].red, green = pBitmap[i][j].green, blue = pBitmap[i][j].blue;
			i32 gray = to_gray(red, green, blue);
			
			i32 rnd = rand()%2;
			if(gray < (1<<7)) {  // draw black
				for(i32 k = i*2; k < i*2+2; ++k) {
					for(i32 l = j*2; l < j*2+2; ++l) {
						pixels[0][k][l] = (k+l)%2 == rnd ? white : black;
						pixels[1][k][l] = (k+l)%2 == rnd ? black : white;
						pixels[2][k][l] = black;
					}	
				}
			}
			else {  // draw white
				for(i32 k = i*2; k < i*2+2; ++k) {
					for(i32 l = j*2; l < j*2+2; ++l) {
						pixels[0][k][l] = pixels[1][k][l] = pixels[2][k][l] = (k+l)%2 == rnd ? white : black;
					}
				}
			}
		}
	}
	
	munmap(pLayer1, pHeader->size);
	munmap(pLayer2, pHeader->size);
	munmap(pOverlap, pHeader->size);
	close(fd[0]);
	close(fd[1]);
	close(fd[2]);
	free_bmp(pBmpFile);
	return 0;
}