#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 25

int main(VOID)
{
HANDLE Readhandle;
CHAR sinkbuff[BUFFER_SIZE];
DWORD read;
BOOL result;

Readhandle = GetStdHandle(STD_INPUT_HANDLE);

if (ReadFile(Readhandle, sinkbuff, BUFFER_SIZE, &read, NULL)){ //read from namepipe
	printf("sink reads %s",sinkbuff);
	fwrite(sinkbuff, BUFFER_SIZE, 1, stdout); //<-- print buffer, can use printf("Message: %s", buffer); too
}
else
fprintf(stderr, "Error reading from pipe");
system("pause");
return 0;
}