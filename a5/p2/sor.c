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
#include <sys/wait.h>
#include <sys/types.h>


// n symbolic constant between 3 to 6, if you changed the value of n, change 
// the matrix accordingly below
#define n 3
/* relaxation factor must be 0 < omega < 2 */
/* set to greater than 1 for speeding up convergency of a 
   slow-converging process, while values less than 1 are used to help 
   establish convergence of a diverging iterative process or speed up 
   convergence of an overshooting process */
#define omega 1.25
#define MAX_NUM 100000
#define MIN_NUM 0

// inputs: A, b, omega
// outputs: ptr (solutions to n variables)

void solveSystem(double A[n][n], double b[n], int Xi);

int main() {
	int status; // for wait
	// Initialize shared memory
	// size of shared memory object
	const int SIZE = 8 * n; // 8 bytes per double

	// name of the shared memory object
	const char *name = "PHI";
	const char *oldphiname = "OLDPHI";

	double phi[n];
	double oldphi[n];
	double error[n];
	int i, k, m;
	
	// initialize phi
	for (i = 0; i < n; i++)
		phi[i] = 0; // set initial guess to zeroes

	// shared memory file descriptor
	int shm_fd;
	int shm_fd_oldphi;

	// pointer to shared memory object is a double pointer
	double *ptr;
	double *oldptr;
	
	// create the shared memory object
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	shm_fd_oldphi = shm_open(oldphiname, O_CREAT | O_RDWR, 0666);
	
	// configure the size of the shared memory object
	ftruncate(shm_fd, SIZE);
	ftruncate(shm_fd_oldphi, SIZE);

	// memory map the shared memory object
	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	oldptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd_oldphi, 0);
	// ptr, oldptr are now shared
	
	// write to the shared memory object
	if (ptr != MAP_FAILED) {
		for (i = 0; i < n; i++) {
			ptr[i] = phi[i]; 
		}
	} else
		perror("mmap()");


	// 3 x 3 system of equations CORRECT
	 double A[n][n] = { { 12.0, 3.0, -5.0 }, { 1.0, 5.0, 3.0 }, { 3.0, 7.0, 13.0 } };
	 double b[n] = { 1, 28, 76 };

	// 4 x 4 system of equations CORRECT
	// double A[n][n] = { { 5, 0, 0, 0}, { 0, 8, 0, 0}, { 2, 3, 4, 5}, { 3, 3, 2, 3} };
	// double b[n] = { 60, 56, 3, 3 };

	// 5 x 5 system of equations CORRECT
	// double A[n][n] = { { -7, 1, 0, 1, 1 }, { 1, 1, 0, 0, 1}, { 1, 1, 5, 0, 1}, { 1, -1, 1, 1, 2}, { 1, 0, 1, 0, 1} };
	// double b[n] = { 1, 0, 9, 1, 0 };

	// 6 x 6 system of equations CORRECT
	// double A[n][n] = { { 9, 0, 0, 0, 0, 0 }, { 1, 2, 0, 0, 0, 0}, { 0, 0, 1, 0, 0, 0}, { 0, 1, 7, -1, 0, 1}, { 1, 1, 1, 1, 1, 1}, { 7, 0, 1, 12, 1, 1} };
	// double b[n] = { 90, 15, 16, 19, -20, 10 };

	 int parentpid = getpid();
	
	double maxError = MIN_NUM;
	double currentError = MAX_NUM;

	while (currentError > 0.01) {

		// NOTE: UPDATES OLDPTR ONLY AFTER VALUES HAVE BEEN CALCULATED FOR PREVIOUS ITERATION
		for (k = 0; k < n; k++){
			oldptr[k] = ptr[k]; // old ptr gets previous values after iteration
		}

		for (i = 0; i < n; i++) { //i is the process number. eg process  1 does x1 and so on
			solveSystem(A,b,i);
			wait(&status);
		}

		for (m = 0; m < n; m++) {
			error[m] = fabs( ( ( ptr[m] - oldptr[m] )  / (ptr[m]) )  * 100 );
		}

		// for debugging purposes
		// printf("errors: \n");
		// for (k = 0; k < n; k++) {
		// 	printf("%f \n", error[k]);
		// } 

		for (k = 0; k < n; k++) {
			printf("In progress, current computation... X%d = %f \n", k+1 ,oldptr[k]); // new calculated phi values
		} 

		/* 
		   check convergence: the new value is same as previous value
		   accomplish this by checking for approximate error between 
		   new value and old value, the max should be close to 0 */

		maxError = MIN_NUM;
		for (m = 0; m < n; m++) {
			maxError = error[m] > maxError ? error[m] : maxError;
		}

		currentError = maxError;
		printf("Current Error %f \n", currentError);

	}

	if (parentpid == getpid()) {
		for (i = 0; i < n; i++) {
			printf("Final Result: X%i = %f \n", i+1, *(ptr+i));
		}
	}

	shm_unlink(name);
	shm_unlink(oldphiname);
	return 0;
}




void solveSystem(double A[n][n], double b[n], int Xi) {

	int pid = fork();

	if (pid == 0) {

		/* the size (in bytes) of shared memory object */
		const int SIZE = 8 * n;
		/* name of the shared memory object */
		const char *name = "PHI";
		const char *oldphiname = "OLDPHI";
		/* shared memory file descriptor */ 
		int shm_fd;
		int shm_fd_oldphi;
		/* pointer to shared memory obect */ 
	 	double *ptr;
	 	double *oldptr;
		/* open the shared memory object */
		shm_fd = shm_open(name, O_RDWR, 0666);
		shm_fd_oldphi = shm_open(name, O_RDWR, 0666);
		/* memory map the shared memory object */
		ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
		oldptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	    /* read from the shared memory object */

		double sigma;
		int j;


		sigma = 0;
		// calculate sigma, which is used in computing the new values
		for (j = 0; j < n; j++) {
			sigma =  j != Xi ? sigma + (A[Xi][j] * ptr[j]) : sigma;
		}

		// Compute new value based on previous answers in oldptr
		ptr[Xi] = ((1-omega)*oldptr[Xi]) + ((omega / A[Xi][Xi]) * (b[Xi]-sigma));
	}
}
