#include "minishell.h"

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
int parsePath(char **);
char *lookupPath(char **, char **);

int main(int argc, char *argv[]) {
	int i;
	int pid, numChildren;
	int status;
	char cmdLine[MAX_LINE_LEN]; 
	struct command_t command;

	char *commandLine = (char *) malloc(MAX_LINE_LEN);
	char **directories = (char **) malloc(sizeof(char)*MAX_PATHS);
	
	while (TRUE) { 
		printPrompt();
		
		//  Read the command line and parse it  
		readCommand(commandLine);
		parseCommand(commandLine, &command); 
		
		parsePath(directories);

		/* Get the full pathname for the file */
		command.name = lookupPath(command.argv, directories);
		// printf("command.name %s\n", command.name);
		// printf("command.argv %s\n", *command.argv);	

		if (strcmp(command.argv[0], "quit") == 0 || strcmp(command.argv[0], "exit") == 0) {
			printf("Closing program.\n");
			break;
		}
		
		// if command not found in any directories in PATH variable
		if(command.name == NULL) {
			/* Report error */ 
			printf("Invalid command name.\n"); 
			continue; // breaks out of loop, back to printprompt
		}

		// printf("command.name %s\n", command.name);
		// printf("command.argv %s\n", command.argv[0]);
		// execv(command.name, command.argv);

		 // Create child process and execute the command 
		 // if the command is found in one of the directories
		if((pid = fork()) == 0) {
			/* Child executing command */ 
			
			printf("I'm a child process with command name: %s\n", command.name);

			if(command.name[0] == '/') { 
				// echo, pwd, ls, etc are all running through here because the file is found
				// printf("command.name %s\n", command.name);
				printf("command.argv: %s\n", command.argv[0]);
				if (strcmp(command.argv[0], "echo") == 0) { // implement echo
					printf("this is echo \n");
					char echoStr[CHAR_MAX];
				    // Generate string of form "echo str"
				    sprintf(echoStr, "echo %s", argv[1]);
				    system(echoStr);
				} else if (strcmp(command.argv[0], "cd") == 0) { // implement cd
					printf("this is cd \n");
				}
			//

				execv(command.name, command.argv);
			} else if (strcmp(command.argv[0], "echo") == 0) { // implement echo
				printf("this is echo");
				// execv
			} else if (strcmp(command.argv[0], "cd") == 0) { // implement cd
				printf("this is cd");
			// execv
			} else { // ls and pwd work here already
				// this never gets executed since if it's not "/", it doesnt go in this loop at all
				// printf("command.name %s\n", command.name);
				// printf("command.argv %s\n", command.argv[0]);
				printf("Executing command: %s\n", *command.argv);
				execv(lookupPath(*directories, command.argv), command.argv);
			}
		}

		
		/* Wait for the child to terminate */
		wait(&status);
	}
	
	/* Shell termination */
	printf("Terminating successfully.\n"); 
	return 0;
}

/* Determine command name and construct the parameter list. 
This function will build argv[] and set the argc value.
argc is the number of "tokens" or words on the command line and argv[] 
is an array of strings (pointers to char *). The last element in argv[]
must be NULL. As we scan the command line from the left, the first token 
goes in argv[0], the second in argv[l], and so on. Each time we add a token 
to argv[], we increment argc.
*/
int parseCommand(char *cLine, struct command_t *cmd) { 
	int argc;
	char **clPtr;
	/* Initialization */
	clPtr = &cLine; /* cLine is the command line */ 
	argc = 0;
	cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);

	/* Fill argv[] */
	while((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
		cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
	}

	/* Set the command name and argc */ 
	cmd->argc = argc - 1;
	cmd->name = (char *) malloc(sizeof(cmd->argv[0])); 
	strcpy(cmd->name, cmd->argv[0]);
	// printf("%s", cmd->argv[0]);
	return 1;
}

void printPrompt() {
	/* Build the prompt string to have the machine name, current directory, 
	or other desired information */
    char promptString[CHAR_MAX];
    strcpy(promptString, "");
    char hostname[CHAR_MAX];
    char *cwd = (char *)malloc(MAX_ARG_LEN);

    gethostname(hostname, sizeof(hostname));
    getcwd(cwd, MAX_ARG_LEN);

	strcat(promptString, getenv("USER"));
	strcat(promptString, "@");
	strcat(promptString, hostname);
	strcat(promptString, ":");
	strcat(promptString, cwd);
	printf ("%s", promptString);
	free(cwd);
	
	fflush(stdout);
}

void readCommand(char *buffer) {
	/* This code uses any set of I/O functions, such as those in the 
	stdio library to read the entire command line into the buffer. 
	This implementation is greatly simplified, but it does the job. */
	gets (buffer);
}

int parsePath(char **dirs) {
	/* This function reads the PATH variable for this environment, 
	then builds an array, dirs[], of the directories in PATH */
	char *pathEnvVar; 
	char *thePath;

	const char delimiter[2] = ":";

    int i;
	
	while(*dirs++) {
		*dirs = NULL; 
	} 
	
	pathEnvVar = (char *) getenv ("PATH"); 
	thePath = (char *) malloc(strlen(pathEnvVar) + 1);
	strcpy(thePath, pathEnvVar);
	

	// printf("%s\n", thePath);

   char *token = (char *) malloc(strlen(pathEnvVar));
   
   /* get first token */
   token = strtok(thePath, delimiter);

   // printf("%s\n", token);
   
   /* get other tokens */
   while( token != NULL ) 
	{
	  token = strtok(NULL, delimiter);
	  // printf("%s\n", token);
	  *dirs = token;
	  dirs++;
   }

   return 1;
}

char *lookupPath(char **argv, char **dir) {
	/* This function searches the directories identified by 
	the dir argument to see if argv[0] (the file name) appears there.
	Allocate a new string, place the full path name in it, 
	then return the string. */
	int i;
	char *result;
	char pName[MAX_PATH_LEN];
	int pathSize = sizeof(*dir)/sizeof(char);//double check this

	// printf("size of dir %d", pathSize);
	char filepath[CHAR_MAX];
	strcpy(filepath, "");


	// printf(argv[0]);
	/* Check to see if file name is already an absolute path name */ 
	if(*argv[0] == '/') {
		return argv[0]; 
	}



	/* Look in PATH directories
	   use access() to see if the file is in a dir */
	for (i = 1; i < pathSize; i++) {
		// printf("dir: %s \n", *dir);
		dir++;
		strcat(filepath, *dir);
		strcat(filepath, "/");
		strcat(filepath, argv[0]);
		

		if (access(filepath, F_OK) == 0) {
			printf("Found valid filepath: %s\n", filepath);
			return filepath;
		}

		// printf("filepath: %s\n", filepath); // extremely useful for debugging, prints the full filepath 
		memset(filepath, 0, CHAR_MAX); //clear the char array
		char filepath[CHAR_MAX] = "";

	}


	// /* if file name not found in any path variable till now then*/ 
	fprintf(stderr, "%s: command not found\n", argv[0]); 
	return NULL;
}
