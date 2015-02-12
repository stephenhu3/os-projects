#include "dining.h"

// Stephen Hu
// Adapted from a similar program I had previously coded when working with pthreads

typedef struct threaddata {
	int taken;
} threaddata;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // lock to synchronize access to the free flag
pthread_mutex_t wait_mutex = PTHREAD_MUTEX_INITIALIZER; // lock to synchronize access to the free flag
pthread_cond_t freeLock = PTHREAD_COND_INITIALIZER; // condition variable so that threads can wait for access to the fork
threaddata f[NPHILOSOPHERS]; // global initialization of forks

int main(int argc, char *argv[]) {
	pthread_t threads[NPHILOSOPHERS];
	int check;
	int i, j, k;

	for (k = 0; k < NPHILOSOPHERS; k++) {
		initializeFork(k);
	}

	for (i = 0; i < NPHILOSOPHERS; i++) {
		check = pthread_create(&threads[i], NULL, initializePhilosopher, i);
		if (check) {
			printf("ERROR; return code from pthread_create() is %d\n", check);
			exit(-1);
		}
	}

	for (j = 0; j < NPHILOSOPHERS; j++) {
		pthread_join(threads[j], NULL);
	}

	return 1;
}

void *initializePhilosopher(int id) {
	int eaten = 1;
	pthread_cond_init(&freeLock, NULL);

	if ((getLeftFork(id)) < getRightFork(id)) {
		for (eaten = 1; eaten <= NMEALS; eaten++) {
			nap();
			if (pickupFork(getLeftFork(id))) {
				printf(" Philosopher #%d: I am picking up my left fork! \n",
					id);
				pickupRightNow: nap();
				if (pickupFork(getRightFork(id))) {
					printf(
						" Philosopher #%d: I am picking up my right fork! \n",
						id);
				} else { // can't pick up right fork, wait until she can
					while (!pickupFork(getRightFork(id))) {
						printf(
							" Philosopher #%d: I'm now waiting to pick up my right fork! \n",
							id);
						pthread_mutex_lock(&wait_mutex);
						pthread_cond_wait(&freeLock, &wait_mutex);
						pthread_mutex_unlock(&wait_mutex);
					}
					pickupFork(getRightFork(id));
					printf(
						" Philosopher #%d: I am picking up my right fork! \n",
						id);
				}
				nap();
				printf(" Philosopher #%d: I am eating meal #%d! \n", id, eaten);
				if (eaten == NMEALS) {
					printf(
						ANSI_COLOR_GREEN "Philosopher #%d: Mhhm! I have eaten all of my meals!" 
						ANSI_COLOR_RESET "\n", id);
				}
				nap();
				returnForks(getLeftFork(id));
				printf(" Philosopher #%d: I am putting down my left fork! \n",
					id);
				pthread_mutex_lock(&wait_mutex);
				pthread_cond_signal(&freeLock);
				pthread_mutex_unlock(&wait_mutex);
				nap();
				returnForks(getRightFork(id));
				printf(" Philosopher #%d: I am putting down my right fork! \n",
					id);
				pthread_mutex_lock(&wait_mutex);
				pthread_cond_signal(&freeLock);
				pthread_mutex_unlock(&wait_mutex);
			}

			else { // could not pick up left fork, wait until she can
				while (!pickupFork(getLeftFork(id))) {
					printf(
						" Philosopher #%d: I'm now waiting to pick up my left fork! \n",
						id);
					pthread_mutex_lock(&wait_mutex);
					pthread_cond_wait(&freeLock, &wait_mutex);
					pthread_mutex_unlock(&wait_mutex);
				}

				pickupFork(getLeftFork(id));
				printf(
					" Philosopher #%d: I can now finally pick up my left fork! \n",
					id);
				printf(" Philosopher #%d: I am picking up my left fork! \n",
					id);
				goto pickupRightNow;
			}
		}
	} else {
		for (eaten = 1; eaten <= NMEALS; eaten++) {
			nap();
			if (pickupFork(getRightFork(id))) {
				printf(" Philosopher #%d: I am picking up my right fork! \n",
					id);
				pickupLeftNow: nap();
				if (pickupFork(getLeftFork(id))) {
					printf(" Philosopher #%d: I am picking up my left fork! \n",
						id);
				} else { // can't pick up left fork, wait until she can
					while (!pickupFork(getLeftFork(id))) {
						printf(
							" Philosopher #%d: I'm now waiting to pick up my left fork! \n",
							id);
						pthread_mutex_lock(&wait_mutex);
						pthread_cond_wait(&freeLock, &wait_mutex);
						pthread_mutex_unlock(&wait_mutex);
					}
					printf(
						" Philosopher #%d: I can now finally pick up my left fork! \n",
						id);
					pickupFork(getLeftFork(id));
					printf(" Philosopher #%d: I am picking up my left fork! \n",
						id);
				}
				nap();
				printf(" Philosopher #%d: I am eating meal #%d! \n", id, eaten);
				if (eaten == NMEALS) {
					printf(
						ANSI_COLOR_GREEN "Philosopher #%d: Mhhm! I have eaten all of my meals!" 
						ANSI_COLOR_RESET "\n", id);
				}
				nap();
				returnForks(getLeftFork(id));
				printf(" Philosopher #%d: I am putting down my left fork! \n",
					id);
				pthread_mutex_lock(&wait_mutex);
				pthread_cond_signal(&freeLock);
				pthread_mutex_unlock(&wait_mutex);
				nap();
				returnForks(getRightFork(id));
				printf(" Philosopher #%d: I am putting down my right fork! \n",
					id);
				pthread_mutex_lock(&wait_mutex);
				pthread_cond_signal(&freeLock);
				pthread_mutex_unlock(&wait_mutex);
			} else { // could not pick up right fork, wait until she can
				while (!pickupFork(getRightFork(id))) {
					printf(
						" Philosopher #%d: I'm now waiting to pick up my right fork! \n",
						id);
					pthread_mutex_lock(&wait_mutex);
					pthread_cond_wait(&freeLock, &wait_mutex);
					pthread_mutex_unlock(&wait_mutex);
				}
				pickupFork(getRightFork(id));
				printf(
					" Philosopher #%d: I can now finally pick up my right fork! \n",
					id);
				printf(" Philosopher #%d: I am picking up my right fork! \n",
					id);
				goto pickupLeftNow;
			}
		}
	}

	pthread_mutex_lock(&wait_mutex);
	pthread_cond_broadcast(&freeLock);
	pthread_mutex_unlock(&wait_mutex);
	pthread_exit(NULL);
}

