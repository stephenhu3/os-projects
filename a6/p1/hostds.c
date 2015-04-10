#include "hostds.h"

// Globals
struct host host;
struct queue *dispatcher;
struct queue *RTQueue;
struct queue *userQueue;
struct queue *p1Queue;
struct queue *p2Queue;
struct queue *p3Queue;
int pid = 1;
int currentTime = 1;

/*

OUTPUT SCHEME:
Time 1:
Process A + Information
Time 2:
Process A + Information
Time 3:
Process A + Information
Time 4:
Process B + Information
Time 5:
Process C + Information
Time 6:
Process A + Information

*/

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Error processing arguments... Please specify filename in format: ./hostds filename.txt\n");
		exit(0);
	}

	// input in the form:
	// <arrival time>, <priority>, <processor time>, <Mbytes>, <#printers>, <#scanners>, <#modems>, <#CDs>

	// open dispatch list for reading
	FILE *file = fopen( argv[1], "r");

	if ((int) file == -1) {
		printf("Error reading dispatch file\n");
		return -1;	
	}

	// seek file pointer for reading from beginning
	fseek(file, 0, SEEK_SET);

	char currentLine[50];
	char *readData;
	int readValues[8];
	int i;
	initSys();
	
	while (fgets(currentLine, 50, file) != NULL) {
		// parse each line
		readData = strtok(currentLine, DELIMINATOR);
		
		i = 0;
		while(readData != NULL && i < NUM_DATA) {
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

		// For debugging purposes
		// for (i = 0; i < NUM_DATA; i++)
		// printf("%d\n", readValues[i]);

		// add to appropriate user or real time queue
		updateDispatcher(readValues[0], readValues[1], readValues[2],
			readValues[3], readValues[4], readValues[5], readValues[6], readValues[7]);

	}

	// processCycle calls runDispatcher
	while(!isEmpty(dispatcher) || host.currentProcess != NULL || !isEmpty(RTQueue) || !isEmpty(p1Queue) || !isEmpty(p2Queue) || !isEmpty(p3Queue)) {
	 	processCycle();
	}

	free(p1Queue);
	free(p2Queue);
	free(p3Queue);
	free(RTQueue);
	free(userQueue);
	free(dispatcher);

	return 0;
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
	
	// initializes components
	memset(host.printers, 0, NUM_PRINTERS);
	memset(host.scanners, 0, NUM_SCANNERS);
	memset(host.modems, 0, NUM_MODEMS);
	memset(host.drives, 0, NUM_DRIVES);
	memset(host.memory, 0, TOTAL_MEM);

	// initializes queues
	initQueue(&dispatcher);
	initQueue(&RTQueue);
	initQueue(&userQueue);
	initQueue(&p1Queue);
	initQueue(&p2Queue);
	initQueue(&p3Queue);
}

//PARAMS: valid uninitialized queue
//EFFECTS: initializes queue and elements within
//RETURNS: none
void initQueue(struct queue **init) {
	struct queue *new;
	new = (struct queue *) malloc(sizeof(struct queue));
	new->header = 0;
	new->next = NULL;
	new->process = NULL;
	*init = new;
}

//PARAMS: process with priority != 0 to be reenqueued
//EFFECTS: enqueues process depending on priority ranged 1 to 3 inclusive
//RETURNS: none
void priorityEnqueue(struct pcb *currentProcess) {
	if(currentProcess->priority == 1)
		enqueue(p1Queue, currentProcess);
	else if(currentProcess->priority == 2)
		enqueue(p2Queue, currentProcess);
	else
		enqueue(p3Queue, currentProcess);
}

