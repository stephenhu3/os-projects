#include "hostds.h"

struct host host;

int main(int argc, char **argv) {
	if (argc != 2) {
		printf("Error processing arguments... Please specify filename in format: ./hostds filename.txt\n");
		exit(0);
	}

	initSys();

	// printf("Printers: %i\n", host.numPrinters);
}

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

	// TODO -- QUEUES
}
