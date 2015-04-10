#ifndef MANAGER_H
#define MANAGER_H

// INCLUDES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
// for fLS()
#include <dirent.h>


// DEFINES
#define NUM_BLOCKS 100
#define BLOCK_SIZE 50
//void initDisk (void);
//int dRead (int addr, char *buf);


// STRUCT DEFINITIONS

// FUNCTION PROTOTYPES

/* Generally, fOpen(), fClose(), fRead(), and fSeek() functions should behave like the UNIX 
   kernel functions open(), close(), read(), and lseek(), respectively, except the behaviour 
   is simplified by the assumptions. For example, fOpen() does not have a flag parameter, so 
   your function should operate as if O_RDWR | O_CREAT were used in the kernel function equivalent.
   The fLs() function should print all the information your system knows about the file, 
   then return -1 if detect an error and 0 otherwise. */
int fOpen (char *name);
int fClose (int fileID);
int fRead (int fileID, char *buffer, int length); 
int fSeek (int fileID, int position);
int fLs (void);

/* Generally, fWrite() function should behave like the UNIX kernel write() function,
   except with behaviour that is simplified by the assumptions. It returns the number
   of bytes actually written by the function call. The fMkdir() function should create
   the named directory, then return -1 if you detect an error and 0 otherwise.
   The fCd() function should change the current directory to the named directory 
   if it exists, then return -1 if you detect an error and 0 otherwise. */
int fWrite (int fileID, char *buffer, int length); 
int fMkdir (char *name);
int fCd (char *name);



#endif
