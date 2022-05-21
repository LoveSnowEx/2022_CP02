#include <popt.h>
#include "basic.h"

enum eShowType {
	BYTES = 0,
	KILO,
	MEGA,
};

int main(int argc, char *argv[]) {
	bool showinbytes = false, showinkb = false, showinmb = false;
	enum eShowType showtype = KILO;
	char opt, buf[BUFSIZ];
	double seconds = DBL_MIN;
	i32 count = INT32_MIN;

	// opt table
	struct poptOption optionsTable[] = {
		{"bytes", 'b', POPT_ARG_NONE, 0, 'b',
			"Show output in bytes", "<bytes>"},
		{"kilo", 0, POPT_ARG_NONE, 0, 'k',
			"Show output in kilobytes (default)", "<kilobytes>"},
		{"mega", 0, POPT_ARG_NONE, 0, 'm',
			"Show output in megabytes", "<megabytes>"},
		{"seconds", 's', POPT_ARG_DOUBLE, &seconds, 's',
			"Repeat printing every N seconds", "N"},
		{"count", 'c', POPT_ARG_INT, &count, 'c',
			"Repeat printing N times , then exit", "N"},
		POPT_AUTOHELP
		POPT_TABLEEND
	};

	// get context
	poptContext optCon = poptGetContext(NULL, argc, (const char **)argv, optionsTable, 0);

	// read opt
	while((opt = poptGetNextOpt(optCon)) > 0) {
		// printf("opt: %c\n", opt);
		switch(opt) {
		case 'b':
			showinbytes = true;
			showtype = BYTES;
			break;
		case 'k':
			showinkb = true;
			showtype = KILO;
			break;
		case 'm':
			showinmb = true;
			showtype = MEGA;
			break;
		}
	}

	// an error occurred during option processing
	if (opt < -1) {
		fprintf(stderr, "error: %s: %s\n",
				poptBadOption(optCon, POPT_BADOPTION_NOALIAS),
				poptStrerror(opt));
		poptFreeContext(optCon);
		exit(1);
	}

	// multiple unit
	if((i32)showinbytes + (i32)showinkb + (i32)showinmb > 1) {
		fprintf(stderr, "error: multiple unit options doesn't make sense.\n");
		poptFreeContext(optCon);
		exit(1);
	}

	// both seconds and count are not set
	if(seconds == DBL_MIN && count == INT32_MIN) count = 1;
	else {
		if(seconds != DBL_MIN && seconds <= 0) {  // seconds is not postive
			fprintf(stderr, "error: seconds argument '%g' is not postive number\n", seconds);
			poptFreeContext(optCon);
			exit(1);
		}
		if(count != INT32_MIN && count <= 0) {  // count is not postive
			fprintf(stderr, "error: count argument '%d' is not postive number\n", count);
			poptFreeContext(optCon);
			exit(1);
		}
		// one of them are set, and seconds is not set, so set second to 1
		if(seconds == DBL_MIN) seconds = 1;
	}

	// printf("sec: %g\ncnt: %d\n", seconds, count);

	FILE *pFile = fopen("/proc/meminfo", "r");

	// open failed
	if(pFile == NULL) {
		fprintf(stderr, "error: failed to read /proc/meminfo\n");
		poptFreeContext(optCon);
		exit(1);
	}

	uint64_t memtotal = 0, memavailable = 0;
	while(count == INT32_MIN || count-- > 0) {
		while(fgets_n(buf, sizeof(buf), pFile)) {
			// fwrite(buf, 1, strlen(buf), stdout);
			uint32_t ram;
			if(sscanf(buf, "MemTotal: %u kB", &ram) == 1) {
				memtotal = ram;
				continue;
			}
			if(sscanf(buf, "MemAvailable: %u kB", &ram) == 1) {
				memavailable = ram;
				continue;
			}
		}
		switch(showtype) {
		case BYTES:
			printf("Available: %12lu B  (%2.2lf%%)\n", memavailable*1000UL, 100.0*memavailable/memtotal);
			break;
		case KILO:
			printf("Available: %12lu KB (%2.2lf%%)\n", memavailable, 100.0*memavailable/memtotal);
			break;
		case MEGA:
			printf("Available: %12lu MB (%2.2lf%%)\n", memavailable/1000UL, 100.0*memavailable/memtotal);
			break;
		}
		
		
		if(count == 0) break;
		struct timespec req;
		req.tv_sec = (int)seconds;
		req.tv_nsec = (seconds-req.tv_sec)*1000000000;
		
		nanosleep(&req, NULL);
		fseek(pFile, 0, SEEK_SET);
	}
	
	fclose(pFile);
	poptFreeContext(optCon);
	return 0;
}