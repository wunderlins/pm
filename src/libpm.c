// $Id: libpm.c 53 2013-03-08 21:46:40Z wunderlins $

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


