#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define	MAX_LINE_LEN 	80
#define MAX_ARGS 		64
#define MAX_ARG_LEN 	16
#define MAX_PATHS 		64
#define MAX_PATH_LEN 	96
#define CHAR_MAX 		1024
#define WHITESPACE	".,\t\n"
#define TRUE			1

struct command_t {
	char *name;
	int argc;
	char *argv[MAX_ARGS];
	int runInBackground;
};
