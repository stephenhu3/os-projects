#include "hostds.h"

// Globals
struct host host;
struct queue *dispatcher, *RTQueue, *userQueue, *p1Queue, *p2Queue, *p3Queue;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Error processing arguments... Please specify filename in format: ./hostds filename.txt\n");
		exit(0);
	}

	initSys();

	// input in the form:
	// <arrival time>, <priority>, <processor time>, <Mbytes>, <#printers>, <#scanners>, <#modems>, <#CDs>
	// 13, 3, 6, 128, 1, 0, 1, 2
	// printf("Printers: %i\n", host.numPrinters);

	// open dispatch list for reading
	FILE *file = fopen( argv[1], "r");

	if (file == -1) {
		printf("Error reading dispatch file\n");
		return 0;	
	}

	// seek file pointer for reading from beginning
	fseek(file, 0, SEEK_SET);

	char currentLine[50];
	char *readData;
	int readValues[8];
	int i;

	while (fgets(currentLine, 50, file) != NULL) {
		// parse each line

		
		readData = strtok(currentLine, DELIMINATOR);
		
		i = 0;
		while(readData != NULL && i < NUM_DATA) {
			// fprintf(stdout, "%s\n", readData);
			readValues[i++] = atoi(readData);
			readData = strtok(NULL, DELIMINATOR);
			
		}
		/*
		readValues indices:
		0: arrival time
		1: priority
		2: processor time
		3: Mbytes
		4: #printers
		5: #scanners
		6: #modems
		7: #CDs

		use values accordingly 
		*/

		// for (i = 0; i < NUM_DATA; i++)
		// 	printf("%d\n", readValues[i]);

	}


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
int updateDispatcher(int time) {
	// TODO
}

//PARAMS: valid uninitialized queue
//EFFECTS: initializes queue and elements within
//RETURNS: none
void initQueue(struct queue *queue) {
	queue = malloc(sizeof(queue));

	queue->header = 0;
	queue->next = NULL;
	queue->process = NULL;
}

//PARAMS: target queue to insert process, current process to queue up
//EFFECTS: queues current process at end of given queue
//RETURNS: none
void enqueue(struct queue *queue, struct pcb *currentProcess) {
	if(!queue->header) {
		queue->process = currentProcess;
		queue->header = 1;
	}
	else {
		struct queue *cursor = queue;
		while (cursor->next)
			cursor = cursor->next;
		struct queue *thisProcess = malloc(sizeof(queue));
		cursor->next = thisProcess;
		cursor->next->header = 0;
		cursor->next->next = NULL;
		cursor->next->process = currentProcess;
	}
}


//PARAMS: pointer to queue containing header to dequeue
//EFFECTS: dequeues the header of queue
//RETURNS: dequeued header
struct queue* dequeue(struct queue **header) {
	if(!((*header)->process))
		return NULL;

	struct queue *dequeuedHeader = *header;

	if((*header)->next == NULL)
		*header = NULL;
	else
		*header = (*header)->next;

	return dequeuedHeader;
}
