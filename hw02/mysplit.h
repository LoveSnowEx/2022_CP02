#pragma once

#include "basic.h"

int mystrsplit(char ***pppList , int *pCounter , const char *pStr, const char *
pSeparator) {
	if(!pStr) {
		printf("error: null string occur\n");
		return -1;
	}
	char *cpy = strdup(pStr);
	i32 cnt = 0, len = strlen(pStr), seplen;
	char **ppList = (char**)malloc(len*sizeof(char*));

	if(!pSeparator) {
		for(i32 i = 0; i < len;) {
			i += strspn(cpy+i, " \t\n");
			if(!cpy[i]) break;
			// printf("%d: %s\n", cnt, cpy+i);
			ppList[cnt++] = cpy+i;
			i += strcspn(cpy+i, " \t\n");
			cpy[i++] = '\0';
		}
	}
	else {
		if(strcmp(pSeparator, "") == 0) {
			printf("error: empty separator\n");
			return -1;
		}
		seplen = strlen(pSeparator);
		for(i32 i = 0; i <= len;) {
			// printf("%d: %s\n", cnt, cpy+i);
			ppList[cnt++] = cpy+i;
			char *nxt = strstr(cpy+i, pSeparator);
			if(!nxt) break;
			i = nxt-cpy;
			cpy[i] = '\0';
			i += seplen;
		}
	}
	*pppList = ppList;
	*pCounter = cnt;
	return 0;
}
