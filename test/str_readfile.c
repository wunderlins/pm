#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include "sqlite3.h"
#include "libpm.h"

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("usage: str_readfile <filename>\n");
		exit(1);
	}

	string contents = str_readfile(argv[1], 0);
	printf("%d, %s\n", contents.length, contents.text);

	return 0;

}
