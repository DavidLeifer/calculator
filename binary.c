#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "binaryFunctions.h"
/*
Notes

Migrated to ~/Desktop/calculator200/

  Have to specify function calls in binaryFunctions.h and here besides the
  actual binaryFunctions.c file.

  addition() - 1 + 3 was printed 0 since 4 is '100' and needed an extra binary digit.

gcc binary.c binaryFunctions.c -o binary && ./binary

*/
const char version[4] = "0.01";
// Put the subtraction - division before the textGUI to eliminate the warnings.
int subtraction(int integerOne, int integerTwo) {
   printf("Difference:  %d - %d = ?\n", integerOne, integerTwo);
  // struct threeInt ___ decimal2IntBinary(int decimal int binaryLength);
  // char* intBinary2Char(struct threeInt intZerosBinary);

  // internet says computers don't have a subtraction sign.
  // same length
  // invert all characters
  // add together

  // 0 - 0 = 0
  // 1 - 0 = 1
  // 1 - 1 = 0
  // 0 - 1 = 1  <- borrows 1 from the next left column

  // int intDecimalDiff = binarySubtraction();
  int intDecimalDiff = 0;

  return intDecimalDiff;
}
int multiplication(int integerOne, int integerTwo) {
  printf("Multiplication\n");
  return 0;
}
int division(int integerOne, int integerTwo) {
  printf("Division\n");
  return 0;
}

int main(int argc, char *argv[4]) {
  int numberOne = 1;
  int numberTwo = 3;
  int division = subtraction(numberOne, numberTwo);

  /*
  // todo function
  // The calculator text interface is shown if terminal doesn't ask for help.
  if (argc < 2) {
    textGUI();
    return 1;
  }
  // Allows for user designated input i.e. '-help' or '-h' etc.
  int i = 0;
  // Find the length of the input.
  while (argv[1][i] != '\0') {
    i++;
  }
  //printf("\n%c  %d\n", argv[1][1], i);
  if (argv[1][1] == 'h' && i == 5) {
    printf("Text Calculator\nVersion ");
    printf("%s Help\n\n", version);
    printf("Features: addition.\n");
  }
  else if (argv[1][1] == 'h' && i == 2) {
    printf("Text Calculator\nVersion ");
    printf("%s Help\n\n", version);
    printf("Features: addition.\n");
  }
  else {
    printf("Incorrect input. Exiting.\n");
  }
  */
  return 0;
}
