#include "basic.h"
#include <iconv.h>

size_t utf8_len(const char *ptr) {
	const unsigned char *p = (const unsigned char *)ptr;
	if(p[0] <= 0x7F) return 1;  // p[0] is ASCII
	if(!(p[1] >= 0x80 && p[1] <= 0xBF)) return 0;  // p[1] valid
	if(p[0] >= 0xC0 && p[0] <= 0xDF) return 2;
	if(!(p[2] >= 0x80 && p[2] <= 0xBF)) return 0;  // p[2] valid
	if(p[0] >= 0xE0 && p[0] <= 0xEF) return 3;
	if(!(p[3] >= 0x80 && p[3] <= 0xBF)) return 0;  // p[3] valid
	if(p[0] >= 0xF0 && p[0] <= 0xF7) return 4;
	return 0;
}

bool isbig5(const char *p) {
	unsigned char a = p[0], b = p[1];
	return (((a>=0xA0 && a<=0xC6) || (a>=0xC9 && a<=0xF9))
				&& ((b>=0x40 && b<=0x7E) || (b>=0xA1 && b<=0xFE)));
}

void szhprint(char *dest, const char *src, char **endptr) {
	if(!src) return;
	iconv_t cd;
	size_t in_s, out_s, len;
	char *in_ptr = (char*)src, *out_ptr = dest;
	if((cd = iconv_open("UTF8", "BIG-5")) == (iconv_t)-1) {
		fprintf(stderr, "error: opening iconv\n");
		exit(1);
	}
	while(*in_ptr) {
		// printf("in_ptr: %lu, c: %u(%hhx)\n", (unsigned long)in_ptr, (unsigned char)*in_ptr, (unsigned char)*in_ptr);
		// printf("out_ptr: %lu, c: %u(%hhx)\n", (unsigned long)out_ptr, (unsigned char)*out_ptr, (unsigned char)*out_ptr);
		if(isbig5(in_ptr)) {
			// printf("big-5\n");
			in_s = 2, out_s = 4;
			if(iconv(cd, &in_ptr, &in_s, &out_ptr, &out_s) == (size_t)-1) {
				fprintf(stderr, "errno: %s\n", strerror(errno));
			}
			continue;
		}
		if((len = utf8_len(in_ptr)) != 0) {
			// printf("utf-8: %ld\n", len);
			strncpy(out_ptr, in_ptr, len);
			in_ptr += len;
			out_ptr += len;
			continue;
		}
		*out_ptr++ = *in_ptr++;
	}
	iconv_close(cd);
	*out_ptr = '\0';
	if(endptr) *endptr = (char*)in_ptr;
}

// void zhprint(const char *ptr, char **endptr) {
// 	if(!ptr) return;
// 	char buf[BUFSIZ];
// 	iconv_t cd;
// 	size_t in_s, out_s, len;
// 	char *in_ptr, *out_ptr;
// 	if((cd = iconv_open("UTF8", "BIG-5")) == (iconv_t)-1) {
// 		fprintf(stderr, "error: opening iconv\n");
// 		exit(1);
// 	}
// 	while(*ptr) {
// 		// printf("ptr: %lu, c: %u\n", (unsigned long)ptr, (unsigned char)*ptr);
// 		if(isbig5(ptr)) {
// 			// printf("big-5\n");
// 			in_ptr = (char *)ptr, out_ptr = buf;
// 			in_s = 2, out_s = 4;
// 			if(iconv(cd, &in_ptr, &in_s, &out_ptr, &out_s) == (size_t)-1) {
// 				fprintf(stderr, "errno: %s\n", strerror(errno));
// 			}
// 			ptr = in_ptr;
// 			*out_ptr = '\0';
// 			printf("%s", buf);

// 			continue;
// 		}
// 		if(len = utf8_len(ptr)) {
// 			// printf("utf-8: %ld\n", len);
// 			fwrite(ptr, 1, len, stdout);
// 			ptr += len;
// 			continue;
// 		}
// 		printf("%c", *ptr++);
// 		ptr++;
// 	}
// 	iconv_close(cd);
// 	if(endptr) *endptr = (char*)ptr;
// }

