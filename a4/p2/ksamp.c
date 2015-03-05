#include "ksamp.h"

//PARAMS:
//EFFECTS:
//RETURNS:
int main(int argc, char *argv[]) {
	int numProc;
	int mode = getMode(argc, argv);
	// printf("Detected mode: %i\n", mode);

	if(mode >= 1) {
		printf(GREEN "\nDefault Info...\n" RESET);

		getProcType();
		getKernalVersion();
		getTLastBoot();
	}

	if(mode >= 2) {
		printf(CYAN "\nAdvance Info...\n" RESET);

		getTStats();
		getDiskStats();
		getMiscStats();
	}

	if(mode == 3) {
		printf(RED "\nMemory Info...\n" RESET);

		getMemInfo();
		getLoadAvgs(atof(argv[2]), atof(argv[3]));
	}

	//printf("%i\n", numProc);
	printf("\n");
}

//PARAMS: argc (# of arguments), argv[] (array of said arguments)
//EFFECTS: identifies which version of the program the user wants to run based on arguments
//RETURNS: version of program to run (1, 2, 3) or exits if argument(s) invalid
int getMode(int argc, char *argv[]) {
	int i;

	switch(argc) {
		case 1:
			return VER1;
		case 2:
			if(!strcmp("-s", argv[1]))
				return VER2;
			else {
				printf("Error processing Argument: %s \nArgument is not supported or is missing parameters. \n", argv[1]);
				exit(BAD_MODE);
			}
		case 4: 
			if(!strcmp("-1", argv[1])) {
				for(i = 2; i < 4; i++)
					if(!atof(argv[i])) {
						printf("Bad parameters... usage of -1 requires numbers.\n");
						exit(BAD_MODE);
					}
				return VER3;
			}
			else {
				printf("Error processing Argument: %s \n Argument is not supported or is missing parameters. \n", argv[1]);
				exit(BAD_MODE);
			}
		default:
			printf("Invalid number of arguments.\n");
			exit(BAD_MODE);
	}
}

//PARAMS: none
//EFFECTS: prints processor type
//RETURNS: none
void getProcType(void) {
	char line[1024], label[128], info[1024];

	FILE *fp;
	fp = fopen("/proc/cpuinfo", "r");
	if(!fp) exit(NO_FILE);

	while(fgets(line, sizeof(line), fp) != NULL) {
		//Scan until the ':' for the label then scan after ':' until \n for info
		if(sscanf(line, "%[^:]: %[^\n]", label, info) > 0) {
			//printf("%s: ", label);
			//printf("%s\n", info);

			//strcmp doesn't work here as there are irregular spaces within the label... 
			//took me way too long to figure this out
			if(strstr(label, "processor")) {
				printf(GREEN "Processor #%s: " RESET, info);
			}
			if(strstr(label, "model name"))
				printf("%s\n", info);
		}
	}
	fclose(fp);
}

//PARAMS: none
//EFFECTS: prints kernal version
//RETURNS: none
void getKernalVersion(void) {
	char version[1024];

	FILE *fp;
	fp = fopen("/proc/sys/kernel/version", "r");
	if(!fp) exit(NO_FILE);
	fscanf(fp, "%[^\n]", version);

	printf(GREEN "Kernel Version: " RESET "%s\n", version);
	fclose(fp);
}

//PARAMS: none
//EFFECTS: prints time SINCE system was last booted (uptime) in seconds
//RETURNS: none
void getTLastBoot(void) {
	int uptime;

	FILE *fp;
	fp = fopen("/proc/uptime", "r");
	if(!fp) exit(NO_FILE);
	fscanf(fp, "%i", &uptime);

	printf(GREEN "Uptime: " RESET "%i seconds \n", uptime);
	fclose(fp);
}

