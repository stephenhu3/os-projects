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
//above all, real time queue has main priority, FCFS, if empty THEN
//{
// userQueue uses round robin scheduling scheme
// p1queue, p2queue, p3queue is basically a priority queue, if p1 empty, then go to p2, if p2 empty, then go to p3, else prioritize if not empty
//}

/*

OUTPUT:
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
	// 13, 3, 6, 128, 1, 0, 1, 2
	// 12, 2, 4, 128, 1, 0, 1, 2
	// 12, 3, 3, 128, 1, 0, 1, 2
	// printf("Printers: %i\n", host.numPrinters);

	// open dispatch list for reading
	FILE *file = fopen( argv[1], "r");

	if ((int) file == -1) {
		printf("Error reading dispatch file\n");
		return 0;	
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

		// For debugging purposes
		// for (i = 0; i < NUM_DATA; i++)
		// printf("%d\n", readValues[i]);
		

		// add to appropriate user or real time queue
		updateDispatcher(readValues[0], readValues[1], readValues[2],
			readValues[3], readValues[4], readValues[5], readValues[6], readValues[7]);

	}



	runDispatcher(currentTime);
	// process cycle calls run dispatcher already
	while(isEmpty(dispatcher) == 0) {
		processCycle();
	}



	// for (int i = 0; i < 11; i++) {
	// 	processCycle();
	// }
	// contents of queues
	// head of RTQueue
	// printf("Head of RT, PID: %d", RTQueue->process->pid);
	// printf("Second of RT, PID: %d", RTQueue->next->process->pid);
	// // is empty, so run 1 last time
	// runDispatcher(currentTime);
	// processCycle();

}

//PARAMS: none
//EFFECTS: Initializes the host with relevant items
//RETURNS: none
void initSys(void) { // No crashes
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
	new = malloc(sizeof(struct queue));
	new->header = 0;
	new->next = NULL;
	new->process = NULL;
	*init = new;
}

//PARAMS: run once per time unit
//EFFECTS: simulates one processing cycle, increments global time
//RETURNS: none
void processCycle(void) {
	// TODO: logic for time remaining, scheduling logic for user queue, fcfs for priority

	// Ambiguities: can we dispatch from user queue to priority queues in same cycle with dispatching from priority queue?
	// Current implementation: can only start dispatching from user queue to priority queue if real time queue empty
	// If real time queue empty, start processing user queue

	//TODO: process cycle should only be called at most once per second, use timing functions to ensure this
	// runDispatcher(currentTime);

	if (isEmpty(RTQueue)) {
		// Dispatch from user queue to appropriate priority queues
		runUser();
		// Run process from priority queues
		if (isEmpty(p1Queue)) {
			if (isEmpty(p2Queue)) {
				if (isEmpty(p3Queue)) {
					currentTime++;
					return;
				} else {
					//run process from priority queue 3, additional logic needed
					executeFCFS(p3Queue);
				}
			} else {
				//run process from priority queue 2, additional logic needed
				executeFCFS(p2Queue);
			}
		} else {
			//run process from priority queue 1, additional logic needed
			executeFCFS(p1Queue);
		}
	} else {
		// run process from real time queue, additional logic needed
		/* Note: "Real time processes will not need any IO resources, but require memory allocation, 64 Mbytes or less" */
		executeFCFS(RTQueue);
	}
	// one unit of time has passed
	currentTime++;
}


