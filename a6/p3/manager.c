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
int fOpen (char *name) {
	//TODO
}

int fClose (int fileID) {
	//TODO
}

int fRead (int fileID, char *buffer, int length); int fSeek (int fileID, int position) {
	//TODO
}

int fLs (void) {
	//TODO
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
	//TODO
}

int main(int argc, char **argv) {

}