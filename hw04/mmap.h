#pragma once
#define _GNU_SOURCE 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


typedef struct _sMmap {
	char *addr;
	int32_t fd;
	struct stat sb;
} sMmap;


sMmap *new_mmap(const char *filename, const char *modes, const int32_t map_flags);

void resize_mmap(sMmap *mmp, size_t new_size);

void free_mmap(sMmap *mmp);