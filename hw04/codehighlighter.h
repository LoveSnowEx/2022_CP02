#include "basic.h"
#include "color.h"

typedef struct _sKeywordTable {
	char lang[16];
	char keywords[256][32];
	size_t keywordcnt;
} sKeywordsTable;

void init_keywordlist(const char *filename);

void printhighlight(const char *text, const size_t size, bool showlinenum, bool showcolor, const char *lang);