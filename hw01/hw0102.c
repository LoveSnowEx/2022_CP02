#include "basic.h"
#include "color.h"
#define MAXSIZE 8192

int main() {
	char s[MAXSIZE], res[MAXSIZE], keyword[MAXSIZE], newword[MAXSIZE];
	printf("Please enter the string:\n");
	while (!fgets_n(s, MAXSIZE, stdin));
	printf("Please enter the keyword:\n");
	while (!fgets_n(keyword, MAXSIZE, stdin));
	printf("Please enter the new word:\n");
	while (!fgets_n(newword, MAXSIZE, stdin));
	i32 len = strlen(s), kwlen = strlen(keyword);
	char cpy = strdup(s);
	for(char *cur = cpy; cur-cpy < len; ) {
		cur = strstr(cur, keyword);
	}
	printf("%s\n", res);
	return 0;
}