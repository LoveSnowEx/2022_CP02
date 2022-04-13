#include "basic.h"

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