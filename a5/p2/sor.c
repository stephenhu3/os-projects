#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <sys/mman.h>
#include <sys/shm.h> 
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <fcntl.h>





// n symbolic constant between 3 to 6
#define n 4
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

void solveSystem(double A[n][n], double b[n], int Xi);
void function();

int main() {
	int status;
	// Initialize shared memory
	// size of shared memory object
	const int SIZE = 8 * n; // 8 bytes per double

	// name of the shared memory object
	const char *name = "PHI";

	double phi[n];
	
	// initialize phi
	int i;
	for (i = 0; i < n; i++)
		phi[i] = 0; // set initial guess to zeroes

	// shared memory file descriptor
	int shm_fd;

	// pointer to shared memory object is a double ptr
	double *ptr;
	

	// create the shared memory object
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	
	// configure the size of the shared memory object
	ftruncate(shm_fd, SIZE);

	// memory map the shared memory object
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	// ptr is now shared
	
	// write to the shared memory object
	if (ptr != MAP_FAILED) {
		for (i = 0; i < n; i++) {
			ptr[i] = phi[i]; 
		}
	} else
		perror("mmap()");


	// 3 x 3 system of equations CORRECT
	// double A[n][n] = { { 12.0, 3.0, -5.0 }, { 1.0, 5.0, 3.0 }, { 3.0, 7.0, 13.0 } };
	// double b[n] = { 1, 28, 76 };

	// 4 x 4 system of equations CORRECT
	double A[n][n] = { { 5, 0, 0, 0}, { 0, 8, 0, 0}, { 2, 3, 4, 5}, { 3, 3, 2, 3} };
	double b[n] = { 60, 56, 3, 3 };

	// 5 x 5 system of equations CORRECT
	// double A[n][n] = { { -7, 1, 0, 1, 1 }, { 1, 1, 0, 0, 1}, { 1, 1, 5, 0, 1}, { 1, -1, 1, 1, 2}, { 1, 0, 1, 0, 1} };
	// double b[n] = { 1, 0, 9, 1, 0 };

	// 6 x 6 system of equations INCORRECT - TODO FOR STEVEN: find a valid 6 x 6 equation that can be solved using this method
	// double A[n][n] = { { 9, 0, 0, 0, 0, 0 }, { 1, 2, 0, 0, 0, 0}, { 0, 0, 1, 0, 0, 0}, { 0, 1, 7, -1, -1, 1}, { 1, 1, 1, 1, 1, -4}, { 7, 1, 1, 12, 1, 1} };
	// double b[n] = { 90, 15, 16, 19, -20, 10 };

	// double A[n][n] = { { 1, 1, -2, 1, 3, -1 }, { 2, -1, 1, 2, 1, -3 }, { 1, 3, -3, -1, 2, 1 }, 
	// 				  { 5, 2, -1, -1, 2, 1 }, { -3, -1, 2, 3, 1, 3 }, { 4, 3, 1, -6, -3, -2 }
	// 				};
	// double b[n] = { 4, 20, -15, -3, 16, -27 };0
	for (i = 0; i < n; i++) {
		solveSystem(A,b,i);
	}

	// wait(&status);

	// this part should only be executed by the original, not forked functions
	for (i = 0; i < n; i++) {
		printf("X%i = %f \n", i+1, *(ptr+i));
	}

	/* TODO FOR STEVEN:
	As you can see, I have implemented shared memory and forking.
	However, currently it just forks the new process and the new process computes
	the Xi for all the unknown variables like before.

	You need to implement such that the forked process i only calculates for Xi
	This is why I have added the argument Xi to "solveSystem"

	You will also need to synchronize the actions so that Xi is computed before
	Xi+1 tries to compute for Xi+1 before Xi has completed, and so forth.

	You need to start this today. This is a large task. 
	Please provide updates to the team.
	*/


	return 0;
}




void solveSystem(double A[n][n], double b[n], int Xi) {

	int pid = fork();
	// printf("parent %d \n", Xi);


	if (pid == 0) {
		// printf("child %d \n", Xi);

		/* the size (in bytes) of shared memory object */
		const int SIZE = 8 * n;
		/* name of the shared memory object */
		const char *name = "PHI";
		/* shared memory file descriptor */ 
		int shm_fd;
		/* pointer to shared memory obect */ 
	 	double *ptr;
		/* open the shared memory object */
		shm_fd = shm_open(name, O_RDWR, 0666);
		/* memory map the shared memory object */
		ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	    /* read from the shared memory object */

		

		double error[n];

		double maxError = MIN_double;
		double currentError = MAX_double;



		/* 
		   check convergence: the new value is same as previous value
		   accomplish this by checking for approximate error between 
		   new value and old value, the max should be close to 0 */

		// static double phi[n];
		double oldphi[n], sigma;
		int i, j, k, m;

		// for (i = 0; i < n; i++)
		// 	phi[i] = 0; // set initial guess to ones

		// for (i = 0; i < n; i++)
		// 	oldphi[i] = 1; // set initial guess to zeroes
		// // Alternatively, we can clear to zeroes using calloc

		// while (currentError > 0.0001) {
		while (currentError > 0.001) {

			
			
			for (k = 0; k < n; k++)
					oldphi[k] = ptr[k]; // old phi gets previous values

			for (i = 0; i < n; i++) {
				sigma = 0;
				for (j = 0; j < n; j++) {
					sigma =  j != i ? sigma + (A[i][j] * ptr[j]) : sigma;
				}
				
				ptr[i] = ((1-omega)*oldphi[i]) + ((omega / A[i][i]) * (b[i]-sigma));

			}

			//  for debugging purposes
			// printf("oldphi: \n");
			// for (k = 0; k < n; k++) {
			// 	printf("%f \n", oldphi[k]); // old phi has previous values
			// }

			// printf("phi: \n");
			// for (k = 0; k < n; k++) {
			// 	printf("%f \n", phi[k]); // new calculated phi values
			// } 
			

			//calculate errors
			
			for (m = 0; m < n; m++) {
				error[m] = fabs( ( ( ptr[m] - oldphi[m] )  / (ptr[m]) )  * 100 );
			}
			
			 // for debugging purposes
			// printf("errors: \n");
			// for (k = 0; k < n; k++) {
			// 	printf("%f \n", error[k]); // new calculated phi values
			// } 


			// check for max error
			maxError = MIN_double;

			for (m = 0; m < n; m++) {
				maxError = error[m] > maxError ? error[m] : maxError;
			}

			currentError = maxError;
			// printf("%f \n", currentError); // current error
			
			/* remove the shared memory object */ 
			shm_unlink(name);
		
			
		}
	}
}
