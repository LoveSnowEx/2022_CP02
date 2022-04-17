#include <popt.h>
#include "basic.h"
#include "mmap.h"
#include "codehighlighter.h"

int main(int argc, char *argv[]) {
	bool showlinenum = false, showcolor = false;
	char opt, *lang = NULL;
	const char *filename;

	init_keywordlist("keywords.txt");

	struct poptOption optionsTable[] = {
		{"linenum", 'n', POPT_ARG_NONE, NULL, 'n',
			"Display the line number before each line", "show line num"},
		{"color", 'c', POPT_ARG_NONE, NULL, 'c',
			"Display the code and make each keyword colorful", "colorful code"},
		{"language", 'l', POPT_ARG_STRING, &lang, 0, 
			"Setup programming language", "set lang"},
		POPT_AUTOHELP
		POPT_TABLEEND
	};
	poptContext optCon = poptGetContext(NULL, argc, (const char **)argv, optionsTable, 0);
	poptSetOtherOptionHelp(optCon, "<filename>");

	// check arg
	if(argc < 2) {
		poptPrintUsage(optCon, stderr, 0);
		poptFreeContext(optCon);
		return 1;
	}

	while((opt = poptGetNextOpt(optCon)) > 0) {
		switch(opt) {
		case 'n':
			showlinenum = true;
			break;
		case 'c':
			showcolor = true;
			break;
		}
	}

	filename = poptGetArg(optCon);
	printf(CLR_WHT"filename: %s\n"CLR_RST, filename);

	sMmap *mmp = new_mmap(filename, "r", MAP_SHARED);
	if(mmp == NULL) {
		poptFreeContext(optCon);
		if(lang) free(lang);
		return 1;
	}
	
	if(showcolor && lang == NULL) lang = strdup("C");

	size_t size = mmp->sb.st_size;
	printhighlight(mmp->addr, size, showlinenum, showcolor, lang);

	free_mmap(mmp);
	poptFreeContext(optCon);
	if(lang) free(lang);
	return 0;
}