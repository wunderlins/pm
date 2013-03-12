DB=~/tmp/pm.db
CC_OPTS=-Wall -std=c99 -I lib/ -D PM_DB=$(DB)
CMD=gcc $(CC_OPTS) -o cmd cmd.c -L. -lpm -lreadline -lpthread -ldl -lc 
SQL=gcc -I lib/ lib/sqlite3.o -o sql sql.c -L. -lpm -lreadline -lpthread -ldl -lc 

all: so cmd sql
	$(CMD)
	$(SQL)
	
db:
	rm $(DB) || true
	sqlite3 $(DB) <var/schema.sql
	sqlite3 $(DB) <var/data.sql
	cp $(DB) pm.dat
	
clean:
	rm pm || true
	rm *.o || true
	rm *.so || true
	rm cmd || true
	rm sql || true
	
exe:
	echo '#!/usr/bin/env bash' > pm
	echo '' >> pm
	echo 'LD_LIBRARY_PATH=. ./cmd "$$@"' >> pm
	chmod 700 pm
	
sqlite3:
	gcc $(CC_OPTS) -c -fPIC lib/sqlite3.c -o lib/sqlite3.o -lpthread -ldl -lc 

libpm:
	gcc $(CC_OPTS) -c -fPIC libpm.c -o libpm.o

so: sqlite3 libpm
	gcc $(CC_OPTS) -shared -Wl,-soname,libpm.so -o libpm.so lib/sqlite3.o libpm.o

cmd:
	$(CMD)

sql:
	$(SQL)

test/select:
	gcc $(CC_OPTS) -o test/select test/select.c


