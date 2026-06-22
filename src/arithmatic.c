#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "struct.h"
#include "binaryChar.h"
#include "arithmatic.h"

// Input to the power of number.
int exponents(int base, int exponent) {
  int output = 1;
  int i = 1;
  while(i <= exponent){
    output *= base;
    // printf("%d  %d\n", i, output);
    i++;
  }
  return output;
}

int addition(int integerOne, int integerTwo) {
  // Input decimal int, output binary.
  // Doesnt work when the sum > 2000 since int has limit ~2,000,,000,000.
  // and the 'intBinary2Char' uses 'shift*10' to convert int to char.
  // int integerOne = 4;
  // int integerTwo = 3;
  // 8 bit binary (n + 1) to include the 'b' prefix. 'const' might break other libraries.

  int binaryLength = 17;
  // printf("Decimals: %d  %d\n", integerOne, integerTwo);
  // Have to specify the functions in this file and generically in binaryFunctions.h
  // to avoid compilation issues.
  struct threeInt intBinaryOne = decimal2intBinary(integerOne, binaryLength);
  struct threeInt intBinaryTwo = decimal2intBinary(integerTwo, binaryLength);
  // printf("%d  %d\n", intBinaryOne.one, intBinaryOne.two);
  // Convert 'binary' to char[] for 'binaryAddition()'.
  char *binaryOne = intBinary2Char(intBinaryOne); // outputs: char binaryOne[] = "100";
  char *binaryTwo = intBinary2Char(intBinaryTwo); // outputs: char binaryOne[] = "11";
  // printf("%s\n", binaryOne);
  // Returns length of the binary digits, difference, and check value used in 'binaryAddition()'.
  struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
  // Adds the two binary char and returns struct with char pointer sum and new int binary
  //  length check if the length is '10', '100', or '1000'.
  struct threeInt binaryAdd = binaryAddition(binaryOne, binaryTwo, intBinaryGreat);
  int intBinaryAdd = charBinary2Int(binaryAdd.string, intBinaryGreat.one);
  // printf("%d\n", intBinaryAdd);
  int intDecimalAdd =  binary2Decimal(intBinaryAdd, intBinaryGreat.one, binaryAdd.one);
  // printf("%d + %d = %d\n", integerOne, integerTwo, intDecimalAdd);
  return intDecimalAdd;
}

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
