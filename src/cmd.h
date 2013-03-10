#ifndef CMD_H_
#define CMD_H_

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

void usage(void);
int mode_pool_create(sqlite3 *dbhandle);
char* opt_get(char opt, struct options_t* opts);
int opt_parse(int argc, char *argv[], const char * optstring, struct options_t* opts);

#endif
