#include "libtest.h"

int main(int argc, char **argv) {
	st_test v = struct_test(atoi(argv[1]), argv[2]);
	printf("st_test {id: %d, string: \"%s\"}\n", v.id, v.string);
	return 0;
}

st_test struct_test(int id, char *string) {
	st_test i;
	i.id = id;

	// allocate mem for a copy of string
	char *append = " -> end";
	char *str = (char*) malloc(sizeof(char) *
			        (strlen(string) + strlen(append) + 1));
	strcpy(str, string);
	strcat(str, append);

	i.string = str;
	return i;
}
