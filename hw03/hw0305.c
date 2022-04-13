#include "basic.h"
#include "bmp.h"

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

int main() {
	sBmpFile *pBmpFile;
	char imgname[1024], puzzlename[1024];

	//enter image file
	printf("Please enter the image: ");
	while(!fgets_n(imgname, 1024, stdin));
	if(read_bmp(imgname, &pBmpFile) == -1) {
		return 1;
	}	

	sBmpHeader *pHeader = pBmpFile->pHeader, header2 = *pHeader;
	sPixel24 *pixels = pBmpFile->pBitmap;

	//enter puzzle file
	printf("Please enter the puzzle: ");
	while(!fgets_n(puzzlename, 1024, stdin));
	FILE *pFile2 = fopen(puzzlename, "r");
	
	if (pFile2 == NULL) {
		printf("File could not be opened!\n");
		free_bmp(pBmpFile);
		return 1;
	}


	// read puzzle M, N, K
	char buf[1048576], buf2[1048576];
	i32 M, N, K;
	while(!fgets_n(buf, sizeof(buf), pFile2));
	sscanf(buf, "%d %d", &M, &N);
	sprintf(buf2, "%d %d", M, N);
	if(strcmp(buf, buf2) != 0 || M <= 0 || N <= 0) {
		printf("error: invalid m, n\n");
		free_bmp(pBmpFile);
		fclose(pFile2);
		return 1;
	}
	while(!fgets_n(buf, sizeof(buf), pFile2));
	sscanf(buf, "%d", &K);
	sprintf(buf2, "%d", K);
	if(strcmp(buf, buf2) != 0 || K <= 0) {
		printf("error: invalid k\n");
		free_bmp(pBmpFile);
		fclose(pFile2);
		return 1;
	}

	i32 puzzle[N][M], chuncks = N*M;
	i32 chunck_height = (pHeader->height-1)/N+1, chunck_width = (pHeader->width-1)/M+1;
	header2.height = (chunck_height+3)*N+3;
	header2.width = (chunck_width+3)*M+3;
	header2.width = (header2.width+3)/4*4;
	header2.bitmap_size = header2.height * header2.width * sizeof(sPixel24);
	header2.size = header2.bitmap_size + header2.offset;

	// freopen("out.txt", "w", stdout);
	for(i32 i = 0; i < K; i++) {
		// read block[i]
		for(i32 j = 0; j < N; j++) {
			for(i32 k = 0; k < M; k++) {
			}
		}
		
		i32 cnt[chuncks+1], diff[2], diffcnt = 0, diff_j = 0, diff_k = 0;
		memset(cnt, 0, sizeof(cnt));
		memset(diff, 0, sizeof(diff));

		for(i32 j = 0; j < N; j++) {
			fgets_n(buf, sizeof(buf), pFile2);
			*buf2 = '\0';
			char *cur = buf;
			for(i32 k = 0; k < M; k++) {
				i32 x = strtol(cur, &cur, 10);
				strncatf(buf2, "%d%s", sizeof(buf2), x, (k==M-1 ? "" : " "));
				// bound check
				if(x < 0 || x > chuncks) {
					printf("error: invalid puzzle at block %d row %d col %d! out of bound\n", i, j, k);
					return 1;
				}
				// count check
				if(cnt[x]) {
					printf("error: invalid puzzle at block %d row %d col %d! repeat chucks occurred\n", i, j, k);
					return 1;
				}
				// not initial state
				if(i != 0) {
					// diffrent to pre puzzle
					if(x-puzzle[j][k] != 0) {
						// different part too much
						if(diffcnt >= 2) {
							printf("error: invalid puzzle at block %d row %d col %d! diffrent part is too much\n", i, j, k);
							return 1;
						}
						// one of them is zero and another is not
						if(((x==0) ^ (puzzle[j][k]==0)) == 0) {
							printf("error: invalid puzzle at block %d row %d col %d! invalid move\n", i, j, k);
							return 1;
						}
						diff[diffcnt++] = x-puzzle[j][k];
						if(diffcnt == 1) {
							diff_j = j, diff_k = k;
						}
						else {
							diff_j -= j, diff_k -= k;
							// swap too far
							if(abs(diff_j) + abs(diff_k) > 1) {
								printf("error: invalid puzzle at block %d row %d col %d! invalid move\n", i, j, k);
								return 1;
							}
							// swap different chunck
							if(diff[0]+diff[1] != 0) {
								printf("error: invalid puzzle at block %d row %d col %d! invalid move\n", i, j, k);
								return 1;
							}
						}
					}
				}
				puzzle[j][k] = x;
				cnt[x]++;
			}
			if(strcmp(buf, buf2) != 0) {
				printf("error: invalid puzzle at block %d row %d! format error\n", i, j);
				return 1;
			}
		}
		// only one different part
		if(i != 0 && diffcnt < 2) {
			printf("error: invalid puzzle at block %d! invalid move\n", i);
			return 1;
		}

		// create outputfile
		sprintf(buf, "%d.bmp", i);
		FILE *pFile3 = fopen(buf, "wb");
		// write header
		fwrite(&header2, 1, sizeof(sBmpHeader), pFile3);

		// write bitmap		
		for (i32 j = 0; j < header2.height; j++) {
			for (i32 k = 0; k < header2.width; k++) {
				i32 x = k, y = header2.height-1-j;
				// black line
				if(x % (chunck_width+3) < 3 || y % (chunck_height+3) < 3) {
					fwrite(&black, 1, sizeof(sPixel24), pFile3);
					continue;
				}
				i32 block_ver = y/(chunck_height+3);
				i32 block_hor = x/(chunck_width+3);

				// out of chunck
				if(block_ver >= N || block_hor >= M) {
					fwrite(&black, 1, sizeof(sPixel24), pFile3);
					continue;
				}
				i32 block2 = puzzle[block_ver][block_hor]-1;

				// blank chunck
				if(block2 == -1) {
					fwrite(&white, 1, sizeof(sPixel24), pFile3);
					continue;
				}
				i32 block_ver2 = block2/M, block_hor2 = block2%M;
				i32 offset_ver = (block_ver2-block_ver)*(chunck_height+3);
				i32 offset_hor = (block_hor2-block_hor)*(chunck_width+3);
				i32 x2 = x + offset_hor - (block_hor2+1)*3;
				i32 y2 = y + offset_ver - (block_ver2+1)*3;
				if(x2 >= pHeader->width || y2 >= pHeader->height) {
					fwrite(&white, 1, sizeof(sPixel24), pFile3);
					continue;
				}
				// printf("x: %d, y: %d, x2: %d, y2: %d\n", x, y, x2, y2);
				fwrite(&pixels[(pHeader->height-1-y2) * pHeader->width + x2], 1, sizeof(sPixel24), pFile3);				
			}
		}
		fclose(pFile3);
	}
	free_bmp(pBmpFile);
	fclose(pFile2);
}