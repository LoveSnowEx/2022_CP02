#include "mmap.h"

sMmap *new_mmap(const char *filename, const char *modes, const int32_t map_flags) {
	char *addr;
	int32_t fd, access_modes = 0, prots = 0;
	struct stat sb;

	for(const char *cur = modes; *cur; ++cur) {
		switch(*cur) {
		case 'w':
			access_modes |= O_WRONLY | O_CREAT;
			prots |= PROT_WRITE;
			break;
		case 'r':
			access_modes |= O_RDONLY;
			prots |= PROT_READ;
			break;
		}
	}

	if((fd = open(filename, access_modes)) == -1) {
		fprintf(stderr, "error: failed to open file\n");
		return NULL;
	}

	if(fstat(fd, &sb) == -1) {
		fprintf(stderr, "error: failed to get file stat\n");
		close(fd);
		return NULL;
	}

	if((addr = mmap(NULL, sb.st_size, prots, map_flags, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "error: failed to mmap file\n");
		close(fd);
		return NULL;
	}

	sMmap *mmp = malloc(sizeof(sMmap));
	mmp->addr = addr;
	mmp->fd = fd;
	mmp->sb = sb;
	return mmp;
}

void resize_mmap(sMmap *mmp, size_t new_size) {
	mremap(mmp->addr, mmp->sb.st_size, new_size, MREMAP_MAYMOVE);
}

void free_mmap(sMmap *mmp) {
	munmap(mmp->addr, mmp->sb.st_size);
	close(mmp->fd);
	free(mmp);
}