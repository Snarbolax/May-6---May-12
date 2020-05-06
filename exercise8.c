#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <stdlib.h>
#include <dirent.h>
#include <stddef.h>

#define MAX_PATH 1024

// header typedef from book
typedef long Align;  /* for alignment to long boundary */
union header {       /* block header: */
	struct {
		union header *ptr; /* next block if on free list */
		unsigned size;     /* size of this block */
	} s;
	Align x;         /* force alignment of blocks */
};
typedef union header Header;

// Exericse 8-5
void dirwalk(char* dir, void(*fcn)(char *)) {
	char name[MAX_PATH];
	struct dirent *dp;
	DIR *dfd;
	if ((dfd = opendir(dir)) == NULL) {
		fprintf(stderr, "dirwalk: canŒt open %s\n", dir);
		return;
	}
	puts(dir);
	while ((dp = readdir(dfd)) != NULL) {
		if (strcmp(dp->d_name, ".") == 0
			|| strcmp(dp->d_name, "..") == 0)
			continue;
		if (strlen(dir) + strlen(dp->d_name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s/%s too long\n",
		else {
			sprintf(name, "%s/%s", dir, dp->d_name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}

void fsize(char* name) {
	struct stat stbuf;

	if (stat(name, &stbuf) == -1) {
		fprintf(stderr, "fsize: canŒt access %s\n", name);
		return;
	}

	if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
		dirwalk(name, fsize);
	}
	struct passwd *pwd = getpwuid(stbuf.st_uid);
	printf("%81d %s\n", (int)stbuf.st_size, name, pwd->pw_name);
}

// Exercise 8-6
void* calloc2(size_t n, size_t size)
{
	void* result = NULL;
	result = malloc(n * size);
	if (NULL != result) { memset(result, 0, n * size); }
	return result;
}

// Exercise 8-8
void *bfree(void* p, size_t n) {
	if (n < sizeof(Header)) { return NULL; }
	Header* bp = (Header *)p;
	bp->s.size = (n / sizeof(Header));
	free(bp + 1);
}