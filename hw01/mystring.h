#pragma once

#include "basic.h"

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
    next = end +1;
    return ret;
}