//PARAMS: run once per time unit
//EFFECTS: simulates one processing cycle, increments global time
//RETURNS: none
void processCycle(void) {
	printf("|------------------------------------------------|\n");
	printf("              PROCESSOR CYCLE #%i                \n", currentTime);
	printf("|------------------------------------------------|\n");
	
	runDispatcher(currentTime);
	runUser();
	
	// Real Time Handler
	if(!isEmpty(RTQueue)) {
		if(host.currentProcess == NULL) {
			host.currentProcess = RTQueue->process;
			dequeue(&RTQueue);
		}
		else if(host.currentProcess->priority != 0) {
			printf(RED "Process %d: Suspended (Remaining Time: %d) | Current Time: %d\n" RESET, host.currentProcess->pid, host.currentProcess->remainingTime, currentTime);
			priorityEnqueue(host.currentProcess);
			host.currentProcess = RTQueue->process;
			dequeue(&RTQueue);
		}
	}
	// Priority 1 Handler
	else if(!isEmpty(p1Queue)) {
		if(host.currentProcess == NULL) {
			host.currentProcess = p1Queue->process;
			dequeue(&p1Queue);
		}
		else if(host.currentProcess->priority > 1) {
			printf(RED "Process %d: Suspended (Remaining Time: %d) | Current Time: %d\n" RESET, host.currentProcess->pid, host.currentProcess->remainingTime, currentTime);
			priorityEnqueue(host.currentProcess);
			host.currentProcess = p1Queue->process;
			dequeue(&p1Queue);
		}
	}
	// Priority 2 Handler
	else if(!isEmpty(p2Queue)) {
		if(host.currentProcess == NULL) {
			host.currentProcess = p2Queue->process;
			dequeue(&p2Queue);
		}
		else if(host.currentProcess->priority > 2) {
			printf(RED "Process %d: Suspended (Remaining Time: %d) | Current Time: %d\n" RESET, host.currentProcess->pid, host.currentProcess->remainingTime, currentTime);
			priorityEnqueue(host.currentProcess);
			host.currentProcess = p2Queue->process;
			dequeue(&p2Queue);
		}
	}
	// Priority 3 Handler
	else if(!isEmpty(p3Queue)) {
		if(host.currentProcess == NULL) {
			host.currentProcess = p3Queue->process;
			dequeue(&p3Queue);
		}
	}

	if(host.currentProcess != NULL) {
		if (host.currentProcess->started != 1) {
			host.currentProcess->started = 1;

			printf(GREEN "Process %d: Started (Remaining Time: %d) | Current Time: %d\n" RESET, host.currentProcess->pid, host.currentProcess->remainingTime, currentTime);
			host.currentProcess->maxTimeAllotted = 20;

		} else if (host.currentProcess->maxTimeAllotted <= 1) {
			printf(MAGENTA "Process %d: Self-terminated after running for 20 cycles\n" RESET, host.currentProcess->pid);
			freeHostRes(host.currentProcess);
			host.currentProcess = NULL;
			currentTime++;
			printf("\n");
			return;

		} else {
			printf(CYAN "Process %d: Continued (Remaining Time: %d) | Current Time: %d\n" RESET, host.currentProcess->pid, host.currentProcess->remainingTime, currentTime);
			host.currentProcess->maxTimeAllotted--;
		}

		host.currentProcess->remainingTime--;

		if(host.currentProcess->remainingTime <= 0) {
			printf(MAGENTA "Process %d: Terminated\n" RESET, host.currentProcess->pid);
			freeHostRes(host.currentProcess);
			host.currentProcess = NULL;
		}
	}
	else
		printf(YELLOW "Idling... Current Time: %i\n" RESET, currentTime);
	printf("\n");
	
	currentTime++;
}


//PARAMS: current time quantum
//EFFECTS: update the dispatcher
//RETURNS: 1 for process put into real time queue, 0 for user queue, -1 if none
int updateDispatcher(int arrival, int priority, int duration, int memsize, 
	int printers, int scanners, int modems, int drives) {
	// Set needed resources
	struct pcbres *resources = (struct pcbres *) malloc(sizeof(struct pcbres));
	resources->printersNeeded = printers;
	resources->scannersNeeded = scanners;
	resources->modemsNeeded = modems;
	resources->drivesNeeded = drives;
	resources->memNeeded = memsize;

	// Set process information
	struct pcb *process = (struct pcb *) malloc(sizeof(struct pcb));
	int currentPID = pid++;
	process->pid = currentPID;
	process->priority = priority;
	process->arrivalTime = arrival;
	process->remainingTime = duration;
	process->res = resources;

	enqueue(dispatcher, process);
}

