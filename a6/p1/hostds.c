#include "hostds.h"

// Globals
struct host host;
struct queue *dispatcher, *RTQueue, *userQueue, *p1Queue, *p2Queue, *p3Queue;

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Error processing arguments... Please specify filename in format: ./hostds filename.txt\n");
		exit(0);
	}

	initSys();

	// printf("Printers: %i\n", host.numPrinters);
}

//PARAMS: none
//EFFECTS: Initializes the host with relevant items
//RETURNS: none
void initSys(void) {
	// initialize constants
	host.numPrinters = NUM_PRINTERS;
	host.numScanners = NUM_SCANNERS;
	host.numModems = NUM_MODEMS;
	host.numDrives = NUM_DRIVES;
	host.totalMem = TOTAL_MEM;
	host.rtMemory = TOTAL_RTMEM;
	host.rtMemoryUsed = 0;
	
	// sets array members to 0
	memset(host.printers, 0, NUM_PRINTERS);
	memset(host.scanners, 0, NUM_SCANNERS);
	memset(host.modems, 0, NUM_MODEMS);
	memset(host.drives, 0, NUM_DRIVES);
	memset(host.memory, 0, TOTAL_MEM);

	// initializes queues
	initQueue(dispatcher);
	initQueue(RTQueue);
	initQueue(userQueue);
	initQueue(p1Queue);
	initQueue(p2Queue);
	initQueue(p3Queue);

}

//PARAMS:
//EFFECTS: simulates one processing cycle
//RETURNS:
void processCycle(void) {
	// TODO
}


//PARAMS: current time quantum
//EFFECTS: update the dispatcher
//RETURNS: 1 for process put into real time queue else 0
void updateDispatcher(int time) {
	// TODO
}

//PARAMS: valid uninitialized queue
//EFFECTS: initializes queue and elements within
//RETURNS: initialized queue
void initQueue(struct queue *queue) {
	queue = malloc(sizeof(queue));

	queue->header = 0;
	queue->next = NULL;
	queue->process = NULL;
}


