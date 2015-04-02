#ifndef HOSTDS
#define HOSTDS

// INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

// DEFINES

#define NUM_PRINTERS 2
#define NUM_SCANNERS 1
#define NUM_MODEMS 1
#define NUM_DRIVES 2
#define TOTAL_MEM 1024
#define TOTAL_RTMEM 64

#define TIME_QUANTUM 1

#define INITIALIZED 0
#define RUNNING 1
#define PAUSED 2
#define TERMINATED 3

// STRUCT DEFINITIONS
struct host {
	int numPrinters;
	int numScanners;
	int numModems;
	int numDrives;
	int totalMem;

	int printers[NUM_PRINTERS];
	int scanners[NUM_SCANNERS];
	int modems[NUM_MODEMS];
	int drives[NUM_DRIVES];
	int memory[TOTAL_MEM];

	int rtMemory;
	int rtMemoryUsed;

	struct pcb *currentProcess;
};

struct pcb {
	int pid;
	int priority;
	int arrivalTime;
	int remainingTime;
	int remainingTimeSplice;

	int state;

	struct pcbres *res;
};

struct pcbres {
	// TODO
};

struct queue {
	int header;

	struct queue *next;
	struct pcb *process;
};

// FUNCTION PROTOTYPES

void initSys(void);
void processCycle(void);
int updateDispatcher(int time);

void initQueue(struct queue *queue);
void enqueue(struct queue *queue, struct pcb *currentProcess);
struct queue* dequeue(struct queue **header);

#endif