//PARAMS: current simulated time quantum
//EFFECTS: runs the dispatcher, enqueuing or holding as appropriate
//RETURNS: none
void runDispatcher(int currentTime) {
	// our return value for whether or not the realtime queue was updated
	int rtIntercept = 0;
	int i, count, allocRet;

	// hold the dispatcher in queue that we can modify with impunity
	struct queue *holder = dispatcher;

	// if elements were added, leftover elements get pushed to this queue which will be the new dispatcher
	struct queue *updatedDispatcher;
	initQueue(&updatedDispatcher);

	if(isEmpty(dispatcher) == 1) {
		return;
	}

	// count elements in dispatcher
	for(count = 1; holder->next != NULL; count++) {
		holder = holder->next;
	}

	// reset after counting
	holder = dispatcher;

	/*
	Any User priority jobs in the User job queue that can run within available resources 
	(memory and i/o devices) are transferred to the appropriate priority queue.
	Normal operation of a feedback queue will accept all jobs at the highest priority level 
	and degrade the priority after each completed time quantum. However, this dispatcher has 
	the ability to accept jobs at a lower priority, inserting them in the appropriate queue.
	This enables the dispatcher to emulate a simple Round Robin dispatcher if all jobs are
	accepted at the lowest priority.
	*/
	for(i = 0; i < count; i++) {
		// grab process
		struct pcb *process = dequeue(&holder)->process;
		
		// handle arrival time
		if(currentTime >= process->arrivalTime) {
			allocRet = allocRes(process);
			if(allocRet == 1 || allocRet == 0) {
				printf(BLUE "Allocated Process PID: %i\n" RESET, process->pid);
				if(process->priority == 0)
					rtIntercept = 1;
				switch(process->priority) {
				case 0:
					enqueue(RTQueue, process);
					break;
				case 1: // if priority is either 1, 2, or 3, enqueue to user queue
				case 2:
				case 3:
					enqueue(userQueue, process);
					break;
				default:
					break;
			}
			}
			else // can't allocate resource so put it back
				enqueue(updatedDispatcher, process);
		}
		else // hasn't arrived yet so put it back
			enqueue(updatedDispatcher, process);
	}

	while(!isEmpty(dispatcher)) {
		holder = dispatcher->next;
		free(dispatcher->process->res);
		free(dispatcher->process);
		free(dispatcher);
		dispatcher = holder;
	}

	// update dispatcher with leftover processes that could not be run
	dispatcher = updatedDispatcher;
}

//PARAMS: none
//EFFECTS: runs the user queue, enqueuing or holding as appropriate
//RETURNS: 1 if user queue enqueued process to a priority queue, else 0
int runUser(void) {
	// our return value for whether or not the realtime queue was updated
	struct queue *userCurrent;
		if (!isEmpty(userQueue)) {
			userCurrent = dequeue(&userQueue);	
			if (userCurrent != NULL) {
				switch(userCurrent->process->priority) {
					case 1:
						enqueue(p1Queue, userCurrent->process);
						return 1;
					case 2:
						enqueue(p2Queue, userCurrent->process);
						return 1;
					case 3:
						enqueue(p3Queue, userCurrent->process);
						return 1;
					default:
					// invalid priority, nothing enqueued
					return 0;
				}
			}
		}
	return 0;
}

//PARAMS: valid queue's head
//EFFECTS: none
//RETURNS: 1 if queue empty, 0 otherwise
int isEmpty(struct queue *queue) {
	if (queue == NULL)
		return 1;
	return queue->header == 0 ? 1 : 0;
}

