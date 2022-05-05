#include <popt.h>
#include "basic.h"
#include "mmap.h"

int main(int argc, char *argv[]) {
	char *inputname = NULL, *outputname = NULL, *structname = NULL, buf[BUFSIZ];

	struct poptOption optionsTable[] = {
		{"input", 'i', POPT_ARG_STRING, &inputname, 0,
			"Display the line number before each line", "show line num"},
		{"structname", 'n', POPT_ARG_STRING, &structname, 0,
			"Display the code and make each keyword colorful", "colorful code"},
		{"ouput", 'p', POPT_ARG_STRING, &outputname, 0, 
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
	
	sMmap *mmp_in = new_mmap(inputname, MAP_SHARED);

	sprintf(buf, "%s.h", outputname);
	char *tmp = outputname;
	outputname = strdup(buf);
	free(tmp);
	sMmap *mmp_out = new_mmap(outputname, MAP_SHARED);


	if(mmp_in == NULL || mmp_out == NULL) {
		poptFreeContext(optCon);
		if(inputname) free(inputname);
		if(outputname) free(outputname);
		if(structname) free(structname);
		return 1;
	}
	
	if(outputname == NULL) outputname = strdup("test.h");
	if(structname == NULL) structname = strdup("Test");



	size_t size = mmp_in->sb.st_size;
	size_t varcnt = 0;
	i32 vartype[1024];
	char varname[1024][64];
	
	char *cur = mmp_in->addr, buf[1024];
	while(cur = strchr(cur, '|')) {
		cur++;
		char *end = strchr(cur, '\n');
		memcpy(buf, cur, end-cur);
		while(cur != end) {
			i32 len = strcspn(cur, "|");
			strncpy(varname, cur, len);
			vartype[varcnt] = len;
			++varcnt;
		}
		cur = end+1;
	}

	resize_mmap()
	cur = mmp_out->addr;


	free_mmap(mmp_in);
	poptFreeContext(optCon);
	if(inputname) free(inputname);
	if(outputname) free(outputname);
	if(structname) free(structname);
	return 0;
}