//PARAMS: current time quantum
//EFFECTS: update the dispatcher
//RETURNS: 1 for process put into real time queue, 0 for user queue, -1 if none
int updateDispatcher(int arrival, int priority, int duration, int memsize, 
	int printers, int scanners, int modems, int drives) {

	// Set needed resources
	struct pcbres *resources = malloc(sizeof(struct pcbres));
	resources->printersNeeded = printers;
	resources->scannersNeeded = scanners;
	resources->modemsNeeded = modems;
	resources->drivesNeeded = drives;
	resources->memNeeded = memsize;

	// Set process information
	struct pcb *process = malloc(sizeof(struct pcb));
	process->pid = pid++;
	process->priority = priority;
	process->arrivalTime = arrival;
	process->remainingTime = duration;
	process->res = resources;

	enqueue(dispatcher, process);

	/*
	Any User priority jobs in the User job queue that can run within available resources 
	(memory and i/o devices) are transferred to the appropriate priority queue.
	Normal operation of a feedback queue will accept all jobs at the highest priority level 
	and degrade the priority after each completed time quantum. However, this dispatcher has 
	the ability to accept jobs at a lower priority, inserting them in the appropriate queue.
	This enables the dispatcher to emulate a simple Round Robin dispatcher if all jobs are
	accepted at the lowest priority.
	*/
}

