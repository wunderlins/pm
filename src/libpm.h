#ifndef LIBPM_H_
#define LIBPM_H_

#include <stdio.h>
#include <stddef.h>
#include <sqlite3.h>

#define MODE_POOL_CREATE 1
#define MODE_POOL_LIST 2
#define MODE_SQL 3
#define MODE_LENGTH 3

#define PM_DEFAULT_DB "pm.dat"
#define XSTR(x) #x
#define STR(x) XSTR(x)

#ifdef PM_DB_DEFAULT
	const char t[255] = STR(PM_DB_DEFAULT);
	const char *default_db = t;
#else
	const char *default_db = NULL;
#endif

#define OPTIONS "d:m:"

const char* modes[] = {
	"", 
	"pcreate",
	"plist",
	"sql"
};

typedef struct pool_t {
	long int id;
	char* title;
	char* description;
	char* author;
	long date;
	int type;
	long id_task;
} pool_t;

typedef struct opt_t {
	char opt;
	char* arg;
} opt_t;

typedef struct options_t {
	opt_t opts[255];
	int num_opts; // number of parsed options
	int num_args; // number of remaining command line arguments
	int argc;
	int optcount;
	//char* args[];  // remaining arguments
} options_t;

sqlite3 *open_db(char* file);
pool_t create_pool_rec(void);
sqlite3_int64 pool_create(sqlite3 **dbhandle, pool_t *pool);
int display_view(sqlite3 *dbhandle, char* view);
int read_file(char* filename, char** contents);
char* opt_get(char opt, struct options_t* opts);
int opt_parse(int argc, char *argv[], const char * optstring, struct options_t* opts);

// read file from FILE to memmory
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
string str_readfilef(FILE *file, int bs);
string str_readfile(char *filename, int bs);
#endif


