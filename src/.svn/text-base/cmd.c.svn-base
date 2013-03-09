#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <getopt.h>

// readline
#include <readline/readline.h>
#include <readline/history.h>

#include "libpm.h"
#include "cmd.h"

int main(int argc, char** argv) {
	
	// parse command line options	
	const char* opts = OPTIONS;
	
	//options_t o;
	struct options_t o = {
		.num_opts = 0, 
		.num_args = 0, 
		.argc = 0
	};
	int r = opt_parse(argc, argv, opts, &o);
	
	/*	
	for (int i=0; i<o.num_opts; i++) {
		printf("%c = %s\n", o.opts[i].opt, o.opts[i].arg);
	}
	
	for (int i=0; i<o.num_args; i++)
		printf("%d, %s\n", i, argv[r+i]);
	
	exit(0);
	*/
	
	// first parameter is the path to the database, replace with getopt later on
	/*
	if (argc < r+1) {
		printf("Usage: pm <dbpath>\n");
		return 1;
	}
	*/
	
	// FIXME: use arg -d for database file, -m for mode
	//dbname = argv[r+1]; // ]argv[1];
	//printf("%s\n", dbname);
	
	/*
	char* db = "";
	opt_get('d', db, &o);
	printf("db: %s\n", db);
	*/
	
	char* db = opt_get('d', &o);
	if (db == NULL) {
		printf("Usage: ...\n");
		return 1;
	}

	char* mode = opt_get('m', &o);

	//printf("db: %s\n", db);
	
	// Create an int variable for storing the return code for each call
	int retval;
	
	// check if we have a data base file
	int db_state = access(db, W_OK);

	/*
	char* content;
	read_file("pm.c", &content);
	printf("%s", content);
	*/
	
	if (db_state == -1) {
		// file does not exist or is not readable. What to do?
		// FIXME: create new db or throw message about unreadable file

		printf("Failed to open db file: %s\n", db);
		return 1;
	}
	
	// Create a handle for database connection, create a pointer to sqlite3
	sqlite3 *dbhandle;
		
	// try to create the database. If it doesnt exist, it would be created
	// pass a pointer to the pointer to sqlite3, in short sqlite3**
	retval = sqlite3_open(db, &dbhandle);
	
	// If connection failed, dbhandle returns NULL
	if(retval) {
		printf("Database connection failed\n");
		return 2;
	}
	printf("Connection successful\n");
	
	printf("%s, %s, %d\n", mode, modes[MODE_POOL_CREATE], strcmp(mode, modes[MODE_POOL_CREATE]));
	
	if (strcmp(mode, modes[MODE_POOL_CREATE]) == 0) {
		/*
		pool_t rec = create_pool_rec();
		rec.title = "title";
		rec.description = "description";
		rec.author = "the author";
		rec.type = 1;
		//printf("id: %lu\ntitle: %s\n", rec.id, rec.title);
	
		sqlite3_int64 id = pool_create(&dbhandle, &rec);
		printf("Inserted pool id: %d\n", id);
		*/
		
		// check if we have user input from the command line
		char* title;
		char* author;
		char* description;
		
		printf("arg: %d, argc: %d\n", r, argc);
		for (int i=argc-o.num_args; i<o.argc; i++) {
			printf("%d = %s\n", i, argv[i]);
		}
		
		//mode_pool_create(dbhandle);
	}
	
	// show open pool
	// A prepered statement for fetching tables
	
	if (strcmp(mode, modes[MODE_POOL_LIST]) == 0) {
		display_view(dbhandle, "pool_open");
		//free(stmt);
		//free(sql);
	}
	
	// Close the dbhandle to free memory
	sqlite3_close(dbhandle);
	
	return 0;
}

int mode_pool_create(sqlite3 *dbhandle) {
	
	char* title =       readline("title:       ");
	char* description = readline("Description: ");
	char* author =      readline("Author:      ");	
	
	pool_t rec = create_pool_rec();
	rec.title = title;
	rec.description = description;
	rec.author = author;
	rec.type = 1;
	//printf("id: %lu\ntitle: %s\n", rec.id, rec.title);

	sqlite3_int64 id = pool_create(&dbhandle, &rec);
	
	// TODO: check for failed db p
	printf("Inserted pool id: %d\n", (int) id);
	
	return 0;
}



 
/**
 * create a task from pool
 * 
 * A poll entry needs to be supplemented with monre information to make 
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
long int pool_triage();

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

