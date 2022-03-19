#include "mixed.h"

i32 abs(i32 n) {
	if(n < 0) return -n;
	return n;
}

i32 gcd(i32 a, i32 b) {
	if(a < b) return gcd(b, a);
	i32 tmp;
	while(a % b) {
		tmp = a%b;
		a = b;
		b = tmp;
	}
	return b;
}

void simplified(sMixedNumber *pNumber) {
	if(!pNumber->numerator || !pNumber->denominator) {
		pNumber->numerator = pNumber->denominator = 0;
		return;
	}
	i32 g = gcd(abs(pNumber->numerator), pNumber->denominator);
	pNumber->numerator /= g;
	pNumber->denominator /= g;
}

int mixed_set(sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c) {
	if(c <= 0) return -1; //ivalid denominator
	if(a < 0) pNumber->numerator = a*c-b;
	else pNumber->numerator = a*c+b;
	pNumber->denominator = c;
	return 0;
}

int mixed_print(const sMixedNumber number) {
	i32 a, b = number.numerator, c = number.denominator;
	if(!c) {
		printf("(0, 0, 0)\n");
		return 0;
	}
	if(b < 0) {
		a = (-b)/c*(-1);
		b = (-b)%c;
		if(a == 0) b = -b;
	}
	else {
		a = b/c;
		b = b%c;
	}
	printf("(%d, %d, %d)\n", a, b, c);
	return 0;
}

void mixed_add(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2) {
	pNumber->numerator = r1.numerator*r2.denominator + r2.numerator*r1.denominator;
	pNumber->denominator = r1.denominator*r2.denominator;
	simplified(pNumber);
}

void mixed_sub(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2) {
	pNumber->numerator = r1.numerator*r2.denominator - r2.numerator*r1.denominator;
	pNumber->denominator = r1.denominator*r2.denominator;
	simplified(pNumber);
}

void mixed_mul(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2) {
	pNumber->numerator = r1.numerator*r2.numerator;
	pNumber->denominator = r1.denominator*r2.denominator;
	simplified(pNumber);
}

void mixed_div(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2) {
	pNumber->numerator = r1.numerator*r2.denominator;
	pNumber->denominator = r1.denominator*r2.numerator;
	simplified(pNumber);
}
