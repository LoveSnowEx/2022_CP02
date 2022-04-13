#include "basic.h"

typedef struct _sBibleLine {
	i32 chapter;
	i32 verse;
	char text[512];
	char book_id[8];
} sBibleLine;

void strtojsstr(char *rest, const char *nptr, char **endptr) {
	const char *begin, *end;
	begin = strchr(nptr, '\"')+1;
	end = strchr(begin, '\"');
	size_t len = end-begin;
	strncpy(rest, begin, len);
	rest[len] = '\0';
	if(endptr) *endptr = (char*)end + 1;
}

void strtobibleLine(sBibleLine *pLine, const char *nptr, char **endptr) {
	char buf[512];
	char *cur = (char*) nptr;
	if(*cur == '{') cur++;

	// chapter
	strtojsstr(buf, cur, &cur);
	if(*cur == ':') cur++;
	pLine->chapter =  strtol(cur, &cur, 10);
	if(*cur == ',') cur++;
	
	// verse
	strtojsstr(buf, cur, &cur);
	if(*cur == ':') cur++;
	pLine->verse =  strtol(cur, &cur, 10);
	if(*cur == ',') cur++;

	// text
	strtojsstr(buf, cur, &cur);
	if(*cur == ':') cur++;
	strtojsstr(pLine->text, cur, &cur);
	if(*cur == ',') cur++;

	// translation_id
	strtojsstr(buf, cur, &cur);
	if(*cur == ':') cur++;
	strtojsstr(buf, cur, &cur);
	if(*cur == ',') cur++;

	// book_id
	strtojsstr(buf, cur, &cur);
	if(*cur == ':') cur++;
	strtojsstr(pLine->book_id, cur, &cur);
	if(*cur == ',') cur++;

	// book_name
	strtojsstr(buf, cur, &cur);
	if(*cur == ':') cur++;
	strtojsstr(buf, cur, &cur);

	if(*cur == '}') cur++;
	
	if(endptr) *endptr = (char*)cur;
	
}

void freebible(sBibleLine **ppLine, size_t size) {
	for(size_t i = 0; i < size; i++) {
		free(ppLine[i]);
	}
	free(ppLine);
}