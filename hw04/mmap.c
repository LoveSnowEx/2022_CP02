#include "mmap.h"


sMmap *new_mmap(const char *filename, const int32_t map_flags) {
	char *addr;
	int32_t fd, access_modes = O_RDWR | O_CREAT, prots = PROT_READ | PROT_WRITE;
	struct stat sb;

	if((fd = open(filename, access_modes, S_IRWXU|S_IRGRP|S_IROTH)) == -1) {
		fprintf(stderr, "error: failed to open file\n");
		return NULL;
	}

	if(fstat(fd, &sb) == -1) {
		fprintf(stderr, "error: failed to get file stat\n");
		close(fd);
		return NULL;
	}
	if(sb.st_size == 0) {
		if(ftruncate(fd, 1) == -1) {
			fprintf(stderr, "error: failed to truncate: %s\n", strerror(errno));
			close(fd);
			return NULL;
		}
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

int sync_mmap(sMmap *mmp) {
	if(msync(mmp->addr, mmp->sb.st_size, MS_SYNC) == -1) {
		fprintf(stderr, "error: failed to sync: %s\n", strerror(errno));
		return -1;
	}
	return 0;
}

int resize_mmap(sMmap *mmp, size_t new_size) {
	printf("new size: %ld\n", new_size);
	if(ftruncate(mmp->fd, new_size) == -1) {
		fprintf(stderr, "error: failed to truncate: %s\n", strerror(errno));
		return -1;
	}
	if((mmp->addr = mremap(mmp->addr, mmp->sb.st_size, new_size, MREMAP_MAYMOVE)) == MAP_FAILED) {
		fprintf(stderr, "error: failed to remap file\n");
		return -1;
	}
	if(fstat(mmp->fd, &mmp->sb) == -1) {
		fprintf(stderr, "error: failed to get file stat\n");
		return -1;
	}
	return 0;
}

int free_mmap(sMmap *mmp) {
	if(munmap(mmp->addr, mmp->sb.st_size) == -1) {
		fprintf(stderr, "error: failed to munmap: %s\n", strerror(errno));
		close(mmp->fd);
		return -1;
	}
	close(mmp->fd);
	free(mmp);
	return 0;
}