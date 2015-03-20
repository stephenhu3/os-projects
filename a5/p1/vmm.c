#include "vmm.h"

int main(int argc, char **argv) {
	// Declarations
	struct page_table pageTable;
	struct tlb tlb;
	FILE *backStore;

	char *page;

	int addrRead = 0;
	int addrCount = 0;
	int pageFaults = 0;
	int tlbHits = 0;

	int pageNum = 0;
	int frameNum = 0;
	int pageOffset = 0;

	// int addresses[NUMTESTADDR] = {1,256,32768,128,65534,33153};

	// TODO: implement checking for arguments (ie. ./vmm filename.txt derperperpdp)

	// Read from addresses.txt
	int *addresses = (int *) malloc(sizeof(int *) * MAXADDR);
	getAddr(argv[1], &addrRead, addresses);
	printf("%i\n", addrRead);

	// Initializes and clears page table
	initPageTable(&pageTable);

	// Open BACKING_STORE.bin into a file
	backStore = NULL;
	backStore = fopen("BACKING_STORE.bin", "r");

	if (backStore == NULL) {
		printf("Error! opening BACKING_STORE.bin\n");
		return 0;
	}
	else
		printf("BACKING_STORE.bin is opened\n");

	for(addrCount = 0; addrCount <= addrRead; ++addrCount) {
		int pageNum = getPageNum(addresses[addrCount]);
		int pageOffset = getPageOffset(addresses[addrCount]);

		frameNum = findPageNum(&pageTable, pageNum);

		if(!frameNum)
			page = pageTable.page[frameNum];
		else {
			page = readPage(backStore, pageNum);
			pageFaults++;
		}
		
		printf("Logical Address: %i  ", addresses[addrCount]);
		printf("Physical Address: %i\n", (frameNum * 0x100) + pageOffset);
		// printf("Page Number: %i\n", pageNum);
		// printf("Page Offset: %i\n", pageOffset);
	}

	free(addresses);

	fclose(backStore);
}

//PARAMS: logical address
//EFFECTS: none
//RETURNS: address shifted 8 bits to the right masked to 8 bits (bits 8-15 = page number)
int getPageNum(int address) {
	return ((address / 0x100) & 0xFF);
}

//PARAMS: logical address
//EFFECTS: none
//RETURNS: address masked to 8 bits (bits 0-7 = page offset)
int getPageOffset(int address) {
	return (address & 0xFF);
}

//PARAMS: filename (addresses.txt) and address read counter
//EFFECTS: reads addresses from file to address array while incrementing counter
//RETURNS: none
void getAddr(char *fileName, int *addrRead, int *addresses) {
	int i;

	FILE *f = fopen(fileName, "r");

	for(i = 0; i < MAXADDR; i++) {
		if(fscanf(f, "%i", &addresses[i]) != 1) {
			break;
		}
		*addrRead = i;
	}
}

// PARAMS: page table
// MODIFIES: pageTable
// EFFECTS: To initialize each index with -1 and 0 for the number of page and number of frame relatively. And to set frame count to 0
// RETURNS: none
void initPageTable(struct page_table *pagetable) {

    int i;
    for (i = 0; i < FRAME_SIZE; i++) {
        pagetable->page_num[i] = -1;
        pagetable->counter[i] = 0;
    }
    pagetable->frame_count = 0;
}

// PARAMS: backingStore and offset
// MODIFIES: none
// EFFECTS: seek to byte position offset * PAGE_SIZE, and returns the buffer.
char *readPage(FILE *backStore, int offset){

    char *buffer = malloc(PAGE_SIZE);

    fseek(backStore, offset * PAGE_SIZE, SEEK_SET);

    return buffer;
}

//PARAMS: page table and page number (0 to 255)
//EFFECTS: none
//RETURNS: index of page_table->page_num that contains pageNum, otherwise return -1 for failure
int findPageNum(struct page_table *pageTable, int pageNum) {
	int i;

	for(i = 0; i < pageTable->frame_count; i++) {
		if(pageTable->page_num[i] == pageNum)
			return i;
	}
	return -1;
}

