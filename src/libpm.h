#ifndef LIBPM_H_
#define LIBPM_H_

#define MODE_POOL_CREATE 1
#define MODE_POOL_LIST 2
#define MODE_LENGTH 2

#define PM_DEFAULT_DB "pm.dat"

#define OPTIONS "d:m:"

const char* modes[] = {
	"", 
	"pool create",
	"pool list"
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
	struct opt_t opts[36];
	int num_opts; // number of parsed options
	int num_args; // number of remaining command line arguments
	int argc;
	//char* args[];  // remaining arguments
} options_t;

pool_t create_pool_rec(void);
sqlite3_int64 pool_create(sqlite3 **dbhandle, pool_t *pool);
int display_view(sqlite3 *dbhandle, char* view);
int read_file(char* filename, char** contents);
char* opt_get(char opt, struct options_t* opts);
int opt_parse(int argc, char *argv[], const char * optstring, struct options_t* opts);

#endif


