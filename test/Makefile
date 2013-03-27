.PHONY: read_file libtest select str_readfile clean
CC_OPTS=-Wall -std=c99
LIBNAME=test

all: read_file libtest select str_readfile

clean:
	$(RM) read_file
	$(RM) libtest
	$(RM) libtest.o
	$(RM) libtest.so
	$(RM) read_file
	$(RM) select
	$(RM) str_readfile
	$(RM) *.pyc

select:
	gcc $(CC_OPTS) -o select select.c


read_file:
	gcc -Wall -o read_file read_file.c -I ../src -L ../target/debug \
	    -I ../src/lib -lpm -lreadline -lpthread -ldl -lc 

str_readfile:
	gcc -Wall -o str_readfile str_readfile.c -I ../src -L ../target/debug \
	    -I ../src/lib -lpm -lreadline -lpthread -ldl -lc 

	    
libtest:
	gcc $(CC_OPTS) -c -fPIC lib$(LIBNAME).c -o lib$(LIBNAME).o
	gcc $(CC_OPTS) -shared -Wl,-soname,lib$(LIBNAME).so -o lib$(LIBNAME).so lib$(LIBNAME).o
	gcc $(CC_OPTS) -o lib$(LIBNAME) lib$(LIBNAME).c
	