#pragma once

#include "basic.h"

long int mystrtol(const char *nptr, char **endptr , int base) {
	const char *cur = nptr;
	i32 neg = 0, res = 0;

	//skip space
	while(isspace(*cur)) cur++;

	//sign
	if(*cur == '+') cur++;
	else if(*cur == '-') {
		neg = 1;
		cur++;
	}

	// hexadecimal: skip "0x", "0X"
	if(base == 0 || base == 16) {
		if(*cur == '0' && (*(cur+1) | ' ') == 'x') {
			cur += 2;
			base = 16;
		}
	}

	// octal: skip "0"
	if(base == 0) base = (*cur == '0' ? 8 : 10);

	while(*cur) {
		i32 dig;
		if(isdigit(*cur)) dig = *cur - '0';
		else if(isxdigit(*cur)) dig = (*cur | ' ') - 'a';
		else break;
		if(dig >= base) break;
		if(neg) {
			res = (res < (LONG_MIN+dig)/base ? LONG_MIN : res * base - dig);
		}
		else {
			res = (res > (LONG_MAX-dig)/base ? LONG_MAX : res * base + dig);
		}
		cur++;
	}
	if(endptr) *endptr = (char *) cur;
	return res;
}

char *mystrchr(const char *s, int c) {
    while(s && *s != '\0') {
        if(*s == c) return (char *)s;
        s++;
    }
    return NULL;
}

char *mystrrchr(const char *s, int c) {
    char *ret = NULL;
    while(s && *s != '\0') {
        if(*s == c) ret = (char *)s;
        s++;
    }
    return ret;
}

size_t mystrspn(const char *s, const char *accept) {
    size_t ret = 0;
    while(s && *s != '\0') {
        if(!mystrchr(accept, *s)) return ret;
        s++;
        ret++;
    }
    return ret;
}

size_t mystrcspn(const char *s, const char *reject) {
    size_t ret = 0;
    while(s && *s != '\0') {
        if(mystrchr(reject, *s)) return ret;
        s++;
        ret++;
    }
    return ret;
}

char *mystrpbrk(const char *s, const char *accept) {
    while(s && *s != '\0') {
        if(mystrchr(accept, *s)) return (char *)s;
        s++;
    }
    return NULL;
}
char *mystrstr(const char *haystack , const char *needle) {
    haystack = mystrchr(haystack, *needle);
    while(haystack && *haystack != '\0') {
        const char *s1 = haystack, *s2 = needle;
        while(*s1 != '\0' && *s1 == *s2) {
            s1++;
            s2++;
        }
        if(*s2 == '\0') return (char *)haystack;
        haystack = mystrchr(haystack+1, *needle);
    }
    return NULL;
}

char *mystrtok(char *str, const char *delim) {
    static char *next = NULL;
    char *ret;
    if(!str) ret = next;
    else ret = str;
    ret += mystrspn(ret, delim);
    if(!ret) return NULL;
    
    char *end = ret+mystrcspn(ret, delim);
    if(!end || *end == '\0') {
        next = NULL;
        return ret;
    }
    *end = '\0';
    next = end + 1;
    return ret;
}
