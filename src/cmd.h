#ifndef CMD_H_
#define CMD_H_

#define USAGE_BEGIN "" \
"Usage: pm [-d database] [-m mode]" \
"\n"

#define USAGE_END "" \
"\n" \
"\n" \
"EXAMPLE:\n" \
"  pm -m pcreate <title> <author> [description]\n" \
"  echo description | pm -m pcreate <title> <author>\n" \
"  pm -m pcreate # will ask intreactively for input\n" \
"\nVARIABLES:\n" \
"  PM_DB: path to the sqlite database\n" \
"\n" \
"EXIT CODES:\n" \
"  1: parameter missing\n" \
"  2: Database connection failed\n" \
"  3: Failed to insert into pool\n" \
"  4: Failed to resolve directory of executable\n" \
"  5: argument -m mode is missing\n" \
"  6: unknown mode -m mode is missing\n" \
"\n"

void usage(void);
sqlite3_int64 mode_pool_create(sqlite3 *dbhandle, struct options_t* opts, char **argv);
long int pool_triage(void);

#endif
