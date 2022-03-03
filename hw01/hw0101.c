#include "basic.h"
#define MAXSIZE 8192

const u32 lookup[16] = {
	0b0000, 0b1000, 0b0100, 0b1100, 0b0010, 0b1010, 0b0110, 0b1110,
	0b0001, 0b1001, 0b0101, 0b1101, 0b0011, 0b1011, 0b0111, 0b1111
};

i32 hex2int(char c) {
	if(isdigit(c)) return c-'0';
	if(isalpha(c)) return 10 + ((c | ' ') -'a');
	return -1;
}

int main() {
	char s[MAXSIZE], res[MAXSIZE];
	printf("Please enter the hex string: ");
	while (!fgets_n(s, MAXSIZE, stdin));
	i32 len = strlen(s);

	if(len%2) {
		printf("error: invalid string length\n");
		return 1;
	}

	for(i32 i = 0; i < len; i += 2) {
		i32 n1 = hex2int(s[i]), n2 = hex2int(s[i+1]);
		if(n1 == -1 || n2 == -1) {
			printf("error: invalid hex: %c%c\n", s[i], s[i+1]);
			return 1;
		}
		char c = (lookup[n2] << 4) + lookup[n1];
		if((!c) ^ (i == len-2)) {
			printf("error: '00' is not in the end\n");
			return 1;
		}
		if(c && !isprint(c)) {
			printf("error: unprintable character from hex: %c%c\n", s[i], s[i+1]);
			return 1;
		}
		res[i/2] = c;
	}
	printf("%s\n", res);
	return 0;
}