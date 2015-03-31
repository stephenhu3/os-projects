#ifndef HOSTDS
#define HOSTDS

// INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// DEFINES

#define NUM_PRINTERS 2
#define NUM_SCANNERS 1
#define NUM_MODEMS 1
#define NUM_DRIVES 2
#define TOTAL_MEM 1024
#define TOTAL_RTMEM 64

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
};

struct pcb {
	// TODO
};

struct pcbres {
	// TODO
};

// FUNCTION PROTOTYPES

void initSys(void);

#endif
