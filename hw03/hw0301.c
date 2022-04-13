#include "basic.h"
#include "hw0301.h"

#define BUFSIZE 8192
#define RESSIZE (1<<24)

int main() {
	char target[BUFSIZE], buf[BUFSIZE], *res = calloc(1, RESSIZE);
	sBibleLine *pLine = calloc(1, sizeof(sBibleLine));
	size_t foundCnt = 0;


	FILE *pFile = fopen("bible.txt", "r");

	if(pFile == NULL) {
		printf("error: open file fail.\n");
		return 1;
	}

	printf("Please enter the search target: ");
	while (!fgets_n(target, BUFSIZE, stdin));

	while(fgets_n(buf, sizeof(buf), pFile)) {
		strtobibleLine(pLine, buf, NULL);
		if(strcasestr(pLine->text, target) != NULL) {
			strncatf(res, "%d. %s %d:%d %s\n", 512, foundCnt+1, pLine->book_id, pLine->chapter, pLine->verse, pLine->text);
			foundCnt++;
		}
	}
	
	printf("Found %ld item%s\n%s", foundCnt, (foundCnt > 1 ? "(s)":""), res);
	

	fclose(pFile);
	free(pLine);
	free(res);
	return 0;
}