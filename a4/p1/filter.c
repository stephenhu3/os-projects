#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <windows.h>
//#include <iostream>
#include <tchar.h>
#include <strsafe.h>
#include <direct.h>
#include <string.h>
#include <conio.h>

#define BUFFER_SIZE 25

int main(VOID)
{
	HANDLE ReadHandle, WriteHandle,OutputHandle;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DWORD read,written;
	CHAR buffer[BUFFER_SIZE];
	int i;

	ReadHandle = GetStdHandle(STD_INPUT_HANDLE);//read from anon pipe

	if (ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL)) {
		printf("filter reads %s.", buffer);
	} else {
		fprintf(stderr, "Filter failed to read from anon pipe.");
	}
	// convert data/msg to from big to small and small to big 
	for(i = 0 ; buffer[i] ; i++){
		char c1 = buffer[i];
		char c2;
		
		if(c1 >= 'a' && c1 <= 'z'){//small to big
			c2 = ('A' + c1 - 'a');
			printf("%c\n",c2);
			buffer[i]=c2;
			printf("%s\n",buffer);
		}
		else if(c1 >= 'A' && c1 <= 'Z'){//big to small
			c2 = ('a' + c1 - 'A');
			printf("%c\n",c2);
			buffer[i]=c2;
			printf("%s\n",buffer);
		}
	}

	WriteHandle = CreateNamedPipe("\\\\.\\filter\\pipe",
		PIPE_ACCESS_OUTBOUND,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		BUFFER_SIZE,
		0,
		0,
		NULL);

	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	ZeroMemory(&pi, sizeof(pi));

	GetStartupInfo(&si);/* establish the START INFO structure for the child process */
	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdInput = ReadHandle;/* redirect standard input to the read end of the pipe */
	si.dwFlags = STARTF_USESTDHANDLES;

	SetHandleInformation(WriteHandle, HANDLE_FLAG_INHERIT, 0);

	//LPTSTR sinkPath = _tcsdup(TEXT("C:\\Users\\steven\\Desktop\\pipe\\sink\\Debug\\sink.exe"));
	//C:\Users\steven\Desktop\pipe\filter\Debug
	CreateProcess(NULL, "C:\\Users\\steven\\Desktop\\pipe\\sink\\Debug\\sink.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	WriteFile(WriteHandle, &buffer, BUFFER_SIZE, NULL, NULL);

	return 0;

}