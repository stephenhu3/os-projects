#include <windows.h>
#include <stdio.h>
DWORD fibSum; /* data is shared by the thread(s) */

/* the thread runs in this separate function */ 
DWORD WINAPI calculateFib(LPVOID nthFib) {
   DWORD n = *(DWORD*)nthFib;

   //implement fibonacci calculation iteratively
   DWORD sum1 = 0;
   DWORD sum2 = 1;
   DWORD preFibSum;
     
   if (n == 0) {
    preFibSum = 0;
   } else if (n == 1) {
    preFibSum = 1;
   } else {
      DWORD temp;

    for (DWORD i = 2; i <= n; i++) {
      temp = sum2;
      sum2 = sum1 + sum2; 
      sum1 = temp;
    }
    preFibSum = sum2;
  }

  fibSum = preFibSum;

  return 0;
}

int main(int argc, char *argv[]) {
  DWORD ThreadId;
  HANDLE ThreadHandle;
  int nthFib;

  if (argc != 2) {
    fprintf(stderr,"A single integer parameter is required\n"); 
    return -1;
  }
  nthFib = atoi(argv[1]);

  if (nthFib < 0) {
    fprintf(stderr,"An integer >= 0 is required\n");
    return -1;
  }

   /* create the thread */
   ThreadHandle = CreateThread(
     NULL, /* default security attributes */
     0, /* default stack size */
     calculateFib, /* thread function */
     &nthFib, /* parameter to thread function */
     0, /* default creation flags */
     &ThreadId); /* returns the thread identifier */

  if (ThreadHandle != NULL) {
      /* now wait for the thread to finish */
      WaitForSingleObject(ThreadHandle,INFINITE);
      
      /* close the thread handle */
      CloseHandle(ThreadHandle);

      printf("The %d fibonacci number is %d\n",fibSum); 
    }
}