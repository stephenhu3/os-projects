//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/types.h>
//#include <windows.h>
////#include <iostream>
//#include <tchar.h>
//#include <strsafe.h>
//#include <direct.h>
//#include <string.h>
//#include <conio.h>
//
//#define BUFFER_SIZE 25
//
//int main(VOID)
//{
//	HANDLE ReadHandle, WriteHandle, OutputHandle;
//	STARTUPINFO si;
//	PROCESS_INFORMATION pi;
//	DWORD read,written;
//	CHAR buffer[BUFFER_SIZE];
//	int i;
//
//	ReadHandle = GetStdHandle(STD_INPUT_HANDLE);//read from anon pipe
//
//	if (ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL)) {
//		printf("filter reads %s.", buffer);
//	} else {
//		fprintf(stderr, "Filter failed to read from anon pipe.");
//	}
//	// convert data/msg to from big to small and small to big 
//	for(i = 0 ; buffer[i] ; i++){
//		char c1 = buffer[i];
//		char c2;
//		
//		if(c1 >= 'a' && c1 <= 'z'){//small to big
//			c2 = ('A' + c1 - 'a');
//			//printf("%c\n",c2);
//			buffer[i]=c2;
//			//printf("%s\n",buffer);
//		}
//		else if(c1 >= 'A' && c1 <= 'Z'){//big to small
//			c2 = ('a' + c1 - 'A');
//			//printf("%c\n",c2);
//			buffer[i]=c2;
//			//printf("%s\n",buffer);
//		}
//	}
//
//	WriteHandle = CreateNamedPipe(L"\\\\.\\pipe\\my_pipe",
//		PIPE_ACCESS_DUPLEX,
//		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
//		1,
//		BUFFER_SIZE,
//		BUFFER_SIZE,
//		NULL,
//		NULL);
//
//
//	WriteFile(WriteHandle, &buffer, BUFFER_SIZE, NULL, NULL);
//	system("pause");
//	return 0;
//
//}

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define BUFFER_SIZE 25

int main(VOID){
	HANDLE Readhandle, WriteHandle;
	CHAR buffer[BUFFER_SIZE];
	DWORD read;
	int aac;

	/* get the read handle of the pipe */
	Readhandle = GetStdHandle(STD_INPUT_HANDLE);

	/* the child reads from the pipe */
	if (ReadFile(Readhandle, buffer, BUFFER_SIZE, &read, NULL)){
		printf("child read %s \n",buffer);

		for(aac = 0; aac < BUFFER_SIZE ; ++aac){
			char ch1 = buffer[aac];
			char ch2;
 
			  if(ch1 >= 'a' && ch1 <= 'z'){//small to big
					ch2 = ('A' + ch1 - 'a');
					buffer[aac]=ch2;

			
			  }
			  else if(ch1 >= 'A' && ch1 <= 'Z'){
					ch2 = ('a' + ch1 - 'A');
					buffer[aac]=ch2;

			  }
		}
		//sinkProcess(buffer[BUFFER_SIZE]);
		//fwrite(buffer, BUFFER_SIZE, 1, stdout);
	}

	WriteHandle = CreateNamedPipe(L"\\\\.\\pipe\\child",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		1,
		BUFFER_SIZE,
		BUFFER_SIZE,
		NULL,
		NULL);

	BOOL result;
	result = ConnectNamedPipe(WriteHandle",0);

	if (!result) {
		printf("Failed to make connection on named pipe.\n");
		// look up error code here using GetLastError()
		CloseHandle(Readhandle); // close the pipe
		system("pause");
		return 0;
	}


	WriteFile(WriteHandle, &buffer, BUFFER_SIZE, NULL, NULL);

	system("pause");
	return 0;
}