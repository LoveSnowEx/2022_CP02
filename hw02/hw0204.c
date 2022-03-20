#include "basic.h"
#include "mixed.h"
#define MAXSIZE 4096

jmp_buf env_buffer;
char warning_detail[128];

int main() {
	char s[MAXSIZE], buf[MAXSIZE];
	printf("Q: ");
	while(!fgets_n(s, MAXSIZE, stdin));

	int val = setjmp(env_buffer);
	switch (val) {
	case 0:
		break;
	default:
		printf("%s: %s\n", warning_massage[val], warning_detail);
		exit(0);
	}

	sMixedNumber res;
	mixed_eval(s, 0, &res);
	mixed_print(buf, res);
	printf("A: %s\n", buf);

	return 0;
}