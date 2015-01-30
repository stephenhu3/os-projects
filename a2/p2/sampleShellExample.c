/* This is a very minimal shell. It finds an executable in the PATH, then loads it and executes it (using execv). Since it uses “.” (dot) as a separator, it cannot handle file names like “minishell.h”. */
#include ...
#include “minishell.h”
char *lookupPath(char **, char **);
int parseCommand(char *, struct command_t *); 
int parsePath(char **);
void printPrompt();
void readCommand(char *);
...

int main() {
	...
	/* Shell initialization */ 
	...

	parsePath(pathv); /* Get directory paths from PATH */
	
	while (TRUE) { 
		printPrompt();
		
		/* Read the command line and parse it */ 
		readCommand(commandLine);
		...
		parseCommand(commandLine, &command); 
		...
		
		/* Get the full pathname for the file */
		command.name = lookupPath(command.argv, pathv); 
		if(command.name == NULL) {
			/* Report error */ 
			continue;
		}

		/* Create child and execute the command */ 
		...
		
		/* Wait for the child to terminate */
		... 
	}
	
	/* Shell termination */ 
	...
}