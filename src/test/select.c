#define _POSIX_C_SOURCE 1

#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdlib.h>

#ifndef DEFAULT_BLOCKSIZE
	// num. bytes of junks of memory to allocate from the heap while reading 
	// strings from file io to mem
	#define DEFAULT_BLOCKSIZE 128
#endif

/** datatype to hold the character data */
struct string_t {
	int memsize;
	int length;
	char *text;
	int error;
};
typedef struct string_t string;

// function declarations
string str_readfile(FILE *file, int bs);

int main(int argc, char **argv) {
	
	/*
	struct timeval {
		long    tv_sec;         // seconds
		long    tv_usec;        // microseconds
	};
	*/ 

	fd_set s_rd, s_wr, s_ex;
	FD_ZERO(&s_rd);
	FD_ZERO(&s_wr);
	FD_ZERO(&s_ex);
	FD_SET(fileno(stdin), &s_rd);
	struct timeval timeout = {
		.tv_sec = 0,
		.tv_usec = 100
	};
	int ret = select(fileno(stdin)+1, &s_rd, &s_wr, &s_ex, &timeout);
	
	printf("I has stdio: %d\n", ret);
	
	// no input
	if (ret == 0)
		return 0;
	
	// read stdin into char
	string input = str_readfile(stdin, 0);
	
	if (input.error == 0)
		printf("%s\n", input.text);
	
	return 0;
}

/** Read file from FILE pointer
 *
 * Reads data from FILE *file and dynamically allocates memory from the heap
 * to store the read data. If memory allocation fails, the returned string
 * struct's member error will equal to 1.
 *
 * If reading the file works, string.error will be 0.
 */
string str_readfile(FILE *file, int bs) {
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

