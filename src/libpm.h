// $Id: libpm.h 53 2013-03-08 21:46:40Z wunderlins $

#ifndef LIBPM_H_
#define LIBPM_H_

#define MODE_POOL_CREATE 1
#define MODE_POOL_LIST 2

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

pool_t create_pool_rec(void);
sqlite3_int64 pool_create(sqlite3 **dbhandle, pool_t *pool);
int display_view(sqlite3 *dbhandle, char* view);

#endif


