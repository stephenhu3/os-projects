#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <windows.h>
//#include "macros.h"
//#include <iostream>

#define BUFFER_SIZE 25

int main(VOID)
{
HANDLE ReadHandle, WriteHandle;
//TCHAR sourcePath[500] = TEXT("..\\..\\main\\Debug\\source.exe"); // filter child . need to implement filtering  ..
//TCHAR sinkPath[500] = TEXT("..\\..\\main\\Debug\\filter.exe");
STARTUPINFO si;
PROCESS_INFORMATION pi;
//char message[BUFFER_SIZE] = "make me BIG \n";
DWORD written;

/* set up security attributes allowing pipes to be inherited */
SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES),NULL,TRUE};
/* allocate memory */
ZeroMemory(&pi, sizeof(pi));

/* create the pipe */
if (!CreatePipe(&ReadHandle, &WriteHandle, &sa, 0)) {
	fprintf(stderr, "Create Pipe Failed");
	return 1;
}

GetStartupInfo(&si);/* establish the START INFO structure for the child process */
si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
si.hStdInput = ReadHandle;/* redirect standard input to the read end of the pipe */
si.dwFlags = STARTF_USESTDHANDLES;

/* don’t allow the child to inherit the write end of pipe */
SetHandleInformation(WriteHandle, HANDLE_FLAG_INHERIT, 0);

/* create the child process */
CreateProcess(NULL, "C:\\Users\\steven\\Desktop\\pipe\\source\\Debug\\source.exe", NULL, NULL,
TRUE, /* inherit handles */
0, NULL, NULL, &si, &pi);

/* close the unused end of the pipe */
//CloseHandle(ReadHandle);

/* the parent writes to the pipe */
//if (!WriteFile(WriteHandle, message,BUFFER_SIZE,&written,NULL))
//fprintf(stderr, "Error writing to pipe.");

/* close the write end of the pipe */
//CloseHandle(WriteHandle); not needed since we are calling not writting in this main 

/* wait for the child to exit */
WaitForSingleObject(pi.hProcess, INFINITE);
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);

system("pause");

return 0;

}