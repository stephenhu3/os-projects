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
#define NUM_DATA 8
#define TOTAL_MEM 1024
#define TOTAL_RTMEM 64

#define TIME_QUANTUM 1

#define INITIALIZED 0
#define RUNNING 1
#define PAUSED 2
#define TERMINATED 3
#define DELIMINATOR ", "

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

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

struct pcb{
	int pid;
	int priority;
	int arrivalTime;
	int started;
	int remainingTime;
	int maxTimeAllotted;

	struct pcbres *res;
}; 

struct pcbres{
	int printersNeeded;
	int scannersNeeded;
	int modemsNeeded;
	int drivesNeeded;
	int memNeeded;

	int printersHave;
	int scannersHave;
	int modemsHave;
	int drivesHave;
	int memHave;

	int printersAllocIndex;
	int scannersAllocIndex;
	int modemsAllocIndex;
	int drivesAllocIndex;
	int memAllocIndex;
};

struct queue{
	int header;

	struct queue *next;
	struct pcb *process;
};

// FUNCTION PROTOTYPES

void initSys(void);
void processCycle(void);
int updateDispatcher(int arrival, int priority, int duration, int memsize, 
	int printers, int scanners, int modems, int drives);
void runDispatcher(int currentTime);
int runUser(void);

int isEmpty(struct queue *queue);
void initQueue(struct queue **init);
void enqueue(struct queue *queue, struct pcb *currentProcess);
void priorityEnqueue(struct pcb *currentProcess);
struct queue* dequeue(struct queue **header);

int checkRes(int pid, int resNeeded, int haveRes, int *hostRes);
void freeHostRes(struct pcb *process);

int allocPrinters(struct pcb *process);
int allocScanners(struct pcb *process);
int allocModems(struct pcb *process);
int allocDrives(struct pcb *process);
int allocMem(struct pcb *process);
int allocRTMem(struct pcb *process);
int allocRes(struct pcb *process);

#endif
