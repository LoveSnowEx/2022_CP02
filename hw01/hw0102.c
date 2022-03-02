#include "basic.h"
#include "color.h"
#define MAXSIZE 8192

#define max(a, b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b;       \
})

#define min(a, b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

int main() {
	char s[MAXSIZE], s1[MAXSIZE] = "", s2[MAXSIZE] = "", keyword[MAXSIZE], newword[MAXSIZE];
	printf("Please enter the string:\n");
	while (!fgets_n(s, MAXSIZE, stdin));
	printf("Please enter the keyword:\n");
	while (!fgets_n(keyword, MAXSIZE, stdin));
	printf("Please enter the new word:\n");
	while (!fgets_n(newword, MAXSIZE, stdin));
	i32 len = strlen(s), kwlen = strlen(keyword), nwlen = strlen(newword);
	for(char *cur = s; cur-s < len;) {
		i32 cnt = strspn(cur, " ,.");
		strncat(s1, cur, cnt);
		strncat(s2, cur, cnt);
		cur += cnt;

		cnt = strcspn(cur, " ,.");
		if(strncasecmp(cur, keyword, max(kwlen, cnt)) == 0) {
			strcat(s1, CLR_GRN);
			strcat(s2, CLR_RED);
			strncat(s1, cur, cnt);
			strncat(s2, newword, nwlen);
			strcat(s1, CLR_RST);
			strcat(s2, CLR_RST);
		}
		else {
			strncat(s1, cur, cnt);
			strncat(s2, cur, cnt);
		}
		cur += cnt;
	}
	printf("Original:\n%s\n", s1);
	printf("New:\n%s\n", s2);
	return 0;
}