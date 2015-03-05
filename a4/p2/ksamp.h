#ifndef KSAMP_H
#define KSAMP_H

// INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

// DEFINES
#define VER1 1
#define VER2 2
#define VER3 3

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

// STRUCT DEFINITIONSS

// FUNCTION PROTOTYPES
int getMode(int argc, char *argv[]);
void getProcType(void);
void getKernalVersion(void);
void getTLastBoot(void);
void getTStats(void);
void getDiskStats(void);
void getMiscStats(void);

#endif
