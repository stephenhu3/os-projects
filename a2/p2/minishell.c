#include "minishell.h"

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
int parsePath(char **);
char *lookupPath(char **, char **);

int main(int argc, char *argv[]) {
	int i, chSuccess;
	int pid, numChildren, parseRe;
	int status;
	char cmdLine[MAX_LINE_LEN]; 
	char pathBuffer[MAX_PATH_LEN];
	struct command_t command;

	char *commandLine = (char *) malloc(MAX_LINE_LEN);
	char **directories = (char **) malloc(sizeof(char)*MAX_PATHS);
	
	while (TRUE) { 
		printPrompt();
		
		//  Read the command line and parse it  
		readCommand(commandLine);
		parseRe = parseCommand(commandLine, &command); 
		
		if(parseRe == 1) {
			// runInBackground check
			if ((strcmp(command.argv[command.argc], "&")) != 0) {
				command.runInBackground = 0;
				// printf("Run in background: FALSE...\n");
				// printf("%i \n", strcmp(command.argv[command.argc - 1], "&"));
				// printf("%s \n", command.argv[command.argc]);
			}
			else {
				command.argv[command.argc] = NULL;

				command.runInBackground = 1;
				// printf("Run in background: TRUE...\n");
			}
		}
		parsePath(directories);

		/* Get the full pathname for the file */
		command.name = lookupPath(command.argv, directories);

		char *argument = (char *) malloc(MAX_LINE_LEN);
		argument = command.argv[1];
		// printf("command.argv[1] %s\n", command.argv[1]);
		// printf("command.name %s\n", command.name);
		// printf("command.argv %s\n", *command.argv);	

		if (strcmp(command.argv[0], "quit") == 0 || strcmp(command.argv[0], "exit") == 0) {
			printf("Closing program.\n");
			break;
		}
	

		// printf("command.name %s\n", command.name);
		// printf("command.argv %s\n", command.argv[0]);

		 // Create child process and execute the command 
		 // if the command is found in one of the directories
		// if (command.runInBackground == 1 && command.name == NULL) {
		// 	command.name = "not null";
		// }

		if((pid = fork()) == 0) {


			/* Child executing command */ 
			if (strcmp(command.argv[0], "echo") == 0) { 
				printf("I'm a child process running echo\n");
				char echoStr[CHAR_MAX];
			    // Generate string of form "echo str"
			    sprintf(echoStr, "echo %s", command.argv[1]);
			    int i;
			    for (i = 2; command.argv[i] != NULL; i++) {
			    	strcat(echoStr, " ");
			    	strcat(echoStr, command.argv[i]);
			    }
			    system(echoStr);
			    command.name = "yes";
			}
		 	else if (strcmp(command.argv[0], "cd") == 0) { 
				printf("I'm a child process running cd\n");
				char cdStr[CHAR_MAX];
			    // Generate string of form "cd str"
			    sprintf(cdStr, "%s", command.argv[1]);
			    int i;
			    for (i = 2; command.argv[i] != NULL; i++) {
			    	strcat(cdStr, " ");
			    	strcat(cdStr, command.argv[i]);
			    }
			    chSuccess = chdir(cdStr);
			    command.name = "yes"; // to avoid the invalid command name

			    if (chSuccess == 0 || cdStr[0] == NULL) {
			    	system("pwd");
				// printf("%s\n", cdStr);
				} else {
					printf("No such file or directory.\n");
					// printf("%s\n", cdStr);
				} 
			}
			else if (command.name[0] == '/') { 
				printf("I'm a child process running a command!\n");
				execv(command.name, command.argv);
			}
			} else {
				if (command.runInBackground == 1)
					printf("I'm a parent process. My child is running in background!\n");
			}

		if (command.name == NULL) {
			/* Report error */
			printf("Invalid command name.\n"); 
			continue; // breaks out of loop, back to printprompt
		}
		
		/* Wait for the child to terminate */
		if(command.runInBackground == 0) {
			printf("I'm a parent process. I am waiting for my child to finish!\n");
			wait(&status);
		}

		
	}
	free(commandLine);
	free(directories);
	
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
	// note, whitespace is represented as , or .
	while((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
		//debugging
		// printf("argv[%d]: %s\n", argc, cmd->argv[argc]);
		cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
	}

	/* Set the command name and argc */ 
	cmd->argc = argc - 1;
	cmd->name = (char *) malloc(sizeof(cmd->argv[0])); 
	strcpy(cmd->name, cmd->argv[0]);
	int i;
	// for (i = 0; i < cmd->argc; i++)
	// 	printf("%s", cmd->argv[i]);
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

// may need to modify so arguments are saved as well
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
	// fprintf(stderr, "%s: command not found\n", argv[0]); 
	return NULL;
}
