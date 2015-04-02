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

// keep track of the current active directory
char currentDir[100];


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

// From FILE * to int

// int fildes = fileno(f);
// From int to FILE *:

// FILE *f = fdopen(fildes, "r+");

int fClose (int fileID) {
	return fclose(fdopen(fileID, "r+"));
}

int fRead (int fileID, char *buffer, int length) {
   // return fread(buffer, sizeof(char), length, fdopen(fileID, "r"));
   return fread(buffer, BLOCK_SIZE, length, fdopen(fileID, "r"));
}

int fSeek (int fileID, int position) {
   return fseek(fdopen(fileID, "r"), position, SEEK_SET);
}



// Are we assuming this function is called after fOpen and provides information on 
// the current file targeted by fOpen? Is there some kind of buffer or stream that 
// can be accessed for the currently "opened" file?
// Otherwise, are we allowed to use a global variable that tracks the latest opened
// file's FILE pointer, which fLs will call upon?

int fLs (void) {
   // The fLs() function should print all the information your system knows about the files in the current directory, 
   // then return -1 if detect an error and 0 otherwise. */

   DIR *directory;
   // struct dirent *dir;
   directory = opendir(currentDir); // current directory


   if (directory) {
      printf("%s\n", "Files in current directory:");

      // while ((dir = readdir(directory)) != NULL) {
      //    if (dir->d_type == DT_REG) printf("%s\n", dir->d_name);
      // }

      /* Explanation: 
      lh flag to show expanded information in human readable form
      grep with regex to avoid displaying directories
      tail to omit first line ("total XX")
      */
      system("ls -lh -S -p | grep -v '/$' | tail -n +2");

      closedir(directory);
      return 0;
   }

   // error with accessing current directory
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

int main(int argc, char **argv) {
   strcpy(currentDir, ".");

   int fd = fOpen("testFile.txt");
   printf("File id: %d \n", fd);
   char buffer[NUM_BLOCKS];

   fSeek(fd, 0);
   /* Read and display data */
   fRead(fd, buffer, NUM_BLOCKS);
   printf("File contents: %s\n", buffer);

   char str[] = "Additional written information\n";
   printf("Size of content written: %d\n", fWrite(fd, str, sizeof(str)));


   // int fd2 = fOpen("testFile.txt");
   // FILE *file = fdopen( fd2, "a+");
   // // FILE *file = fopen( "testFile.txt", "a+");
   // // printf("%d \n", fwrite(str, 1, sizeof(str), file));

   // fSeek(fd2, 0);
   // /* Read and display data */
   // fRead(fd2, buffer2, NUM_BLOCKS);
   // printf("File contents: %s\n", buffer2);

   
   printf("%s\n", fLs() == -1 ? "Failed fLs" : "Succeeded fLs");
   fClose(fd);
   printf("%s\n", fMkdir("burgers") == -1 ? "Failed fMkdir" : "Succeeded fMkdir");
   printf("%s\n", fCd("burgers") == -1 ? "Failed fCd" : "Succeeded fCd");
   printf("%s\n", fLs() == -1 ? "Failed fLs" : "Succeeded fLs");
}