#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



// n symbolic constant between 3 to 6
#define n 3
#define MAX_FLOAT 100000
#define MIN_FLOAT 0

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

float * solveSystem(float A[n][n], float b[n]);

int main() {
	float A[3][3] = { { 12.0, 3.0, -5.0 }, { 1.0, 5.0, 3.0 }, { 3.0, 7.0, 13.0 } };
	float b[3] = { 1, 28, 76 };

	int i;
	for (i = 0; i < n; i++) {
		printf("%f \n", *(solveSystem(A, b)+i));
	}

	return 0;
}

float * solveSystem(float A[n][n], float b[n]) {

	float error[n];

	float maxError = MIN_FLOAT;
	float currentError = MAX_FLOAT;

	/* relaxation factor must be 0 < omega < 2
	   check convergence: the new value is same as previous value
	   accomplish this by checking for approximate error between 
	   new value and old value, the max should be close to 0 */

	static float phi[n];
	int i;
	for (i = 0; i < n; i++)
		phi[i] = 0; // set initial guess to zeroes

	float oldphi[n];
	for (i = 0; i < n; i++)
		oldphi[i] = 0; // set initial guess to zeroes
	// Alternatively, we can clear to zeroes using calloc

	while (currentError > 0.0001) {
		

		float sigma;
		/* set to greater than 1 for speeding up convergency of a 
		slow-converging process, while values less than 1 are used to help 
		establish convergence of a diverging iterative process or speed up 
		convergence of an overshooting process */
		float omega = 1.75;
		int j;

		int k;
		for (k = 0; k < n; k++)
				oldphi[k] = phi[k]; // old phi has previous values

		for (i = 0; i < n; i++) {
			sigma = 0;
			for (j = 0; j < n; j++) {
				if (j != i)
					sigma = sigma + (A[i][j] * phi[j]);
			}
			
			phi[i] = (1-omega)*phi[i] + (omega / A[i][i]) * (b[i]-sigma);

		}

		/* for debugging purposes
		printf("oldphi: \n");
		for (k = 0; k < n; k++) {
			printf("%f \n", oldphi[k]); // old phi has previous values
		}

		printf("phi: \n");
		for (k = 0; k < n; k++) {
			printf("%f \n", phi[k]); // new calculated phi values
		} */
		

		//calculate errors
		int m;
		for (m = 0; m < n; m++) {
			error[m] = ((phi[m]-oldphi[m])/(phi[m])) * 100;
		}
		
		/* for debugging purposes
		printf("errors: \n");
		for (k = 0; k < n; k++) {
			printf("%f \n", error[k]); // new calculated phi values
		} */


		// check for max error
		maxError = MIN_FLOAT;

		for (m = 0; m < n; m++) {
			if (error[m] > maxError)
				maxError = error[m];
		}

		currentError = maxError;
		
	}
	return phi;
}
