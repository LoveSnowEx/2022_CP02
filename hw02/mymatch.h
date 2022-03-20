#pragma once

#include "basic.h"
#include "mysplit.h"

bool ismatch(const char *pStr, const char *pPattern) {
	if(*pStr == '\0' || *pPattern == '\0') return *pStr == '\0' && *pPattern == '\0';
	if(*pPattern == '?') return ismatch(pStr+1, pPattern+1);
	if(*pPattern == '*') {
		for(i32 i = 0;; i++) {
			if(ismatch(pStr+i, pPattern+1)) return true;
			if(pStr[i] == '\0') return false;
		}
	}
	return *pStr == *pPattern && ismatch(pStr+1, pPattern+1);
}

int mymatch(char ***pppList , const char *pStr, const char *pPattern) {
	if(!pStr) {
		printf("error: null string occured\n");
		return -1;
	}
	if(!pPattern) {
		printf("error: null pattern occured\n");
		return -1;
	}
	i32 matchCnt = 0, splitCnt, len = strlen(pStr);
	char **splitList, **ppList = (char**)calloc(1, len*sizeof(char*));
	mystrsplit(&splitList, &splitCnt, pStr, NULL);
	for(i32 i = 0; i < splitCnt; i++) {
		if(ismatch(splitList[i], pPattern)) ppList[matchCnt++] = splitList[i];
	}
	*pppList = ppList;
	return matchCnt;
}