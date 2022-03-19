#pragma once
#include "basic.h"

enum {
	MIX_NO_FAIL = 0,
	MIXED_READ_FAIL,
	MIXED_SET_FAIL,
	MIXED_PRINT_FAIL,
	MIXED_ADD_FAIL,
	MIXED_SUB_FAIL,
	MIXED_MUL_FAIL,
	MIXED_DIV_FAIL,
};

extern jmp_buf env_buffer;
extern char warning_detail[];
static const char warning_massage[16][64] = {
	"",
	"error: fail to read mixed",
	"error: fail to set mixed",
	"error: fail to print mixed",
	"error: fail to add mixed",
	"error: fail to sub mixed",
	"error: fail to mul mixed",
	"error: fail to div mixed",
};

typedef struct _sMixedNumber {
	i32 numerator;
	i32 denominator;
} sMixedNumber;

static const sMixedNumber ZERO = {0, 1};
static const sMixedNumber ONE = {1, 1};

int simplified(sMixedNumber *pNumber);

void mixed_read(const char *nptr, char **endptr, sMixedNumber *mixed);

void mixed_eval(const char *nptr, char **endptr, sMixedNumber *mixed);

void mixed_set(sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c);

void mixed_print(char *dest, const sMixedNumber number);

// pNumber = r1 + r2
void mixed_add(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 - r2
void mixed_sub(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 * r2
void mixed_mul(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 / r2
void mixed_div(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
