#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "binaryFunctions.h"

/*
Notes

  Have to specify function calls in binaryFunctions.h and here besides the
  actual binaryFunctions.c file.

gcc binary.c binaryFunctions.c userInput.c -o binary && ./binary

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

int test(int small, int large, int mid) {
  int realSum;
  int mainSum;
  int smallTwo = 0;
  int bad = 1;
  int total = 1;
  while (small <= large) {
    while (smallTwo <= mid) {
      realSum = small + smallTwo;
      mainSum = addition(small, smallTwo);
      if (realSum != mainSum) {
        bad++;
        //printf("%d + %d = %d != %d\n", small, smallTwo, realSum, mainSum);
      }
      smallTwo++;
      total++;
    }
    smallTwo = 0;
    //printf("%d ", small);
    small++;
  }
  printf("%d  %d\n", bad, total);
  return 0;
}

int main(int argc, char *argv[4]) {
  int small = 0;
  int large = 200;
  int mid = 200;
  int testOut = test(small, large, mid);
  /*
  int numberOne = 1;
  int numberTwo = 3;
  int division = subtraction(numberOne, numberTwo);
  */
  // int intInterface = interface(argc, argv[1]);
  return 0;
}