//PARAMS: target queue to insert process, current process to queue up
//EFFECTS: queues current process at end of given queue
//RETURNS: none
void enqueue(struct queue *target, struct pcb *currentProcess) {
		if(target->header == 0) {
			target->process = currentProcess;
			target->header = 1;
			// added element is the head, single element of queue
		}
		else {
			struct queue *cursor = target;
			while (cursor->next != NULL)
				cursor = cursor->next;
			struct queue *newQueue = (struct queue *) malloc(sizeof(struct queue));
			newQueue->header = 0;
			newQueue->next = NULL;
			newQueue->process = currentProcess;
			cursor->next = newQueue;
		}
}

//PARAMS: pointer to queue containing header to dequeue
//EFFECTS: dequeues the header of queue
//RETURNS: dequeued header
struct queue* dequeue(struct queue **header) {
	if(((*header)->process) == NULL)
		return NULL;

	struct queue *dequeuedHeader = (struct queue *) malloc(sizeof(struct queue));
	memcpy(dequeuedHeader, *header, sizeof(struct queue));

	struct pcb *dequeuedProcess = (struct pcb *) malloc(sizeof(struct pcb));
	memcpy(dequeuedProcess, (*header)->process, sizeof(struct pcb));	

	struct pcbres *resources = (struct pcbres *) malloc(sizeof(struct pcbres));
	memcpy(resources, (*header)->process->res, sizeof(struct pcbres));

	dequeuedProcess->res = resources;
	dequeuedHeader->process = dequeuedProcess;

	// remove current header and set new header
	if((*header)->next == NULL) { // single element case
		*header = NULL;
		initQueue(header);
	}
	else {
		*header = (*header)->next;
		// set as header
		(*header)->header = 1;
	}

	return dequeuedHeader;
}

//PARAMS: pid, resources in need of allocation, resources present on host, host resource pool
//EFFECTS: attempts to allocate resources from host as needed
//RETURNS: index within host resource pool available for allocation, -1 for not enough resources
int checkRes(int pid, int resNeeded, int haveRes, int *hostRes) {
	int i, count = 0, cursor = -1;

	for(i = 0; i < haveRes; i++) {
		if(hostRes[i] == 0) {
			if(cursor == -1)
				cursor = i;
			count++;
			
			if(count == resNeeded)
				break;
		}
		else if(hostRes[i] == pid)
			return -1;
		else {
			cursor = -1;
			count = 0;
		}
	}

	if(count == resNeeded)
		return cursor;
	else
		return -1;
}

//PARAMS: process
//EFFECTS: frees host resources allocated to process
//RETURNS: none
void freeHostRes(struct pcb *process) {
	int i;
	
	if(process->res->printersAllocIndex != -1)
		for(i = process->res->printersAllocIndex; i < process->res->printersAllocIndex + process->res->printersHave; i++)
			host.printers[i] = 0;

	if(process->res->scannersAllocIndex != -1)
		for(i = process->res->scannersAllocIndex; i < process->res->scannersAllocIndex + process->res->scannersHave; i++)
			host.scanners[i] = 0;

	if(process->res->modemsAllocIndex != -1)
		for(i = process->res->modemsAllocIndex; i < process->res->modemsAllocIndex + process->res->modemsHave; i++)
			host.modems[i] = 0;

	if(process->res->drivesAllocIndex != -1)
		for(i = process->res->drivesAllocIndex; i < process->res->drivesAllocIndex + process->res->drivesHave; i++)
			host.drives[i] = 0;

	if(process->res->memAllocIndex != -1)
		for(i = process->res->memAllocIndex; i < process->res->memAllocIndex + process->res->memHave; i++)
			host.memory[i] = 0;	
}

//PARAMS: process
//EFFECTS: allocates host printers to process
//RETURNS: 1 for sucess, 0 for failure
int allocPrinters(struct pcb *process) {
	if(process->res->printersNeeded == 0)
		return 1;

	int i, cursor = checkRes(process->pid, process->res->printersNeeded, host.numPrinters, host.printers);

	if(cursor != -1) {
		for(i = cursor; i < cursor + process->res->printersNeeded; i++)
			host.printers[i] = process->pid;

		process->res->printersHave = process->res->printersNeeded;
		process->res->printersAllocIndex = cursor;

		return 1;
	}

	return 0;
}

