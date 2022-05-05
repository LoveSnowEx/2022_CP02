#include "basic.h"

void f(char *c) {
	if(isalpha(*c)) *c -= 'A'-10;
	else if(isdigit(*c)) *c -= '0';
}


int main() {
	char s[BUFSIZ];
	FILE *fp = fopen("out", "wr");
	
	fgets_n(s, BUFSIZ, stdin);
	for(char *cur = s; *cur; ) {
		if(*cur == ' ') {
			++cur;
			continue;
		}
		f(cur);
		f(cur+1);
		unsigned c = ((unsigned char)*cur << 4) + (unsigned char)*(cur+1);
		fwrite(&c, 1, 1, fp);
		cur += 2;
	}
	fclose(fp);
}