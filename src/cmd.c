// needed for realpath, it is treated as GNU extension instead of posix
#define _GNU_SOURCE

#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <libgen.h>

// readline
#include <readline/readline.h>
#include <readline/history.h>

#include "libpm.h"
#include "cmd.h"

int verbose = 0;

void usage(void) {
	printf("Usage: pm [-d database] [-m mode]\n\n");
	printf(" -m: ");
	for(int i=0; i<MODE_LENGTH; i++) {
		printf("\"%s\" ", modes[i+1]);
	}
	printf("\n\nEnvironment Variables:\n");
	printf("PM_DB: path to the sqlite database\n");
}

int main(int argc, char** argv) {
	
	// parse command line options	
	const char* opts = OPTIONS;
	
	//options_t o;
	struct options_t o = {
		.num_opts = 0, 
		.num_args = 0, 
		.argc = 0,
		.optcount = 0
	};
	int r = opt_parse(argc, argv, opts, &o);
	if (verbose)
		printf("opt_parse(): option count %d\n", r);
	
	// check where the db has to be read from. the command line option
	// -d has first priority. If this is not given, then the environment 
	// variable PM_DB is checked. If still nothing was found the default 
	// file pm.dat (in the same folder as the executable) is used.
	char* db = opt_get('d', &o);
	if (db == NULL) {
		// check if we can use an environment variable
		char *db_env = getenv("PM_DB");		
		
		if (db_env == NULL) {
			
			// check if we can find a file named "pm.dat" in the same folder 
			// as the executable.
			char path[PATH_MAX + 1]; 
			path[0] = '\0';
			realpath((const char*) argv[0], path);
			if (path == NULL || path[0] == '\0') {
				usage();
				return 1;
			}
			dirname(path);
			char *tmp = strcat(path, "/");
			db = strcat(tmp, PM_DEFAULT_DB);
		} else
			db = db_env;
	}

	char* mode = opt_get('m', &o);
	if (mode == NULL) {
		usage();
		return 1;
	}
	
	// check if we have a data base file
	int db_state = access(db, W_OK);

	if (db_state == -1) {
		printf("Failed to open db file: %s\n", db);
		return 1;
	}
	
	// Create a handle for database connection, create a pointer to sqlite3
	sqlite3 *dbhandle;
		
	// Create an int variable for storing the return code for each call
	int retval;
	
	// open the database
	// pass a pointer to the pointer to sqlite3, in short sqlite3**
	retval = sqlite3_open(db, &dbhandle);
	
	// If connection failed, dbhandle returns NULL
	if(retval) {
		printf("Database connection failed\n");
		return 2;
	}
	printf("Connection successful\n");
	
	//printf("%s, %s, %d\n", mode, modes[MODE_POOL_CREATE], strcmp(mode, modes[MODE_POOL_CREATE]));
	
	if (strcmp(mode, modes[MODE_POOL_CREATE]) == 0) {
		/*
		printf("arg: %d, argc: %d\n", r, argc);
		for (int i=argc-o.num_args; i<o.argc; i++) {
			printf("%d = %s\n", i, argv[i]);
		}
		*/
		
		sqlite3_int64 ret = mode_pool_create(dbhandle, &o, argv);
		
		// failed to insert
		if (ret < 0) {
			usage();
			return 3;
		}
	}
	
	if (strcmp(mode, modes[MODE_POOL_LIST]) == 0) {
		display_view(dbhandle, "pool_open");
		//free(stmt);
		//free(sql);
	}
	
	// Close the dbhandle to free memory
	sqlite3_close(dbhandle);
	
	return 0;
}

/** insert pool entry
 *
 * uses the last 3 parameters as title, author and description. if the 
 * last parameters (description) is not provided it is read from stdin.
 *
 * parameters are read interactively from the command prompt if ommited as 
 3 command line argument.
 *
 * Returns:
 *   > 0, inserted id on success
 *   < 0, on error
 *
 *   -1, not enough arguments
 */
sqlite3_int64 mode_pool_create(sqlite3 *dbhandle, struct options_t* opts, char **argv) {
	
	char* title = NULL;
	char* author = NULL;
	char* description = NULL;
	
	printf("opts.num_opts: %d, opts.num_args: %d, opts.optcount: %d, opts.argc: %d\n", \
	       opts->num_opts, opts->num_args, opts->optcount, opts->argc);
	
	// read parameters from argv if given
	//       remaining param 1 = title	
	//       remaining param 2 = author
	//       remaining param 3 = description
	int c = 1;
	for (int i=opts->optcount; i<opts->argc; i++, c++) {
		switch (c) {
			case 1:
				title = argv[i];
				break;
			case 2:
				author = argv[i];
				break;
			case 3:
				description = argv[i];
				break;
		}
	}
	
	// if there is no param 3 provided or param 3 is == "-" then check 
	// for data on stdin
	//  - check if we have input from stdin, see test/select
	//  - if there is input on stdin, read it into memory,
	//    see libpm/str_readfile()
	fd_set s_rd, s_wr, s_ex;
	FD_ZERO(&s_rd);
	FD_ZERO(&s_wr);
	FD_ZERO(&s_ex);
	FD_SET(fileno(stdin), &s_rd);
	struct timeval timeout = {
		.tv_sec = 0,
		.tv_usec = 20
	};
	int ret = select(fileno(stdin)+1, &s_rd, &s_wr, &s_ex, &timeout);
	if (ret > 0) {
		string input = str_readfile(stdin, 0);
		description = input.text;
	}
	
	// interactive input for missing arguments
	if (title == NULL)
		title =       readline("title:       ");
	if (author == NULL)
		author =      readline("Author:      ");	
	if (description == NULL)
		description = readline("Description: ");
	
	// we need all three parameters, else abort
	if (title == NULL || author == NULL || description == NULL)
		return -1;
	
	pool_t rec = create_pool_rec();
	rec.title = title;
	rec.description = description;
	rec.author = author;
	rec.type = 1;
	//printf("id: %lu\ntitle: %s\n", rec.id, rec.title);
	
	sqlite3_int64 id = pool_create(&dbhandle, &rec);
	
	// TODO: check for failed db operation
	printf("Inserted pool id: %d\n", (int) id);
	
	return id;
}
 
/**
 * create a task from pool
 * 
 * A pool entry needs to be supplemented with monre information to make 
 * it a task.
 *
 * Minimal requirement to make a task from a pool entry are:
 * - urgency
 * - importance
 * - assignee (Person)
 * - accountable (Person)
 * - informed (Person)
 * - tasktype
 */
long int pool_triage(void) {
	return 0;
}

