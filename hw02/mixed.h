#include "basic.h"

typedef struct _sMixedNumber {
	i32 numerator;
	u32 denominator;
} sMixedNumber;

void simplified(sMixedNumber *pNumber);

// return -1 if invalid; otherwise , return 0.
int mixed_set( sMixedNumber *pNumber , int32_t a, int32_t b, int32_t c);

// in the form of (a,b,c)
int mixed_print( const sMixedNumber number);

// pNumber = r1 + r2
void mixed_add(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 - r2
void mixed_sub(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 * r2
void mixed_mul(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);

// pNumber = r1 / r2
void mixed_div(sMixedNumber *pNumber , const sMixedNumber r1, const sMixedNumber r2);
