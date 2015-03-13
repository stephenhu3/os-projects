#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// n symbolic constant between 3 to 6
#define n 3
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

// inputs: A, b, omega, sigma, phi


int sigma;
int omega = 2;// set to greatet than 1 for speeding up convergency of a slow-converging process, while values less than 1 are used to help establish convergence of a diverging iterative process or speed up convergence of an overshooting process
int i;
int j;
for (i = 1; i <= n; i++) {
	sigma = 0;
	for (j = 1; j <= n; j++) {
		if (j != i)
			sigma = sigma + (A[i][j] * phi[j]);
	}
	phi[i] = (1-omega)*phi[i] + (omega/A[i][i])*(b[i]-sigma);
}
