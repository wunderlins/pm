// make sure to compile with -lreadline

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(){
	
	//FILE* fd = fdopen(stdin, 'r');
	// char* buf;
	// int s = 5;
	
	char* buf = readline("input: ");
	
	//fclose(fd);
	
	printf("output: %s\n", buf);
	
	return 0;
}