//PARAMS: process
//EFFECTS: allocates host scanners to process
//RETURNS: 1 for sucess, 0 for failure
int allocScanners(struct pcb *process) {
	if(process->res->scannersNeeded == 0)
		return 1;

	int i, cursor = checkRes(process->pid, process->res->scannersNeeded, host.numScanners, host.scanners);

	if(cursor != -1) {
		for(i = cursor; i < cursor + process->res->scannersNeeded; i++)
			host.scanners[i] = process->pid;

		process->res->scannersHave = process->res->scannersNeeded;
		process->res->scannersAllocIndex = cursor;

		return 1;
	}

	return 0;
}

//PARAMS: process
//EFFECTS: allocates host modems to process
//RETURNS: 1 for success, 0 for failure
int allocModems(struct pcb *process) {
	if(process->res->modemsNeeded == 0)
		return 1;

	int i, cursor = checkRes(process->pid, process->res->modemsNeeded, host.numModems, host.modems);

	if(cursor != -1) {
		for(i = cursor; i < cursor + process->res->modemsNeeded; i++)
			host.modems[i] = process->pid;

		process->res->modemsHave = process->res->modemsNeeded;
		process->res->modemsAllocIndex = cursor;

		return 1;
	}

	return 0;
}

//PARAMS: process
//EFFECTS: allocates host (CD) drives to process
//RETURNS: 1 for sucess, 0 for failure
int allocDrives(struct pcb *process) {
	if(process->res->drivesNeeded == 0)
		return 1;

	int i, cursor = checkRes(process->pid, process->res->drivesNeeded, host.numDrives, host.drives);

	if(cursor != -1) {
		for(i = cursor; i < cursor + process->res->drivesNeeded; i++)
			host.drives[i] = process->pid;

		process->res->drivesHave = process->res->drivesNeeded;
		process->res->drivesAllocIndex = cursor;

		return 1;
	}

	return 0;
}

//PARAMS: process
//EFFECTS: allocates host memory to process
//RETURNS: 1 for sucess, 0 for failure
int allocMem(struct pcb *process) {
	if(process->res->memNeeded == 0)
		return 1;

	int i, cursor = checkRes(process->pid, process->res->memNeeded, host.totalMem, host.memory);

	if(cursor != -1) {
		for(i = cursor; i < cursor + process->res->memNeeded; i++)
			host.memory[i] = process->pid;

		process->res->memHave = process->res->memNeeded;
		process->res->memAllocIndex = cursor;

		return 1;
	}

	return 0;
}

//PARAMS: process
//EFFECTS: allocates host real-time memory to process
//RETURNS: 1 for sucess, 0 for failure
int allocRTMem(struct pcb *process) {
	if(process->res->memNeeded == 0)
		return 1;

	int i, cursor = checkRes(process->pid, process->res->memNeeded, host.rtMemory, host.memory);

	if(cursor != -1) {
		for(i = cursor; i < cursor + process->res->memNeeded; i++)
			host.memory[i] = process->pid;

		process->res->memHave = process->res->memNeeded;
		process->res->memAllocIndex = cursor;

		return 1;
	}

	return 0;
}


//Question: should freehostres be called using this function?
//PARAMS: process
//EFFECTS: allocates relevant host resources to process
//RETURNS: 1 for success and realtime, 0 for success, -1 for failure
int allocRes(struct pcb *process) {
	int printers, scanners, modems, drives, memory;

	if(process->priority == 0) {
		memory = allocRTMem(process);
		return 1;
	}
	else if (process->priority < 4 && process->priority > 0){
		// these I/O resources only allocated to non-realtime tasks
		memory = allocMem(process);
		printers = allocPrinters(process);
		scanners = allocScanners(process);
		modems = allocModems(process);
		drives = allocDrives(process);
		return 0;
	}

	// priority was invalid, not within 0 to 3 range
	return -1;
}
