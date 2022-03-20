#include "basic.h"
#include "mymatch.h"

#define MAXSIZE 8192

int main() {
	char s[MAXSIZE], pat[MAXSIZE];
	printf("Please enter the string:\n");
	while (!fgets_n(s, MAXSIZE, stdin));
	printf("Please enter the pattern:\n");
	while (!fgets_n(pat, MAXSIZE, stdin));
	char **res;
	int n = mymatch(&res, s, pat);
	for(int i = 0; i < n; i++) {
		printf("%s\n", res[i]);
	}
	return 0;
}