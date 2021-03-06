#include "manager.h"

// keep track of the current active directory, default to current directory
char currentDir[100] = ".";


/* Generally, fOpen(), fClose(), fRead(), and fSeek() functions should behave like the UNIX 
   kernel functions open(), close(), read(), and lseek(), respectively, except the behaviour 
   is simplified by the assumptions. For example, fOpen() does not have a flag parameter, so 
   your function should operate as if O_RDWR | O_CREAT were used in the kernel function equivalent.
   The fLs() function should print all the information your system knows about the file, 
   then return -1 if detect an error and 0 otherwise. */

// returns the fileid
int fOpen (char *name) {
   // "a+ mode" opens file for reading or writing if it exists, creates if not
   // file pointer at end of file after open, need to reset to start if reading
   FILE *file = fopen( name, "a+");
   return fileno(file);
}

int fClose (int fileID) {
	return fclose(fdopen(fileID, "r+"));
}

int fRead (int fileID, char *buffer, int length) {
   return fread(buffer, BLOCK_SIZE, length, fdopen(fileID, "r"));
}

int fSeek (int fileID, int position) {
   return fseek(fdopen(fileID, "r"), position, SEEK_SET);
}

int fLs (void) {
   // The fLs() function should print all the information your system knows about the files in the current directory, 
   // then return -1 if detect an error and 0 otherwise. */

   DIR *directory;
   directory = opendir(currentDir); // current directory


   if (directory) {
      printf("%s\n", "Files in current directory:");

      /* Explanation: 
      lh flag to show expanded information in human readable form
      grep with regex to avoid displaying directories
      tail to omit first line ("total XX")
      */
      system("ls -lh -S -p | grep -v '/$' | tail -n +2");

      closedir(directory);
      return 0;
   }

   // return -1 if error with accessing current directory
   return -1;
}

/* Generally, fWrite() function should behave like the UNIX kernel write() function,
   except with behaviour that is simplified by the assumptions. It returns the number
   of bytes actually written by the function call. The fMkdir() function should create
   the named directory, then return -1 if you detect an error and 0 otherwise.
   The fCd() function should change the current directory to the named directory 
   if it exists, then return -1 if you detect an error and 0 otherwise. */
int fWrite (int fileID, char *buffer, int length) {
   if (!length) return 0; // nothing written if length is zero
   FILE *file = fdopen(fileID, "a+"); //open file for writing
   if (file) {
      int written = fwrite(buffer, 1, length, file);
      fflush(file); // write data to the file immediately
      return written;
   } else return 0;
}

int fMkdir (char *name) {
   return mkdir(name, 0755);
}

int fCd (char *name) {
	return chdir(name);
}