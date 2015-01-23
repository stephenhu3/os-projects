#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int deletions(char* stringRef);


int main() {
  int t,i;
  scanf("%d",&t);

  char inStr[100001];
  char* inStrPtr = inStr;


  for ( i = 0; i < t; i++ ) {
    scanf("%s", inStrPtr);
    printf("%d\n", deletions(inStrPtr));
  }

  return 0;
}



int deletions(char* inputRef) {
  int numDeletions = 0;
  int type; // 0 for A, 1 for B

  if (*inputRef == 'A') {
    type = 0;
  }
  else {
    type = 1;
  }


  while(*inputRef++) {
    if ((type == 0) && (*inputRef == 'A')) {
      numDeletions++; 
    } 
    else if ((type == 1) && (*inputRef == 'B')) {
      numDeletions++; 
    } else {
        type ^= 1;
    }
     
  }

  return numDeletions;
}