//PARAMS: current simulated time quantum
//EFFECTS: runs the dispatcher, enqueuing or holding as appropriate
//RETURNS: 1 if real time queue was updated (for intercepting), 0 otherwise
int runDispatcher(int currentTime) {
	// our return value for whether or not the realtime queue was updated
	int rtIntercept = 0;
	int i, count;

	// hold the dispatcher in queue that we can modify with impunity
	struct queue *holder = dispatcher;

	// if elements were added, leftover elements get pushed to this queue which will be the new dispatcher
	struct queue *updatedDispatcher;
	initQueue(&updatedDispatcher);

	if(isEmpty(dispatcher) == 1) {
		printf("There is nothing to dispatch right now.\n\n");
		return 0;
	}
	// Question: What does holder do?
	// trying to count number of items in dispatcher
	// count elements in dispatcher
	for(count = 1; holder->next != NULL; count++) {
		holder = holder->next;
	}

	// For debugging
	// printf("count: %d\n", count);

	// reset after counting
	holder = dispatcher;

	// if this is to simulate 1 time quantum, should only be able to perform once, not loop
	// Current issue: segfault after terminating process
	for(i = 0; i < count; i++) {
		// grab process
		struct pcb *process = dequeue(&holder)->process;
		
		// handle arrival time
		if(currentTime >= process->arrivalTime) {
			if(allocRes(process)) {
				printf("Allocated Process PID: %i\n\n", process->pid);
				if(process->priority == 0)
					rtIntercept = 1;
			}
			else // can't allocate resource so put it back
				enqueue(updatedDispatcher, process);
		}
		else // hasn't arrived yet so put it back
			enqueue(updatedDispatcher, process);

		// queue appropriately
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
	

	//TODO: this is causing segfaults since holder can be null, come back to this
	// free the current dispatcher
	// while(isEmpty(dispatcher) != 1) {
	// 	holder = dispatcher->next;
	// 	free(dispatcher->process);
	// 	free(dispatcher);
	// 	dispatcher = holder;
	// }

	// // update dispatcher with leftover processes that could not be run
	dispatcher = updatedDispatcher;

	return rtIntercept;
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


//PARAMS: input queue (any of of the priority queues or real time queue)
//EFFECTS: executes the input queue in FCFS fashion
//		   (last element has time remaining decremented, dequeued if completed)
//RETURNS: 1 if process dequeued and run, else 0
int executeFCFS(struct queue *queue) {
	//TODO: display necessary information
	// A message displaying the process ID when the process starts;
	// A regular message every second the process is executed; and
	// A message when the process is Suspended, Continued, or Terminated.
	// terminated if time remaining is zero
	// continued if it was started and resumed
	// suspended if another process is run before completion of current process

	/*
	When a job is started (fork and exec("process",...)), 
	the dispatcher will display the job parameters 
	(Process ID, priority, processor time remaining (in seconds),
	memory location and block size, and resources requested) before performing the exec.
	*/

	/*
	A low priority User job is allowed to run for one dispatcher tick (one second) 
	before it is suspended (SIGTSTP) or terminated (SIGINT) if its time has expired. 
	If suspended, its priority level is lowered (if possible) and it is re-queued on
	the appropriate priority queue as shown in Figures 1 & 3 above.
	To retain synchronisation of output between your dispatcher and the child process,
	your dispatcher should wait for the process to respond to a SIGTSTP or SIGINT signal
	before continuing ( waitpid(p->pid, &status, WUNTRACED)). 
	To match the performance sequence indicated in Stallings' comparison of scheduling 
	policies the User job should not be suspended and moved to a lower priority level 
	unless another process is waiting to be (re)started.
	*/
	if (queue != NULL) {
	if (!isEmpty(queue)) {
		struct queue *cursor = queue;

		// cursor is last element in queue
		while (cursor->next)
			cursor = cursor->next;

		//need to add check that resources can be allocated before running
		// allocRes(cursor->process); // this was done in the sending from dispatcher to queues, so no need here
		// if started flag was not set, set to one
		if (cursor->process->started != 1) {
			cursor->process->started = 1;

			// //need to add check that resources can be allocated before running
			// allocRes(cursor->process);

			printf("Process %d: Started (Remaining Time: %d) | Current Time: %d\n", cursor->process->pid, cursor->process->remainingTime, currentTime);
		} else {
			printf("Process %d: Continued (Remaining Time: %d) | Current Time: %d\n", cursor->process->pid, cursor->process->remainingTime, currentTime);
		}

		//alternative:
		// cursor->remainingTime = cursor->remainingTime - remainingTimeSplice; 
		cursor->process->remainingTime = cursor->process->remainingTime - 1;

		if (cursor->process->remainingTime <= 0) {
			printf("Process %d: Terminated\n", cursor->process->pid);
			// free resources only if process to be terminated
			dequeue(&cursor); // TODO: this here is causing segfault, fixed by calling by reference
			freeHostRes(cursor->process);
			
		}
		else {
			printf("Process %d: Suspended\n", cursor->process->pid);	
			// implement aging, move to 1 priority lower if in priority queues
			switch(cursor->process->priority) {
				case 0: // is in real time queue
					break;
				case 1:
					enqueue(p2Queue, dequeue(&cursor)->process);
					break;
				case 2:
					enqueue(p3Queue, dequeue(&cursor)->process);
					break;
				case 3:
					// can't move any lower
					break;
				default:
				// invalid priority, nothing enqueued
				return 0;
			}
		}
		return 1;
	}
	}
	return 0;
}

//PARAMS: valid queue's head
//EFFECTS: none
//RETURNS: 1 if queue empty, 0 otherwise
int isEmpty(struct queue *queue) {
	return queue->header == 0 ? 1 : 0;
}

//PARAMS: target queue to insert process, current process to queue up
//EFFECTS: queues current process at end of given queue
//RETURNS: none
void enqueue(struct queue *target, struct pcb *currentProcess) {
	if (target != NULL) {
		if(target->header == 0) {
			target->process = currentProcess;
			target->header = 1;
			// added element is the head, single element of queue
		}
		else {
			struct queue *cursor = target;
			while (cursor->next != NULL)
				cursor = cursor->next;
			struct queue *thisProcess = malloc(sizeof(struct queue));
			cursor->next = thisProcess;
			cursor->next->header = 0;
			cursor->next->next = NULL;
			cursor->next->process = currentProcess;
		}
	} else {
		printf("Invalid queue, nothing enqueued.\n");
	}
}


//PARAMS: pointer to queue containing header to dequeue
//EFFECTS: dequeues the header of queue
//RETURNS: dequeued header
struct queue* dequeue(struct queue **header) {
	if(((*header)->process) == NULL)
		return NULL;

	struct queue *dequeuedHeader = *header;

	if((*header)->next == NULL)
		*header = NULL;
	else
		*header = (*header)->next;

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

	// freeHostRes(process);
	// priority was invalid, not within 0 to 3 range
	return -1;
}
