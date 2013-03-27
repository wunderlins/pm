#include "libtest.h"

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s <id> <filename>\n", argv[0]);
		exit(1);
	}

	printf("struct_test(atoi(argv[1]), argv[2]);\n");
	st_test v = struct_test(atoi(argv[1]), argv[2]);
	printf("st_test {id: %d, string: \"%s\"}\n", v.id, v.string);

	printf("str_readfile(argv[2], 512);\n");
	string r = str_readfile(argv[2], 512);
	printf("string {memsize: %d, length: %d, text: \"%s\", error: %d}\n",
	       r.memsize, r.length, r.text, r.error);

	return 0;
}

st_test struct_test(int id, char *string) {
	st_test i;
	i.id = id;

	// allocate mem for a copy of string
	char *append = " -> end";
	char *str = (char*) malloc(sizeof(char) *
			        (strlen(string) + strlen(append) + 1));
	strcpy(str, string);
	strcat(str, append);

	i.string = str;
	return i;
}

/** Read file from FILE pointer
 *
 * Reads data from FILE *file and dynamically allocates memory from the heap
 * to store the read data. If memory allocation fails, the returned string
 * struct's member error will equal to 1.
 *
 * If reading the file works, string.error will be 0.
 */
string str_readfilef(FILE *file, int bs) {
	int charsize = sizeof(char);
	int charcount = 0;
	int blocksize;
	if (!bs)
		blocksize = DEFAULT_BLOCKSIZE;
	else
		blocksize = bs;
	int memsize = blocksize;
	char *line = (char*) malloc(charsize * (memsize+1));
	char *linep = line;

	string l;
	l.error = 0;
	l.length = 0;
	l.memsize = memsize;
	l.length = charcount;
	l.text = linep;

	while (1) {

		if(charcount > memsize) {
			// reallocate memmory
			memsize = memsize+blocksize;
			char *al = (char*) realloc(linep, charsize * (memsize+1));

			if (al == NULL) {
				fprintf(stderr, "str_readfile(): Error, out of memory.\n");
				*line = '\0';
				l.text = linep;
				l.error = 1;
				return l;
			}
		}

		char c = fgetc(file);

		//if (c == '\n' || c == '\r' || c == EOF) {
		if (c == EOF) {
			charcount--;
			break;
		}

		// remember input char
		*line = c;
		(void) *(line++);
		charcount++;
	}

	*line = '\0';
	l.text = linep;
	l.memsize = memsize;
	l.length = charcount;

	return l;
}

string str_readfile(char *filename, int bs) {
	struct stat sb;

	// file reachable and exists ?
	//printf("%s\n", filename);
	if (stat(filename, &sb) == -1) {
		string l = {
			.error = errno,
			.length = 0,
			.memsize = 0,
			.length = 0,
			.text = '\0'
		};
		return l;
	}

	// file must be readable by this user
	FILE* fd;
	fd = fopen(filename, "r");

	// failed to open file
	if (fd == NULL) {
		string l = {
			.error = errno,
			.length = 0,
			.memsize = 0,
			.length = 0,
			.text = '\0'
		};
		return l;
	}

	return str_readfilef(fd, bs);
}
