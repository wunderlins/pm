#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __LIBTEST_H
#define __LIBTEST_H

struct st_test_t {
	int id;
	char *string;
};
typedef struct st_test_t st_test;

st_test struct_test(int id, char *string);

#endif
