// #include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
// #include <assert.h>
// #include <math.h>


#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h>



// n symbolic constant between 3 to 6
#define n 3
/* relaxation factor must be 0 < omega < 2 */
/* set to greater than 1 for speeding up convergency of a 
   slow-converging process, while values less than 1 are used to help 
   establish convergence of a diverging iterative process or speed up 
   convergence of an overshooting process */
#define omega 0.25
#define MAX_double 100000
#define MIN_double 0

// TODO: FOR STEVEN
// I have implemented the SOR algorithm as a single process program. 
// You will need to break this into multiple processes program

// may use shmget, shmat, shmdt, shmctl,...
// may use shm_open, mmap, shm_unlink,...

// segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);

// shared_memory = (char *) shmat(id, NULL, 0);

// sprintf(shared_memory, "Writing to shared memory");

// shmdt(shared_memory);

// get this working on a single process first, then multiprocess using shared memory and IPC

// implement SOR on an n-process system by having n different processes (all running same program
// where the ith process computes the Xi (ith variable in vector x)

// include A and b as arrays inside program
// output to stdout

// inputs: A, b, omega
// outputs: phi (solutions to n variables)

// shared memory is phi
// process i solves for Xi, and puts it in phi[i]

double * solveSystem(double A[n][n], double b[n]);

int main() {
	// size of shared memory object
	const int SIZE = 4096;

	// name of the shared memory object
	const char *name = "PHI";

	double phi[n];
	
	// initialize phi
	int i;
	for (i = 0; i < n; i++)
		phi[i] = 0; // set initial guess to zeroes

	// shared memory file descriptor
	int shm_fd;

	// pointer to shared memory object is a void ptr
	double *ptr;
	

	// create the shared memory object
	shm_fd = shm_open(name, O_CREAT | O_RDRW, 0666);
	
	// configure the size of the shared memory object
	ftruncate(shm_fd, SIZE);

	// memory map the shared memory object
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	
	// write to the shared memory object
	for (i = 0; i < n; i++) {
		*ptr = phi[n];
		ptr++;
	}

	int pid = fork();

	// child process
	if (pid == 0) { 
		/* the size (in bytes) of shared memory object */
		 const int SIZE = 4096;
		 /* name of the shared memory object */
		 const char *name = "PHI";
		/* shared memory file descriptor */ 
		 int shm_fd;
		/* pointer to shared memory obect */ 
	 	double *ptr;
		/* open the shared memory object */
		shm_fd = shm_open(name, O_RDONLY, 0666);
		/* memory map the shared memory object */
		ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	    /* read from the shared memory object */
	    
	    int i;
	    for (i = 0; i < n; i++) {
	    	printf("%d",(double *)ptr);
	    	ptr++;
	    }

		/* remove the shared memory object */ 
		shm_unlink(name);
		return 0;

	}
}