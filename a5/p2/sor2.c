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
#include <signal.h>


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

void solveSystem(double A[n][n], double b[n], int Xi);
void function();
double xyz=0;

int main() {
	int status; // for wait
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
	} else{
		printf("ERROR writing to shared memory \n");
		perror("mmap()");
	}


	// 3 x 3 system of equations CORRECT
	 double A[n][n] = { { 12.0, 3.0, -5.0 }, { 1.0, 5.0, 3.0 }, { 3.0, 7.0, 13.0 } };
	 double b[n] = { 1, 28, 76 };

	int parentpid = getpid();
	
	for (i = 0; i < n; i++) { //i is the process number. eg process  1 does x1 and so on
		solveSystem(A,b,i); // this solve the matrix n times.. we want it to compute for Xi n times and not the whole program
	wait(&status);
	}



	if(parentpid == getpid()){ // only parent prints.
	for (i = 0; i < n; i++) {
		printf("X%i = %f \n", i+1, *(ptr+i));
		printf("pid# : %i \n",getpid());
	}}

	printf("\n\nTHIS IS A TEST FILE ONLY\n \n");
	shm_unlink(name);

	return 0;
}




void solveSystem(double A[n][n], double b[n], int Xi) {

	int pid = fork();
	printf("parent %d \n", Xi);


	if (pid == 0) {
		 printf("entering child %d \n", Xi);

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

		//printf("halfway child %d \n", Xi);

		double oldphi[n], sigma;
		int i, j, k, m;

		
		while (currentError > 0.01) {

			for (k = 0; k < n; k++){
				//printf("before stuck 12321here? child %d \n", Xi);
					oldphi[k] = ptr[k]; // old phi gets previous values
				}
					printf("oldphi[%i] = %f ",Xi,oldphi[Xi]);
				//printf("before stuck here? child %d \n", Xi);
			//for (i = 0; i < n; i++) {
				sigma = 0;
				for (j = 0; j < n; j++) {
					sigma =  j != Xi ? sigma + (A[Xi][j] * ptr[j]) : sigma;
					//printf("stuck here? child %d \n", Xi);
				}
				double aasd = ((1-omega)*oldphi[Xi])+  ((omega / A[Xi][Xi]) * (b[Xi]-sigma)); // test if we saved files to the shared memory
				printf("aas = %f  ",aasd);
				ptr[Xi] = ((1-omega)*oldphi[Xi]) + ((omega / A[Xi][Xi]) * (b[Xi]-sigma));
				printf(" ptr[%d] = %f ",Xi, ptr[Xi]);


			//calculate errors
			printf("almostend child %d \n", Xi);
				error[Xi] = fabs( ( ( ptr[Xi] - oldphi[Xi] )  / (ptr[Xi]) )  * 100 );
			
			// check for max error
			maxError = MIN_double;

			//for (m = 0; m < n; m++) {
				maxError = error[Xi] > maxError ? error[Xi] : maxError;
			//}

			currentError = maxError;
			// printf("%f \n", currentError); // current error
			
			/* remove the shared memory object */ 
			//shm_unlink(name);
			
		}
		//kill(pid,SIGTERM);
	printf("exiting child %d \n", Xi);
	}
	//kill(pid,SIGTERM);
		//kill(pid,SIGHUP);
}