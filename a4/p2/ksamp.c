#include "ksamp.h"

//PARAMS:
//EFFECTS:
//RETURNS:
int main(int argc, char *argv[]) {
	int numProc;

	printf("Version 1 Info... \n");

	numProc = getProcType();
	getKernalVersion();
	getTLastBoot();

	//printf("%i\n", numProc);

}

//PARAMS: none
//EFFECTS: prints processor type
//RETURNS: number of processors
int getProcType(void) {
	int numProc;

	char line[1024];
	char *label, *info;

	label = (char *) malloc(128 * sizeof(char));
	info = (char *) malloc(1024 * sizeof(char));

	FILE *fp;
	fp = fopen("/proc/cpuinfo", "r");

	while(fgets(line, sizeof(line), fp) != NULL) {
		//Scan until the ':' for the label then scan after ':' until \n for info
		if(sscanf(line, "%[^:]: %[^\n]", label, info) > 0) {
			//printf("%s: ", label);
			//printf("%s\n", info);

			//strcmp doesn't work here as there are spaces after the label... 
			//took me way too long to figure this out
			if(strstr(label, "processor")) {
				printf("Processor #%s: ", info);
				numProc = atoi(info);
			}
			if(strstr(label, "model name"))
				printf("%s\n", info);
		}
	}
	free(label);
	free(info);
	fclose(fp);

	return ++numProc;
}

//PARAMS: none
//EFFECTS: prints kernal version
//RETURNS: none
void getKernalVersion(void) {
	char version[128];

	FILE *fp;
	fp = fopen("/proc/sys/kernel/version", "r");
	fscanf(fp, "%[^\n]", version);

	printf("Kernel Version: %s\n", version);
	fclose(fp);
}

//PARAMS: none
//EFFECTS: prints time since system was last booted (uptime) in seconds
//RETURNS: none
void getTLastBoot(void) {
	int uptime;

	FILE *fp;
	fp = fopen("/proc/uptime", "r");
	fscanf(fp, "%d", &uptime);

	printf("Uptime: %d seconds \n", uptime);
	fclose(fp);
}
