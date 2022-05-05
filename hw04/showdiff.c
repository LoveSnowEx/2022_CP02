#include "basic.h"
#include "mmap.h"
#include <dirent.h>

int main() {
	// char s[BUFSIZ];
	// char target[] = {0xb4, 0x48, 0xa5, 0x56, 0xaa, 0xba, 0xaa, 0x41, 0xb8, 0xcb, 0x00, 0x00};
	// char dirname[] = "/mnt/d/pm2";
	// DIR *d = opendir(dirname);
	// struct dirent *dir;
	// while((dir = readdir(d)) != NULL) {
	// 	char *end = strchr(dir->d_name, 10);
	// 	if(end) *end = 0;
	// 	sprintf(s, "%s/%s", dirname, dir->d_name);
	// 	// printf("file: |%s|\n", s);
		
	// 	sMmap *mmp = new_mmap(s, "r", MAP_SHARED);
	// 	if(!mmp) continue;
		
	// 	for(size_t i = 0; i < mmp->sb.st_size; ++i) {
	// 		if(memcmp(mmp->addr+i, target, sizeof(target)-2) != 0) continue;
	// 		printf("%04lx (in %s)\n", i, dir->d_name);
	// 		printf("%s\n", target);
	// 	}
	// 	free_mmap(mmp);
		
	// }
	// closedir(d);
	
	
	sMmap *mmp1 = new_mmap("/mnt/d/Files/Downloads/F104.GNX", MAP_SHARED);
	sMmap *mmp2 = new_mmap("/mnt/d/Files/Downloads/F105.GNX", MAP_SHARED);

	for(size_t i = 0; i < mmp1->sb.st_size; ++i) {
		if(mmp1->addr[i] != mmp2->addr[i]) {
			printf("%04lx: %02x|%02x\n", i, (uint8_t)mmp1->addr[i], (uint8_t)mmp2->addr[i]);
		}
	}
	
	free_mmap(mmp1);
	free_mmap(mmp2);
}
// B4 D6 AF BD AA BA AC 76 B8 CB 00 AE F0 BD E8 2B 31 35 00 42 5F 4B 49 48 49 4E 2B 31 35 00 B5 B7 BA F7 AA BA AC 76 B8 CB 00 AE F0 BD E8 2B 34 30 00 42 5F 4B 49 48 49 4E 2B 34 30 00 A5 D6 BD E8 AA BA AA 41 B8 CB 00 BE 79 A4 4F 2B 35 30 20 B9 44 BC 77 2D 32 30 00 42 5F 49 52 4F 4B 45 2B 35 30 20 42 5F 4D 4F 52 41 4C 53 2D 32 30 00 AD AB AA F7 C4 DD B8 CB 00 B7 50 A8 FC 2B 34 30 20 B9 44 BC 77 2D 33 30 00 42 5F 4B 41 4E 4A 55 53 45 49 2B 34 30 20 42 5F 4D 4F 52 41 4C 53 2D 33 30