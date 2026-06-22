#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "struct.h"
/*
#include "binaryChar.h"
#include "arithmatic.h"
#include "userInput.h"

// todo function for finding logarithm.
int logarithm(){
  return 0;
}
*/

// todo char string length.

struct threeInt charAppend(char firstChar[], int firstLength, char appendChar[], int appendLength, int space)  {

  int i = 0;
  int j = 0;
  int k = 0;
  // The total length of the returned char 'totalChar'.
  int totalLength = firstLength + appendLength;

  if (space == 1) {
    totalLength++;
  }
  // printf("totalLength:  %d\n", totalLength);
  printf("appendChar[]:  %s\n", appendChar);


  char *totalChar = malloc(totalLength * sizeof(char));
  strcpy(totalChar, "a");
  // 'i' keeps track of the 'totalChar'.
  // 'j' counts the 'firstChar' and is reset for the 'appendChar'.
  while (appendChar[k] != '\0') { // int k = 0; (appendChar[k] != '\0)  k++;
    if (i < firstLength) {
      totalChar[i] = firstChar[j];
      // printf("%d    %c  %d\n", i, firstChar[i], j);
      j++;
    }
    else {
      totalChar[i] = appendChar[k];
      // printf("%d    %c  %d\n", i, appendChar[k], k);
      k++;
    }
    i++;
  }
  if (space == 1) {
    totalChar[i] = ' ';
    i++;
  }
  // Null terminator.
  totalChar[i] = '\0';
  // printf("charAppend: %s\n", totalChar);
  // Returns a struct with length of 'totalChar' and the string.
  struct threeInt appendOutput;
  // appendOutput.one = totalLength - 1;
  // todo include '='


  appendOutput.one = totalLength - (appendLength - k);
  appendOutput.two = 0;
  appendOutput.three = 0;
  // The 'firstChar' with the 'appendChar'.
  appendOutput.string = totalChar;

  return appendOutput;
};
