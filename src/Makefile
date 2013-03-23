DST=../target/debug

DB=/home/wus/tmp/pm.dat
CC_OPTS=-Wall -std=c99 -I lib/ -g -DPM_DB_DEFAULT="$(DB)"
#CC_OPTS=-Wall -std=c99 -I lib/ -g 
CMD=gcc $(CC_OPTS) -o $(DST)/cmd cmd.c -L. -lpm -lreadline -lpthread -ldl -lc 
SQL=gcc -I lib/ lib/sqlite3.o -o $(DST)/sql sql.c -L. -lpm -lreadline -lpthread -ldl -lc 

all: exe so cmd sql
	$(CMD)
	$(SQL)
	
db:
	rm $(DB) || true
	sqlite3 $(DB) <var/schema.sql
	sqlite3 $(DB) <var/data.sql
	cp $(DB) pm.dat $(DST)/pm.dat
	
clean:
	rm pm || true
	rm *.o || true
	rm *.so || true
	rm cmd || true
	rm sql || true
	
exe:
	echo '#!/usr/bin/env bash' > $(DST)/pm
	echo '' >> $(DST)/pm
	echo 'LD_LIBRARY_PATH=. ./cmd "$$@"' >> $(DST)/pm
	chmod 700 $(DST)/pm
	
sqlite3:
	gcc $(CC_OPTS) -c -fPIC lib/sqlite3.c -o lib/sqlite3.o -lpthread -ldl -lc 

libpm:
	gcc $(CC_OPTS) -c -fPIC libpm.c -o libpm.o

so: sqlite3 libpm
	gcc $(CC_OPTS) -shared -Wl,-soname,libpm.so -o $(DST)/libpm.so lib/sqlite3.o libpm.o

cmd:
	$(CMD)

sql:
	$(SQL)

test/select:
	gcc $(CC_OPTS) -o test/select test/select.c


