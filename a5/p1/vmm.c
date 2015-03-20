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

	// Open BACKING_STORE.bin into a file
	backStore = NULL;
	backStore = fopen("BACKING_STORE.bin", "r");

	if (backStore == NULL) {
		printf("Error! opening BACKING_STORE.bin\n");
		return 0;
	}
	else
		printf("BACKING_STORE.bin is opened\n");

	// Initializes and clears page table
	initPageTable(&pageTable);

	// Initializes and clears TLB
	initTLB(&tlb);

	// Read from addresses.txt
	int *addresses = (int *) malloc(sizeof(int) * MAXADDR);
	getAddr(argv[1], &addrRead, addresses);
	// printf("%i\n", ++addrRead);

	for(addrCount = 0; addrCount <= addrRead; ++addrCount) {
		pageNum = getPageNum(addresses[addrCount]);
		pageOffset = getPageOffset(addresses[addrCount]);

		frameNum = checkTLB(&tlb, pageNum);
		
		if(frameNum == -1)
			frameNum = findPageNum(&pageTable, pageNum);
		else
			tlbHits++;

		if(frameNum != -1) {
			page = pageTable.page[frameNum];
			// printf("ver1  ");
 		}
		else {
			// printf("ver2  ");
			page = readPage(backStore, pageNum);
			frameNum = backStoreUpdate(&pageTable, page, pageNum);
			pageFaults++;
		}
	
		// printf("Frame Number: %i  ", frameNum);
		printf("Virtual Address: %i  ", addresses[addrCount]);
		printf("Physical Address: %i  ", (frameNum * 0x100) + pageOffset);
		printf("Value: %i\n", page[pageOffset]);
		// printf("Page Number: %i  ", pageNum);
		// printf("Page Offset: %i", pageOffset);

		updateTLB(&tlb, pageNum, frameNum);

		fflush(stdout);
	}

	printf("Number of Translated Addresses = %i\n", ++addrRead);
	printf("Page Faults = %i\n", pageFaults);
	printf("Page Fault Rate = %f\n", (double) pageFaults / (double) (++addrRead));

	printf("TLB Hits = %i\n", tlbHits);
	printf("TLB Hit Rate = %f\n", (double) tlbHits / (double) (++addrRead));

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
	if(offset < 0 || offset >= PAGE_SIZE)
		return NULL;
	
    	char *buffer = malloc(PAGE_SIZE);

    	fseek(backStore, offset * PAGE_SIZE, SEEK_SET);

	if(!fread(buffer, sizeof(char), PAGE_SIZE, backStore))
		return NULL;

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

//PARAMS: page table and page number (0 to 255)
//EFFECTS: updates page table with new page from BACKING_STORE.bin
//RETURNS: none
int backStoreUpdate(struct page_table *pageTable, char *page, int pageNum) {
	int i, frameNum, limit, freeIndex;

	if(pageTable->frame_count == FRAME_SIZE) {
		for(i = 0; i < FRAME_SIZE; i++) {
			if(pageTable->counter[i] >= limit) {
				limit = pageTable->counter[i];
				freeIndex = i;
			}
		}
	free(pageTable->page[freeIndex]);
	pageTable->page[freeIndex] = page;
	pageTable->page_num[freeIndex] = pageNum;
	pageTable->counter[freeIndex] = 0;
	frameNum = freeIndex;
	}
	else {
		pageTable->page[pageTable->frame_count] = page;
		pageTable->page_num[pageTable->frame_count] = pageNum;
		frameNum = pageTable->frame_count;
		pageTable->frame_count++;
	}

	for(i = 0; i < pageTable->frame_count; i++)
		pageTable->counter[i]++;

	return frameNum;
}

//PARAMS: tlb struct
//EFFECTS: initialize pageNum, frameNum values to -1 and counter values and tlbcount to 0
//RETURNS: none
void initTLB(struct tlb *tlb) {
	int i;

	for(i = 0; i < TLB_SIZE; i++) {
		tlb->pageNum[i] = -1;
		tlb->frameNum[i] = -1;
		tlb->counter[i] = 0;
	}
	tlb->tlbCount = 0;
}

//PARAMS: tlb struct and page number
//EFFECTS: checks if tlb contains frameNum for pageNum
//RETURNS: returns frameNum or -1 on failure
int checkTLB(struct tlb *tlb, int pageNum) {
	int i;

	for(i = 0; i < tlb->tlbCount; i++)
		if(tlb->pageNum[i] == pageNum)
			return tlb->frameNum[i];

	return -1;
}

//PARAMS: tlb struct, page number, and frame number
//EFFECTS: updates tlb with page and frame number eviction where necessary
//RETURNS: none
void updateTLB(struct tlb *tlb, int pageNum, int frameNum) {
	int i, limit, freeIndex;

	if(tlb->tlbCount == TLB_SIZE) {
		for(i = 0; i < TLB_SIZE; i++) {
			if(tlb->counter[i] >= limit) {
				limit = tlb->counter[i];
				freeIndex = i;
			}
		}
		tlb->pageNum[freeIndex] = pageNum;
		tlb->frameNum[freeIndex] = frameNum;
		tlb->counter[i] = 0;
	}
	else {
		tlb->pageNum[tlb->tlbCount] = pageNum;
		tlb->frameNum[tlb->tlbCount] = frameNum;
		tlb->tlbCount++;
	}

	for(i = 0; i < tlb->tlbCount; i++)
		tlb->counter[i]++;
}
