#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "binaryFunctions.h"


/*
Notes

  Have to specify function calls in binaryFunctions.h and here besides the
  actual binaryFunctions.c file.

gcc binary.c binaryFunctions.c -o binary && ./binary

*/

int main() {
  // int numberOne = 300;
  // int numberTwo = 300;
  // 1 + 3 printed 0 since 4 is '100' and needed an extra binary digit.
  int numberOne;
  int numberTwo;
  char options[2];
  printf("Text Based Calculator 1.0\n");
  printf("Options (press enter):\n-a  Addition\n-s  Subtraction\n-m  Multiplication\n-d  Division\n");
  scanf("%2s", options);
  // while loop checks if the input matches operand ('+', '-', '*', 'etc').
  int i = 1;
  int opCheck;
  char *additionChar = "-a";
  char *subtractionChar = "-s";
  while (i >= 1) {
    if (options[i] == additionChar[i]) {
      opCheck = 1;
    }
    else if (options[i] == subtractionChar[i]) {
      opCheck = 2;
    }
    else {
      opCheck = 0;
    }
    // printf("%c %c\n", options[i], subtractionChar[i]);
    i--;
  }
  if (opCheck == 0) {
    printf("Error\n");
  }
  else {
    printf("Type a number below 200 (press enter): ");
    scanf("%d", &numberOne);
    printf("Type a number below 200 (press enter): ");
    scanf("%d", &numberTwo);
    if (opCheck == 1) {
      printf("%\n", opCheck);
      int sum = addition(numberOne, numberTwo);
      printf("%d + %d = %d\n", numberOne, numberTwo, sum);
    }
    else if (opCheck == 2) {
      // int difference = difference(numberOne, numberTwo);
      printf("Subtraction\n");
    }
  }
}
