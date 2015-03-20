#ifndef VMM_H
#define VMM_H

// INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// DEFINES

#define NUMTESTADDR 6
#define MAXADDR 2048

#define PAGE_SIZE 256
#define FRAME_SIZE 128
#define TLB_SIZE 16

// STRUCT DEFINITIONSS

struct page_table{
	char *page[FRAME_SIZE];
	int page_num[FRAME_SIZE];
	int counter[FRAME_SIZE];
	int frame_count;
};

struct tlb{
	int pageNum[TLB_SIZE];
	int frameNum[TLB_SIZE];
	int counter[TLB_SIZE];
	int tlbCount;
};

// FUNCTION PROTOTYPES
int getPageNum(int address);
int getPageOffset(int address);
void getAddr(char *fileName, int *addrRead, int *addresses);

void initPageTable(struct page_table *pagetable);
char *readPage(FILE *backStore, int offset);
int findPageNum(struct page_table *pageTable, int pageNum);
int backStoreUpdate(struct page_table *pageTable, char *page, int pageNum);

void initTLB(struct tlb *tlb);
int checkTLB(struct tlb *tlb, int pageNum);
void updateTLB(struct tlb *tlb, int pageNum, int frameNum);

#endif
