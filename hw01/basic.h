#pragma once

#include <assert.h>
#include <complex.h>
#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <tgmath.h>
#include <threads.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>

typedef int32_t i32;
typedef int64_t i64;
typedef uint32_t u32;
typedef uint64_t u64;

size_t fgets_n(char *str, int size, FILE *stream) {
    char *ptr = fgets(str, size, stream);
    if(!ptr) return 0;
    char *end = strchr(ptr, '\n');
    if(end) *end = '\0';
    return strlen(ptr);
}

void strncatf(char *dest, char *format, size_t n, ...) {
	char buf[n];
    va_list args;
    va_start(args, n);
	vsprintf(buf, format, args);
	strncat(dest, buf, n);
	va_end(args);
}

