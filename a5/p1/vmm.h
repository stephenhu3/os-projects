#ifndef VMM_H
#define VMM_H

// INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// DEFINES

#define NUMTESTADDR 6
#define MAXADDR 1024

#define PAGE_SIZE 256
#define FRAME_SIZE 128


struct page_table{
	char *page[FRAME_SIZE];
	int page_num[FRAME_SIZE];
	int counter[FRAME_SIZE];
	int frame_count;
};

// STRUCT DEFINITIONSS

// FUNCTION PROTOTYPES
int getPageNum(int address);
int getPageOffset(int address);
void getAddr(char *fileName, int *addrRead, int *addresses);

void initPageTable(struct page_table *pagetable);
char *readPage(FILE *backStore, int offset);


#endif
