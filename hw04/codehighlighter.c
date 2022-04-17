#include "codehighlighter.h"


size_t langcnt = 0;
static sKeywordsTable keywordlist[32];

void init_keywordlist(const char *filename) {
	char buf[BUFSIZ];
	FILE *pFile = fopen(filename, "r");
	if(pFile == NULL) return;
	while(true) {
		while(!feof(pFile) && !fgets_n(buf, sizeof(buf), pFile));
		if(feof(pFile)) break;
		// printf("lang: %s\n", buf);
		strncpy(keywordlist[langcnt].lang, buf, sizeof(keywordlist[langcnt].lang));

		while(!fgets_n(buf, sizeof(buf), pFile));
		sscanf(buf, "%lu", &keywordlist[langcnt].keywordcnt);
		// printf("kwcnt: %lu\n", keywordlist[langcnt].keywordcnt);

		for(size_t i = 0; i < keywordlist[langcnt].keywordcnt; ++i) {
			while(!fgets_n(keywordlist[langcnt].keywords[i], sizeof(keywordlist[langcnt].keywords[i]), pFile));
			// printf("%s, ", keywordlist[langcnt].keywords[i]);
		}
		// printf("\n");
		langcnt++;
	}
	// fprintf(stderr, "%lu\n", langcnt);
}

void printhighlight(const char *text, const size_t size, bool showlinenum, bool showcolor, const char *lang) {
	// setup lang
	i32 langidx = -1;
	if(showcolor) {
		for(size_t i = 0; i < langcnt; ++i) {
			if(strcasecmp(lang, keywordlist[i].lang) == 0) {
				langidx = i;
				break;
			}
		}
	}
	bool isnewline = true;
	size_t linenum = 0, i = 0;
	while(i < size) {
		if(showlinenum) {
			if(isnewline) {
				printf("%4lu|", ++linenum);
				isnewline = false;
			}
			if(text[i] == '\n') {
				isnewline = true;
			}
		}

		if(!showcolor) {
			if(text[i] == '\t') printf("    ");
			else printf("%c", text[i]);
			++i;
			continue;
		}

		static char sepcial[] = "_@#.";
		// skip not alpha and not special
		if(!isalpha(text[i]) && !strchr(sepcial, text[i])) {
			if(text[i] == '\t') printf("    ");
			else printf("%c", text[i]);
			++i;
			continue;
		}

		bool match = false;
		for(size_t j = 0; j < keywordlist[langidx].keywordcnt; ++j) {
			char *kw = keywordlist[langidx].keywords[j];
			size_t kwlen = strlen(kw);
			char end = text[min(i+kwlen, size-1)];
			if(isalnum(end) || strchr(sepcial, text[i])) continue;
			if(strncmp(text+i, kw, kwlen) == 0) {
				printf(CLR_LBL"%s"CLR_RST, kw);
				i += kwlen;
				match = true;
				break;
			}
		}
		if(match) continue;

		while(isalnum(text[i]) || strchr(sepcial, text[i])) {
			if(text[i] == '\t') printf("    ");
			else printf("%c", text[i]);
			++i;
		}
	}
	printf("\n");
}