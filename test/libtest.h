#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#ifndef __LIBTEST_H
#define __LIBTEST_H

struct st_test_t {
	int id;
	char *string;
};
typedef struct st_test_t st_test;

st_test struct_test(int id, char *string);

// read file from FILE to memmory
#ifndef DEFAULT_BLOCKSIZE
	// num. bytes of junks of memory to allocate from the heap while reading
	// strings from file io to mem
	#define DEFAULT_BLOCKSIZE 128
#endif

struct string_t {
	int memsize;
	int length;
	char *text;
	int error;
};
typedef struct string_t string;

string str_readfile(char *filename, int bs);

#endif
