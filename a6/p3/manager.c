/* The on-disk file descriptor contains only minimal information: 
1) a filename of six or fewer characters
2) at most four disk blocks per file (you may use 2-byte block addresses.) 
   The on-disk file descriptor will fit into one disk block.
* The disk blocks will be very small, say, 50 bytes per block (use a symbolic constant).
* Directories need contain only minimal information describing the file, just enough to get your file manager working.
Also:
* No need to implement file sharing, so no locks.
* No need to implement file modes such as read, write, or execute.
* No need to include any protection or authentication in your file system.
* No need to implement path names, just filenames within the current directory. * No need to implement buffering. */

#include "manager.h"

/* Generally, fOpen(), fClose(), fRead(), and fSeek() functions should behave like the UNIX 
   kernel functions open(), close(), read(), and lseek(), respectively, except the behaviour 
   is simplified by the assumptions. For example, fOpen() does not have a flag parameter, so 
   your function should operate as if O_RDWR | O_CREAT were used in the kernel function equivalent.
   The fLs() function should print all the information your system knows about the file, 
   then return -1 if detect an error and 0 otherwise. */

// returns the fileid
int fOpen (char *name) {
   // "a+ mode" opens file for reading or writing if it exists, creates if not
   FILE *file = fopen( name, "a+");
   return fileno(file);
}

// From FILE * to int

// int fildes = fileno(f);
// From int to FILE *:

// FILE *f = fdopen(fildes, "r+");

int fClose (int fileID) {
	return fclose(fdopen(fileID, "r+"));
}

// issue reading, TODO
int fRead (int fileID, char *buffer, int length) {
   // return fread(buffer, sizeof(char), length, fdopen(fileID, "r"));
   return fread(buffer, BLOCK_SIZE, length, fdopen(fileID, "r"));
}

int fSeek (int fileID, int position) {
   return fseek(fdopen(fileID, "r"), position, SEEK_SET);
}

int fLs (void) {
	//TODO
   // The fLs() function should print all the information your system knows about the file, 
   // then return -1 if detect an error and 0 otherwise. */
}


/* Generally, fWrite() function should behave like the UNIX kernel write() function,
   except with behaviour that is simplified by the assumptions. It returns the number
   of bytes actually written by the function call. The fMkdir() function should create
   the named directory, then return -1 if you detect an error and 0 otherwise.
   The fCd() function should change the current directory to the named directory 
   if it exists, then return -1 if you detect an error and 0 otherwise. */
int fWrite (int fileID, char *buffer, int length) {
	//TODO
}

int fMkdir (char *name) {
	//TODO
}

int fCd (char *name) {
	//TODO`
}

int main(int argc, char **argv) {
   int fd = fOpen("testFile.txt");
   printf("File id: %d \n", fd);
   char buffer[NUM_BLOCKS];
   fSeek(fd, 0);
   /* Read and display data */
   fRead(fd, buffer, NUM_BLOCKS);
   printf("File contents: %s\n", buffer);
   fClose(fd);
}