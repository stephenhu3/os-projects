#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <tchar.h>
#include <strsafe.h>
#include <direct.h>
#include <string.h>
#include <conio.h>

#define BUFFER_SIZE 25

int main(VOID) {
	HANDLE ReadHandle;
	HANDLE WriteHandle;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char message[BUFFER_SIZE] = "BIG iS small";
	DWORD written;

	SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
	ZeroMemory(&pi, sizeof(pi));

	if (!CreatePipe(&ReadHandle, &WriteHandle, &sa, 0)) {
		fprintf(stderr, "CreatePipe failed.");
		return 1;
	}

	GetStartupInfo(&si);

	si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	si.hStdInput = ReadHandle;
	si.dwFlags = STARTF_USESTDHANDLES;

	SetHandleInformation(WriteHandle, HANDLE_FLAG_INHERIT, 0);

	//LPTSTR szCmdline = _tcsdup(TEXT("..\\..\\main\\Debug\\filter.exe")); <-- gives me error with this idk why

	//CreateProcess(NULL,"C:\\Users\\steven\\Desktop\\pipe\\filter\\Debug\\filter.exe" , NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	CloseHandle(ReadHandle);

	if (!WriteFile(WriteHandle, message, BUFFER_SIZE, &written, NULL)) {
		fprintf(stderr, "Failed to write to the anonymous pipe buddy.\n");
	}

	CloseHandle(WriteHandle);

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	system("pause");
	return 0;
}