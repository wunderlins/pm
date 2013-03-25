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


/** read a file into memmory
 *
 *
 * // example:
 * char* content;
 * read_file("pm.c", &content);
 * printf("%s", content);

int read_file(char* filename, char* contents[]) {
	int size = 0;

	struct stat sb;

	// file reachable and exists ?
	//printf("%s\n", filename);
	if (stat(filename, &sb) == -1)
		return errno;

	// file must be readable by this user
	FILE* fd;
	fd = fopen(filename, "r");

	// failed to open file
	if (fd == NULL)
		return errno;

	// get file length
	fseek(fd, 0, SEEK_END);
	size = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	// allocate memmory
	*contents = (char*) malloc((char) sizeof(char)*sb.st_size + 1);
	if (*contents == NULL) {
		fclose(fd);
		return errno;
	}
	//printf("%d\n", (int) sb.st_size);

	// check if the read size equals the file size
	if (size != fread(*contents, sizeof(char), sb.st_size, fd)) {
		free(*contents);
		fclose(fd);
		return 3; // -2 means file reading fail
	}

	// close handle
	fclose(fd);

	return 0;
}
 */

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("usage: print_file <filename>\n");
		exit(1);
	}

	char *contents;
	int r = read_file(argv[1], &contents);
	printf("%d, %s\n", r, contents);

	return 0;

}
