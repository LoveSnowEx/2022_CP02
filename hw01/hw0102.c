#include "basic.h"
#include "color.h"
#define MAXSIZE 8192

int main() {
	char s[MAXSIZE], s1[MAXSIZE] = "", s2[MAXSIZE] = "", keyword[MAXSIZE], newword[MAXSIZE];
	printf("Please enter the string:\n");
	while (!fgets_n(s, MAXSIZE, stdin));
	printf("Please enter the keyword:\n");
	while (!fgets_n(keyword, MAXSIZE, stdin));
	printf("Please enter the new word:\n");
	while (!fgets_n(newword, MAXSIZE, stdin));
	i32 len = strlen(s), kwlen = strlen(keyword);
	// print("Original:\n");
	char cpy = strdup(s);
	for(char *cur = cpy; cur-s < len;) {
		char *nxt = strcasestr(cur, keyword);
		if(!nxt || (nxt+kwlen <= len && (nxt[kwlen] != ',' || nxt[kwlen] != ' '))) {
			strncat(s1, cur, nxt+kwlen-cur);
			strncat(s2, cur, nxt+kwlen-cur);
			cur = nxt+kwlen;
		}
		else {
			strncatf(s1, CLR_RED"%.*s", cur, nxt+kwlen-cur);
			strncatf(s2, CLR_GRN"%.*s", cur, nxt+kwlen-cur);
		}
	}
	printf("%s\n", res);
	return 0;
}