//PARAMS: none
//EFFECTS: prints the amount of time the processor has spent in user mode system mode,
//	   the amount of time the system was idle
//RETURNS: none
void getTStats(void) {
	char line[1024], label[16];

	int userTime, sysTime, idleTime;

	int USER_HZ = sysconf(_SC_CLK_TCK);

	FILE *fp;
	fp = fopen("/proc/stat", "r");
	if(!fp) exit(NO_FILE);

	while(fgets(line, sizeof(line), fp) != NULL) {
		if(sscanf(line, "%s %i %*i %i %i", label, &userTime, &sysTime, &idleTime) > 0) {
			if(strstr(label, "cpu")) {
				userTime /= USER_HZ;
				sysTime /= USER_HZ;
				idleTime /= USER_HZ;

				printf(CYAN "%s: " RESET "User Time:%i System Time:%i Idle Time:%i (seconds)\n", label, userTime, sysTime, idleTime);
			}
		}
	}
	fclose(fp);
}

//PARAMS: none
//EFFECTS: prints disk reads/writes
//RETURNS: none
void getDiskStats(void) {
	char line[1024], label[16];

	int readsDone, readsMerged, writesDone, writesMerged;

	FILE *fp;
	fp = fopen("/proc/diskstats", "r");
	if(!fp) exit(NO_FILE);

	while(fgets(line, sizeof(line), fp) != NULL) {
		if(sscanf(line, "%*[^a-z]%s %i %i %*i %*i %i %i", label, &readsDone, &readsMerged, &writesDone, &writesMerged) > 0) {
			if(!strcmp(label, "sda")) {
				readsDone += readsMerged;
				writesDone += writesMerged;

				printf(CYAN "Disk reads/writes made on system: " RESET "%i/%i\n", readsDone, writesDone);
			}
		}
	}
	fclose(fp);
}

//PARAMS: none
//EFFECTS: number of context switches kernal has performed, time at which the system was booted, and the number of
//	   processes that have been created since the system was booted
//RETURNS: none
void getMiscStats(void) {
	char line[1024], label[16];

	int info;

	int USER_HZ = sysconf(_SC_CLK_TCK);

	FILE *fp;
	fp = fopen("/proc/stat", "r");
	if(!fp) exit(NO_FILE);

	while(fgets(line, sizeof(line), fp) != NULL) {
		if(sscanf(line, "%s %i", label, &info) > 0) {
			//context switches
			if(strstr(label, "ctxt")) {
				printf(CYAN "Kernel Context Switches: " RESET "%i\n", info);
			}

			//time since system was booted (since UNIX epoch)
			if(strstr(label, "btime")) {
				info /= USER_HZ;

				printf(CYAN "System Booted: " RESET "%i seconds (since the UNIX epoch) \n", info);
			}

			//number of processes created since the system was booted
			if(strstr(label, "processes")) {
				printf(CYAN "Processes Created: " RESET "%i\n", info);
			}
		}
	}
	fclose(fp);
}

//PARAMS: none
//EFFECTS: prints total and available memory
//RETURNS: none
void getMemInfo(void) {
	char line[1024], label[16], info[1024];

	FILE *fp;
	fp = fopen("/proc/meminfo", "r");
	if(!fp) exit(NO_FILE);

	while(fgets(line, sizeof(line), fp) != NULL) {
		if(sscanf(line, "%[^:]: %[^\n]", label, info) > 0) {
			if(strstr(label, "MemTotal") || strstr(label, "MemFree")) {
				printf(RED "%s: " RESET "%s\n", label, info);
			}
		}
	}
	fclose(fp);
}

//PARAMS: samplingRate and runTime (both are numbers and can have decimals)
//EFFECTS: samples load averages every samplingRate seconds for runTime seconds, prints
//	   each samplex
//RETURNS: none
void getLoadAvgs(double samplingRate, double runTime) {
	char load[16];
	double timeElapsed = 0;

	FILE *fp;
	
	printf(YELLOW "\nSampling load averages...\nSampling Rate: %f seconds\nSampling Time: %f seconds\n" RESET, samplingRate, runTime);

	while(timeElapsed < runTime) {
		fp = fopen("/proc/loadavg", "r");
		if(!fp) exit(NO_FILE);
		fscanf(fp, "%s", load);

		fclose(fp);
		printf(MAGENTA "%s " RESET, load);

		//we meet again fflush >.< why is it that every other printf is fine and yet this one is not
		fflush(stdout);

		usleep(samplingRate * 1000000);		
		timeElapsed += samplingRate;
	}	
}







