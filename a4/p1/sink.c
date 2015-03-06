#include <stdio.h>
#include <windows.h>
#define BUFFER_SIZE 25

int main(VOID) // anonymous ...
{
HANDLE Readhandle;
CHAR buffer[BUFFER_SIZE]= "A2SfahACDef";// <-fortestonly
DWORD read;
int aac;

/* get the read handle of the pipe */
Readhandle = GetStdHandle(STD_INPUT_HANDLE);

/* the child reads from the pipe */
if (ReadFile(Readhandle, buffer, BUFFER_SIZE, &read, NULL)){
printf("child read %s \n",buffer);///////////////////////////////////////

for(aac = 0; aac < BUFFER_SIZE ; ++aac){
	char ch1 = buffer[aac];
	char ch2;	
 
      if(ch1 >= 'a' && ch1 <= 'z'){//small to big
            ch2 = ('A' + ch1 - 'a');
			printf("%c\n", ch2);
            buffer[aac]=ch2;
			printf("%s\n", buffer);

			
      }
      else if(ch1 >= 'A' && ch1 <= 'Z'){//big to small
            ch2 = ('a' + ch1 - 'A');
			printf("%c\n", ch2);
			buffer[aac]=ch2;
			printf("%s\n", buffer);

	  }
}
fwrite(buffer, BUFFER_SIZE, 1, stdout); // printf for buffer
//else{
//	fprintf(stderr, "Error reading from pipe");
//	}
}
system("pause");
return 0;
}