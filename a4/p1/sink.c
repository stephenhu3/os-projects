#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 25

int main(VOID)
{
HANDLE Readhandle;
CHAR sinkbuff[BUFFER_SIZE];
DWORD read;

HANDLE pipe = CreateFile(
        L"\\\\.\\pipe\\my_pipe",
        GENERIC_READ, // only need read access
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

if (pipe == INVALID_HANDLE_VALUE) {
       printf("Failed to connect to pipe.");
        // look up error code here using GetLastError()
        system("pause");
        return 1;
    }
printf("Reading data from pipe \n");

char data[1024];
    DWORD numRead;

    ConnectNamedPipe(pipe, NULL);

    bool result = ReadFile(pipe, data, 1024, &numRead, NULL);

     if (result) {
        buffer[numBytesRead / sizeof(wchar_t)] = ''; // null terminate the string
        printf("Number of bytes read: " << numBytesRead << endl;
        printf("Message: %s \n");
    } else {
        printf("Failed to read data from the pipe.\n");
    }
	 CloseHandle(pipe);
	
//ConnectNamedPipe("\\\\.\\filter\\pipe",0);
//Readhandle = GetStdHandle(STD_INPUT_HANDLE); ;
//if (ReadFile(Readhandle, sinkbuff, BUFFER_SIZE, &read, NULL)){ //read from namepipe
//	printf("sink reads %s",sinkbuff);
	//fwrite(sinkbuff, BUFFER_SIZE, 1, stdout); //<-- print buffer, can use printf("Message: %s", buffer); too
//}
//else
//fprintf(stderr, "Error reading from pipe");
//system("pause");
return 0;
}