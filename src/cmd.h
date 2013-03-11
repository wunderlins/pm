#ifndef CMD_H_
#define CMD_H_

#define USAGE_BEGIN "" \
"Usage: pm [-d database] [-m mode]" \
"\n"

#define USAGE_END "" \
"\n\nVARIABLES:\n" \
"PM_DB: path to the sqlite database\n" \
"\n" \
"EXIT CODES:\n" \
"  1: parameter missing\n" \
"  2: Database connection failed\n" \
"  3: Failed to insert into pool\n"

void usage(void);
sqlite3_int64 mode_pool_create(sqlite3 *dbhandle, struct options_t* opts, char **argv);
long int pool_triage(void);

#endif
