/**
 *
 * EXIT CODES
 *   -1 General exception
 *    1 DB file does not exist or is not readable
 *    2 failed to open database
 * 
 * TODO:
 * - add config file
 */

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
	char* sql = (char*) malloc(sizeof(char)*130 + sizeof(pool->title) + \
	                           sizeof(pool->description) + sizeof(pool->author));
	
	sprintf(sql, "INSERT INTO pool (title, description, author, date, type) " \
	             "VALUES ('%s', '%s', '%s', %lu, %i)", \
	             pool->title, pool->description, pool->author, pool->date, \
	             pool->type);
	
	retval = sqlite3_exec(*dbhandle, sql, 0, 0, 0);
	
	if(retval) {
		printf("Inserting into pool failed, error: %d\n", retval);
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
		return errno;
		fclose(fd);
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
	int optcount = 0;
	opts->argc = argc;
	
	while ((option = getopt(argc, argv, optstring)) != -1) {
		opt_t o = {.opt = option, .arg = optarg};
		
		opts->opts[i] = o;
		opts->num_opts++;
		optcount++;
		if (optarg != NULL)
			optcount++;
		i++;
	}
	optcount++; // skip the program 
	
	opts->num_args = argc - optcount;
	
	/*
	i=0;
	for(int c=optcount+1; c<argc; c++) {
		printf("c: %d, i: %d - %s\n", c, i, argv[c]);
		//opts->args[c] = (char*) malloc(sizeof(char) * 255);
		opts->args[i++] = argv[c]; 
		// printf("%d\n", opts->args[c]);
		continue;
	}
	*/
	
	//opts->args = argv;
	
	return optcount;
}

