#include "basic.h"

u32 getDoubleBit(double d, i32 n) {
	i32 reverse = 63-n;
	u64 *byte = (u64 *)&d;
	return *byte >> reverse & 1;
}

int main() {
	char s[1024];
	double d;
	printf("Please enter a floating -point number (double precision): ");
	while(!fgets_n(s, 1024, stdin));
	sscanf(s, "%lf", &d);

	u32 i = 0, sign = 0, exp = 0, bit;
	char frac[1000] = "1";

	printf("Sign: ");
	while(i < 1) {
		bit = getDoubleBit(d, i);
		sign = bit;
		printf("%u", bit);
		i++;
	}

	printf("\nExponent: ");
	while(i < 12) {
		bit = getDoubleBit(d, i);
		exp = exp*2 + bit;
		printf("%u", bit);
		i++;
	}

	printf("\nFraction: ");
	while(i < 64) {
		bit = getDoubleBit(d, i);
		if(bit) {
			strncatf(frac, " + 2^-%u", 10, i-11);
		}
		printf("%u", bit);
		i++;
	}

	printf("\n%lg = (-1)^%u * (%s) * 2^(%u-1023)\n", d, sign, frac, exp);
	return 0;
}