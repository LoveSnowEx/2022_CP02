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
	char **ppList = (char**)calloc(1, len*sizeof(char*));

	if(!pSeparator) {
		for(i32 i = 0; i < len;) {
			i += strspn(cpy+i, " \t\n");
			if(!cpy[i]) break;
			// printf("%d: %s\n", cnt, cpy+i);
			i32 nxt = i + strcspn(cpy+i, " \t\n");
			cpy[nxt++] = '\0';
			ppList[cnt++] = strdup(cpy+i);
			i = nxt;
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
			char *nxt = strstr(cpy+i, pSeparator);
			if(nxt) *nxt = '\0';
			ppList[cnt++] = strdup(cpy+i);
			if(!nxt) break;
			i = nxt-cpy+seplen;
		}
	}
	free(cpy);
	*pppList = ppList;
	*pCounter = cnt;
	return 0;
}
