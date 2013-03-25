/*

User interface definition:

"bin" -d [path to db] -m [mode]
 - list modes / help?
 - create a pool entry
 - remove a pool entry
 - modify a pool entry
 
 - convert pool to task
 - modify task 
 
 - sql interface
 
 - search by criteria
   - like over title/description
   - by author
   - by type / tasktype
 
 - daemon 
   - import from imap
   - import from csv
 
 - implement external viewer for other content types than 
   text/plain
*/

#include <stdio.h>
#include <sqlite3.h>
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
#include <sys/select.h>
#include <sys/time.h>


// our own libs
#include "libpm.h"

// data management
int display_view(sqlite3 *dbhandle, char* view) {
	int retval;	
	sqlite3_stmt *stmt;
	
	// select those rows from the table
	char* sqlt = "SELECT * from %s";
	char* sql = (char*)malloc(sizeof(sqlt) + sizeof(view) + 2);
	sprintf(sql, sqlt, view);
	retval = sqlite3_prepare_v2(dbhandle, sql, -1, &stmt, 0);
	if(retval) {
		printf("Selecting data from DB Failed\n");
		return -1;
	}

	// Read the number of rows fetched
	int cols = sqlite3_column_count(stmt);
			
	while(1) {
		// fetch a row's status
		retval = sqlite3_step(stmt);
		
		if(retval == SQLITE_ROW) {
			// SQLITE_ROW means fetched a row
					
			// sqlite3_column_text returns a const void* , typecast it to const char*
			for(int col=0; col<cols; col++) {
				const char *val = (const char*)sqlite3_column_text(stmt, col);
				printf("%s\t", /*sqlite3_column_name(stmt, col),*/ val);
			}
			printf("\n");
		} else if(retval == SQLITE_DONE) {
			// All rows finished
			printf("All rows fetched\n");
			break;
		}	else 	{
			// Some error encountered
			printf("Some error encountered\n");
			return -1;
		}
	}
	
	return 0;
}

pool_t create_pool_rec(void) {
	pool_t p = {
		.id = 0, 
		.title = "", 
		.description = "",
		.author = "", 
		.date = time(NULL),
		.type = 0,
		.id_task = 0
	};
	
	return p;
}

/**
 * add a new pool entry
 *
 */
sqlite3_int64 pool_create(sqlite3 **dbhandle, pool_t *pool) {
	int retval;
	char* sql = (char*) malloc(sizeof(char)*200 + \
	                           strlen(pool->title)*sizeof(char) + \
	                           strlen(pool->description)*sizeof(char) + \
	                           strlen(pool->author)*sizeof(char));
	
	sprintf(sql, "INSERT INTO pool (title, description, author, date, type) " \
	             "VALUES ('%s', '%s', '%s', %lu, %i)", \
	             pool->title, pool->description, pool->author, pool->date, \
	             pool->type);
	
	char *errmsg = NULL;
	retval = sqlite3_exec(*dbhandle, (const char*) sql, NULL, NULL, &errmsg);
	
	if(retval) {
		printf("Inserting into pool failed, error: %d\n", retval);
		printf("%s\n", errmsg);
		sqlite3_free(errmsg);
		return -1;
	}
	
	sqlite3_int64 id = sqlite3_last_insert_rowid(*dbhandle);
	return id;
}

/** read a file into memmory
 *
 *
 * // example: 
 * char* content;
 * read_file("pm.c", &content);
 * printf("%s", content);
 */
int read_file(char* filename, char** contents) {
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
	*contents = (char*) malloc((char) sizeof(char)*size + 1);
	if (*contents == NULL) {
		fclose(fd);
		return errno;
	}
	//printf("%d\n", (int) sb.st_size);
	
	// check if the read size equals the file size
	if (size != fread(*contents, sizeof(char), sb.st_size, fd)) {
		free(*contents);
		fclose(fd);
		return 3;
	}
	
	// close handle
	fclose(fd);
	
	return 0;
}

char* opt_get(char opt, struct options_t* opts) {
	
	for(int i=0; i<opts->num_opts; i++) {
		//printf("c: %d, i: %d - %s\n", c, i, argv[c]);
		//opts->args[c] = (char*) malloc(sizeof(char) * 255);
		//opts->args[i++] = argv[c]; 
		// printf("%d\n", opts->args[c]);
		//continue;
		
		//printf("%c, %s\n", opts->opts[i].opt, opts->opts[i].arg);
		if (opts->opts[i].opt == opt) {
			//value = (char*)malloc(sizeof(char)*sizeof(opts->opts[i].arg));
			//value = opts->opts[i].arg;
			//printf("yay\n");
			return opts->opts[i].arg;
//			return 0;
		}
	}
	
	return NULL;
}

int opt_parse(int argc, char *argv[], const char * optstring, struct options_t* opts) {
	
	opterr = 0;
	
	int option;
	int i = 0;
	//int optcount = 0;
	opts->argc = argc;
	
	while ((option = getopt(argc, argv, optstring)) != -1) {
		opt_t o = {.opt = option, .arg = optarg};
		
		opts->opts[i] = o;
		opts->num_opts++;
		opts->optcount++;
		if (optarg != NULL)
			opts->optcount++;
		i++;
	}
	opts->optcount++; // skip the program 
	
	opts->num_args = argc - opts->optcount;
	
	/*
	i=0;
	for(int c=opts->optcount+1; c<argc; c++) {
		printf("c: %d, i: %d - %s\n", c, i, argv[c]);
		//opts->args[c] = (char*) malloc(sizeof(char) * 255);
		opts->args[i++] = argv[c]; 
		// printf("%d\n", opts->args[c]);
		continue;
	}
	*/
	
	//opts->args = argv;
	
	return opts->optcount;
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

