#include "mixed.h"

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

int mixed_simplified(sMixedNumber *pNumber) {
	if(pNumber->denominator == 0) {  // divide by zero
		pNumber->numerator = 1;
		return -1;
	}
	if(pNumber->numerator == 0) {  // 0/x -> 0/1
		pNumber->denominator = 1;
		return 0;
	}
	if(pNumber->denominator < 0) {  // a/b * -1/-1
		pNumber->numerator *= -1;
		pNumber->denominator *= -1;
	}
	i32 g = gcd(abs(pNumber->numerator), pNumber->denominator);
	pNumber->numerator /= g;
	pNumber->denominator /= g;
	return 0;
}

void mixed_read(const char *nptr, char **endptr, sMixedNumber *mixed) {
	char *cur = (char *)nptr, buf[128];
	i32 a = 0, b = 0, c = 1;

	// read a
	a = strtol(cur, &cur, 10);

	// read b, c
	size_t len = 0;
	if(*cur == '\\') {
		sscanf(cur, "\\frac{%d}{%d}", &b, &c);
		sprintf(buf, "\\frac{%d}{%d}", b, c);
		if(strncmp(cur, buf, (len = strlen(buf))) != 0) {
			sprintf(warning_detail, "%s\n",cur);
			longjmp(env_buffer, MIXED_READ_FAIL);
		}
	}
	else if(strcspn(cur, "+-*/") != 0) {
		sprintf(warning_detail, "%s\n",cur);
		longjmp(env_buffer, MIXED_READ_FAIL);
	}
	mixed_set(mixed, a, b, c);
	cur += len;
	if(endptr) *endptr = cur;
}

void mixed_eval(const char *nptr, char **endptr, sMixedNumber *mixed) {
	char *cur = (char *)nptr;
	char operator = '+';  // dafault operator
	sMixedNumber tmp1, tmp2;

	// set default = 0
	*mixed = ZERO;

	// skip space and tab
	cur += strspn(cur, " \t");
	// read first num
	mixed_read(cur, &cur, &tmp1);
	
	while(*(cur += strspn(cur, " \t"))) {
		if(*cur == '+' || *cur == '-') {
			// mixed += tmp1
			if(operator == '+') mixed_add(mixed, *mixed, tmp1);
			// mixed -= tmp1
			else mixed_sub(mixed, *mixed, tmp1);

			operator = *cur++;
			// read tmp1
			mixed_read(cur, &cur, &tmp1);
		}
		else if(*cur == '*' || *cur == '/') {
			if(*cur++ == '*') {
				// read tmp2
				mixed_read(cur, &cur, &tmp2);
				// tmp1 *= tmp2
				mixed_mul(&tmp1, tmp1, tmp2);
			}
			else {
				// read tmp2
				mixed_read(cur, &cur, &tmp2);
				// tmp1 /= tmp2
				mixed_div(&tmp1, tmp1, tmp2);
			}
		}
		else {
			break;
		}
	}
	// final clean up '+' and '-' operator
	if(operator == '+') mixed_add(mixed, *mixed, tmp1);
	else mixed_sub(mixed, *mixed, tmp1);
	if(endptr) *endptr = cur;
}

void mixed_set(sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c) {	
	// numerator
	if(a < 0) pNumber->numerator = a*c-b;
	else pNumber->numerator = a*c+b;

	// denominator
	pNumber->denominator = c;

	if(mixed_simplified(pNumber) == -1) {
		if(a != 0 || b == 0) sprintf(warning_detail, "%d", a);
		if(b) strncatf(warning_detail, "\\frac{%d}{%d}", 128, b, c);
		longjmp(env_buffer, MIXED_SET_FAIL);
	}
}

void mixed_print(char *dest, const sMixedNumber number) {
	i32 a, b = number.numerator, c = number.denominator;
	if(!c) {
		longjmp(env_buffer, 1);
		return;
	}

	if(b < 0) {
		a = -(abs(b)/c);
		b = abs(b)%c;
		if(a == 0) b = -b;
	}
	else {
		a = b/c;
		b = b%c;
	}
	// if a != 0: print; if a == 0 && b == 0: print
	if(a != 0 || b == 0) sprintf(dest, "%d", a);
	if(b) strncatf(dest, "\\frac{%d}{%d}", 128, b, c);
}

void mixed_add(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2) {
	pNumber->numerator = r1.numerator*r2.denominator + r2.numerator*r1.denominator;
	pNumber->denominator = r1.denominator*r2.denominator;
	if(mixed_simplified(pNumber) == -1) {
		char buf1[128], buf2[128];
		mixed_print(buf1, r1);
		mixed_print(buf2, r2);
		strncatf(buf1, "+%s", 128, buf2);
		sprintf(warning_detail, "%s", buf1);
		longjmp(env_buffer, MIXED_ADD_FAIL);
	}
}

void mixed_sub(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2) {
	pNumber->numerator = r1.numerator*r2.denominator - r2.numerator*r1.denominator;
	pNumber->denominator = r1.denominator*r2.denominator;
	if(mixed_simplified(pNumber) == -1) {
		char buf1[128], buf2[128];
		mixed_print(buf1, r1);
		mixed_print(buf2, r2);
		strncatf(buf1, "-%s", 128, buf2);
		sprintf(warning_detail, "%s", buf1);
		longjmp(env_buffer, MIXED_SUB_FAIL);
	}
}

void mixed_mul(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2) {
	pNumber->numerator = r1.numerator*r2.numerator;
	pNumber->denominator = r1.denominator*r2.denominator;
	if(mixed_simplified(pNumber) == -1) {
		char buf1[128], buf2[128];
		mixed_print(buf1, r1);
		mixed_print(buf2, r2);
		strncatf(buf1, "*%s", 128, buf2);
		sprintf(warning_detail, "%s", buf1);
		longjmp(env_buffer, MIXED_MUL_FAIL);
	}
}

void mixed_div(sMixedNumber *pNumber, const sMixedNumber r1, const sMixedNumber r2) {
	pNumber->numerator = r1.numerator*r2.denominator;
	pNumber->denominator = r1.denominator*r2.numerator;
	if(mixed_simplified(pNumber) == -1) {
		char buf1[128], buf2[128];
		mixed_print(buf1, r1);
		mixed_print(buf2, r2);
		strncatf(buf1, "/%s", 128, buf2);
		sprintf(warning_detail, "%s", buf1);
		longjmp(env_buffer, MIXED_DIV_FAIL);
	}
}
