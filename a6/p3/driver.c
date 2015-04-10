// C) Write one driver program (driver.c) that tests each function and feature (such as subdirectories).

#include "manager.h"

int main(int argc, char **argv) {
	char buffer[NUM_BLOCKS];
	char buffer2[NUM_BLOCKS];

	printf("********************************\n");
	printf("Test in current directory\n");
	// Test opening file in current directory, creates if it doesn't exist
	int fd = fOpen("testFile.txt");
	printf("%s\n", fd == -1 ? "Failed fOpen" : "Succeeded fOpen");

	// Print its file ID and name
	printf("File Name: testFile.txt\n");
	printf("File ID: %d \n", fd);

	// Write to file
	char str[] = "Test written information\n";
	printf("Size of content written: %d\n", fWrite(fd, str, sizeof(str)));

	fSeek(fd, 0);
	// Read and display data 
	fRead(fd, buffer, NUM_BLOCKS);
	printf("File contents: %s\n", buffer);

	// Close file descriptor
	printf("%s\n", fClose(fd) == 0 ? "Succeeded fClose" : "Failed fClose");

	// Display current directory's file information   
	printf("%s\n", fLs() == -1 ? "Failed fLs" : "Succeeded fLs");
	printf("********************************\n");

	printf("Test in new directory\n");
	// Make new directory and access it
	printf("%s\n", fMkdir("testDirectory") == -1 ? "Failed fMkdir" : "Succeeded fMkdir");
	printf("New directory name: testDirectory\n");
	printf("%s\n", fCd("testDirectory") == -1 ? "Failed fCd" : "Succeeded fCd");

	// Create file in new directory
	int fd2 = fOpen("testFileNewDirectory.txt");
	printf("%s\n", fd2 == -1 ? "Failed fOpen" : "Succeeded fOpen");

	// Print its file ID and name
	printf("File Name: testFileNewDirectory.txt\n");
	printf("File ID: %d \n", fd2);

	// Write to file
	char str2[] = "Written information to new directory file\n";
	printf("Size of content written: %d\n", fWrite(fd2, str2, sizeof(str2)));

	fSeek(fd2, 0);
	// Read and display data 
	fRead(fd2, buffer2, NUM_BLOCKS);
	printf("File contents: %s\n", buffer2);

	// Close file descriptor
	printf("%s\n", fClose(fd2) == 0 ? "Succeeded fClose" : "Failed fClose");
	printf("%s\n", fLs() == -1 ? "Failed fLs" : "Succeeded fLs");
	printf("********************************\n");
}