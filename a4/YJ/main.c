#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define BUFFER_SIZE 25

int main(VOID)
{
HANDLE ReadHandle, WriteHandle;
STARTUPINFO si;
PROCESS_INFORMATION pi;
char message[BUFFER_SIZE] = "Greetings\n";
TCHAR childPath[500] = TEXT("..\\..\\child\\Debug\\child.exe");
TCHAR filterPath[500] = TEXT("..\\..\\filter\\Debug\\filter.exe");
TCHAR sinkPath[500] = TEXT("..\\..\\sink\\Debug\\sink.exe");

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
/* establish the START INFO structure for the child process */
GetStartupInfo(&si);
si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
/* redirect standard input to the read end of the pipe */
si.hStdInput = ReadHandle;
si.dwFlags = STARTF_USESTDHANDLES;
/* don¡¯t allow the child to inherit the write end of pipe */
SetHandleInformation(WriteHandle, HANDLE_FLAG_INHERIT, 0);

/* create the child process */ //source.c
CreateProcess(NULL, childPath, NULL, NULL,
TRUE, /* inherit handles */
0, NULL, NULL, &si, &pi);

//HANDLE filter;
/* create the filter process */ //filter.c
//CreateNamedPipe(filterPath,
//				PIPE_ACCESS_DUPLEX |
//				FILE_FLAG_OVERLAPPED, 
//				PIPE_TYPE_BYTE| PIPE_WAIT, PIPE_UNLIMITED_INSTANCES,
//				BUFFER_SIZE, BUFFER_SIZE, NULL, NULL);

CreateProcess(NULL, filterPath, NULL, NULL,
TRUE, /* inherit handles */
0, NULL, NULL, NULL, &pi);

CreateProcess(NULL, sinkPath, NULL, NULL,
TRUE, /* inherit handles */
0, NULL, NULL, NULL, &pi);

//BOOL sink = ConnectNamedPipe(filter, NULL);
/* close the unused end of the pipe */
CloseHandle(ReadHandle);
/* the parent writes to the pipe */
if (!WriteFile(WriteHandle, message,BUFFER_SIZE,&written,NULL))
	fprintf(stderr, "Error writing to pipe.\n");
/* close the write end of the pipe */
CloseHandle(WriteHandle);
/* wait for the child to exit */
WaitForSingleObject(pi.hProcess, INFINITE);
CloseHandle(pi.hProcess);
CloseHandle(pi.hThread);
system("pause");
return 0;
}