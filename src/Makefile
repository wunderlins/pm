TARGET=debug
DST=../target/$(TARGET)
DB=/home/wus/tmp/pm.dat
LIBDIR=-L. -L../target/$(TARGET)
CC_OPTS=-Wall -std=c99 -I lib/ -g -DPM_DB_DEFAULT="$(DB)" $(LIBDIR)
CMD=gcc $(CC_OPTS) -o $(DST)/cmd cmd.c -lpm -lreadline -lpthread -ldl -lc 
SQL=gcc $(LIBDIR) -I lib/ lib/sqlite3.o -o $(DST)/sql sql.c -lpm -lreadline -lpthread -ldl -lc  

.PHONY: exe so db clean pypm sql cmd

all: 
	$(MAKE) exe so cmd sql pypm
	$(MAKE) -C ../test all
	
db:
	rm $(DB) || true
	sqlite3 $(DB) <var/schema.sql
	sqlite3 $(DB) <var/data.sql
	cp $(DB) pm.dat $(DST)/pm.dat
	
clean:
	$(MAKE) -C ../test clean
	$(RM) $(DST)/pm
	$(RM) *.o
	$(RM) *.pyc
	$(RM) $(DST)/*.so
	$(RM) $(DST)/cmd
	$(RM) $(DST)/sql
	
exe:
	echo '#!/usr/bin/env bash' > $(DST)/pm
	echo '# $$Id$$' >> $(DST)/pm
	echo 'LD_LIBRARY_PATH=. ./cmd "$$@"' >> $(DST)/pm
	chmod 700 $(DST)/pm
	../bin/git_id.sh $(DST)/pm $(DST)/pm
	
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

pypm:
	../bin/git_id.sh pypm.py $(DST)/pypm.py
	
