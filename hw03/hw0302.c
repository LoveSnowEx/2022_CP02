#include "basic.h"

#define BUFSIZE 8192

void clearSrtTrash(char *nptr, size_t size) {
	size_t len = 0;
	for(size_t i = 0; i < size; i++) {
		if(isprint(nptr[i]) || nptr[i] == '\0') nptr[len++] = nptr[i];
		if(nptr[i] == '\0') break;
	}
}

int main() {
	u64 hr1, min1, sec1, msec1, hr2, min2, sec2, msec2, last_ns = 0, from_ns, to_ns;
	double speed;
	char buf1[BUFSIZE], buf2[4][BUFSIZE], *end;
	struct timespec rqts, rmts;

	printf("Open a srt file: ");
	while(!fgets_n(buf1, sizeof(buf1), stdin));

	FILE *pFile = fopen(buf1, "rb");
	if(pFile == NULL) {
		printf("error: open file fail.\n");
		return 0;
	}

	printf("Speed (0.5-2): ");
	while(!fgets_n(buf1, sizeof(buf1), stdin));
	speed = strtod(buf1, &end);
	if(buf1 == end || speed < 0.5 || speed > 2) {
		printf("error: invalid speed input.\n");
		return 1;
	}

	system("clear");
	while(!feof(pFile)) {
		for(size_t i = 0; i < 3; i++) {
			while(!feof(pFile) && !fgets_n(buf2[i], sizeof(buf2[i]), pFile));
			clearSrtTrash(buf2[i], sizeof(buf2[i]));
		}
		if(strtol(buf2[0], NULL, 10) == 0L) break;

		sscanf(buf2[1], "%lu:%lu:%lu,%lu --> %lu:%lu:%lu,%lu", &hr1, &min1, &sec1, &msec1, &hr2, &min2, &sec2, &msec2);
		
		from_ns = (((hr1*60+min1)*60+sec1)*1000+msec1)*1000000;
		to_ns = (((hr2*60+min2)*60+sec2)*1000+msec2)*1000000;
		// printf("from: %lu, to: %lu\n", from_ns, to_ns);
		
		rqts.tv_sec = (from_ns-last_ns)/1000000000/speed;
		rqts.tv_nsec = (from_ns-last_ns)%1000000000/speed;
		// printf("sleep: sec: %lu, nsec: %lu\n", rqts.tv_sec, rqts.tv_nsec);
		nanosleep(&rqts, &rmts);

		printf("%s\n", buf2[2]);

		rqts.tv_sec = (to_ns - from_ns)/1000000000/speed;
		rqts.tv_nsec = (to_ns - from_ns)%1000000000/speed;
		// printf("sleep: sec: %lu, nsec: %lu\n", rqts.tv_sec, rqts.tv_nsec);
		nanosleep(&rqts, &rmts);

		system("clear");
		
		last_ns = to_ns;
	}
}
