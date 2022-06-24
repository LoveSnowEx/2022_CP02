#include "basic.h"
#include "cJSON.h"

int main() {
	char buf[BUFSIZ];
	FILE *fp = fopen("game.json", "r");
	fread(buf, 1, sizeof(buf), fp);
	cJSON *json = cJSON_Parse(buf);
	char *str = cJSON_Print(json);
	// printf("%s\n", str);
	write(1, str, strlen(str));
	write(1, "\n", 1);
	free(str);
	cJSON_Delete(json);
	return 0;
}