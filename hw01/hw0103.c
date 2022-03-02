#include "basic.h"
#include "mystring.h"
#include "mysplit.h"
#define MAXSIZE 8192


int main() {
	// struct timespec req, rem;
	// req.tv_nsec = 100000000;

	char str[MAXSIZE], sep[MAXSIZE];
	while (!fgets_n(str, MAXSIZE, stdin));
	// while (!fgets_n(sep, MAXSIZE, stdin));
	char **ppList = NULL;
	i32 counter = -1;
	mystrsplit(&ppList, &counter, str, NULL);
	for(i32 i = 0; i < counter; i++) {
		printf("%s\n", ppList[i]);
	}
	// nanosleep(&req, &rem);
	return 0;
}