# $Id: Makefile 54 2013-03-08 21:49:47Z wunderlins $

CC_OPTS=-Wall -std=c99 -I lib/

all:
	gcc $(CC_OPTS) -c -fPIC lib/sqlite3.c -o lib/sqlite3.o -lpthread -ldl -lc 
	gcc $(CC_OPTS) -c -fPIC libpm.c -o libpm.o
	gcc $(CC_OPTS) -shared -Wl,-soname,libpm.so -o libpm.so lib/sqlite3.o libpm.o
	
	gcc $(CC_OPTS) -o cmd cmd.c -L. -lpm -lreadline -lpthread -ldl -lc 
	
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
	
exe:
	echo '#!/usr/bin/env bash' > pm
	echo '' >> pm
	echo '# $$Id: Makefile 54 2013-03-08 21:49:47Z wunderlins $$	' >> pm
	echo ''	>> pm
	echo 'LD_LIBRARY_PATH=. ./cmd "$$@"' >> pm
	chmod 700 pm

