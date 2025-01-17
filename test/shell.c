// $Id: shell.c 49 2013-03-02 11:58:04Z wunderlins $

/**
 * interactive shell to pm
 * 
 * this utility requires libreadline-dev to be installed for compilation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#define PROMPT "pm > "
 
int main() {
	char* input;

	// Configure readline to auto-complete paths when the tab key is hit.
	rl_bind_key('\t', rl_complete);
	
	printf("$Revision: 49 $\nCtrl-D to exit\n");

	for(;;) {

		// Display prompt and read input (n.b. input must be freed after use)...
		input = readline(PROMPT);

		// Check for EOF.
		if (!input) {
			printf("\n");
			break;
		}

		// Add input to history.
		add_history(input);

		// Do stuff...

		// Free input.
		free(input);
	}
}
