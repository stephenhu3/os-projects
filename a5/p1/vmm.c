#include "vmm.h"

int main(int argc, char **argv) {
	// int addresses[NUMTESTADDR] = {1,256,32768,128,65534,33153};

	// TODO: implement checking for arguments (ie. ./vmm filename.txt derperperpdp)

	int addrCount;
	
	// time to read from addresses.txt
	int addrRead;
	int *addresses = (int *) malloc(sizeof(int *) * MAXADDR);

	getAddr(argv[1], &addrRead, addresses);

	printf("%i\n", addrRead);

	for(addrCount = 0; addrCount < addrRead; addrCount++) {
		int pageNum = getPageNum(addresses[addrCount]);
		int pageOffset = getPageOffset(addresses[addrCount]);

		printf("Logical Address: %i\n", addresses[addrCount]);
		printf("Page Number: %i\n", pageNum);
		printf("Page Offset: %i\n", pageOffset);
	}

	free(addresses);
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
