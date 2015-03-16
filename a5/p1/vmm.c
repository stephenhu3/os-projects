#include "vmm.h"

int main(int argc, char **argv) {
	int addresses[NUMTESTADDR] = {1,256,32768,128,65534,33153};

	int addrCount;

	for(addrCount = 0; addrCount < NUMTESTADDR; addrCount++) {
		int pageNum = getPageNum(addresses[addrCount]);
		int pageOffset = getPageOffset(addresses[addrCount]);

		printf("Logical Address: %i\n", addresses[addrCount]);
		printf("Page Number: %i\n", pageNum); 
		printf("Page Offset: %i\n", pageOffset);
	}
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
