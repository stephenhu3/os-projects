#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NPHILOSOPHERS 5
#define NMEALS 3
#define ONESEC_PERIOD 1000000 // one second
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

void *initializePhilosopher(int id);
void initializeFork(int id);
int getLeftFork(int id);
int getRightFork(int id);
int pickupFork(int id);
void returnForks(int id);
void nap(void);