void initializeFork(int id) {
	f[id].taken = 0;
}

// given a philosopher id (0 .. 4), return the index of her left fork
// non-circular ordered fork pickup to prevent deadlocks
int getLeftFork(int id) {
	if (id == 0) {
		return 3;
	} else if (id == 1) {
		return 2;
	} else if (id == 2) {
		return 1;
	} else if (id == 3) {
		return 4;
	} else {
		return 0;
	}
}

// given a philosopher id (0 .. 4), return the index of her right fork
// non-circular ordered fork pickup to prevent deadlocks
int getRightFork(int id) {
	if (id == 0) {
		return 2;
	} else if (id == 1) {
		return 1;
	} else if (id == 2) {
		return 4;
	} else if (id == 3) {
		return 0;
	} else {
		return 3;
	}
}

// given a philosopher id (0 .. 4), picks up her fork, returns 1 if successful pickup, 0 if not
int pickupFork(int id) {
	pthread_mutex_lock(&lock);
	if (!f[id].taken) {
		f[id].taken = 1;
		pthread_mutex_unlock(&lock);
		return 1;
	} else {
		pthread_mutex_unlock(&lock);
		return 0;
	}
}

// given a philosopher id (0 .. 4), drops her fork, returns 1 if successful drop, 0 if not
void returnForks(int id) {
	pthread_mutex_lock(&wait_mutex);
	f[id].taken = 0;
	pthread_cond_signal(&freeLock);
	pthread_mutex_unlock(&wait_mutex);
}

// naps for 1 to 3 seconds
void nap(void) {
	usleep(ONESEC_PERIOD + (random() % (2 * ONESEC_PERIOD))); // produces wait between 1 to 3 seconds
}
