#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "struct.h"
#include "binaryChar.h"
#include "arithmatic.h"
#include "arithmaticSteps.h"
#include "userInput.h"

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
  // Doesnt work when the sum > 200 since char has limit ~2,000,000,000.
  // and the 'intBinary2Char' uses 'shift*10' to convert int to char.
  // int integerOne = 4;
  // int integerTwo = 3;
  // printf("Decimals: %d  %d\n", integerOne, integerTwo);
  // Have to specify the functions in this file and generically in binaryFunctions.h
  // to avoid compilation issues.
  // 17 bit binary 'maxBinaryLength' from "main.c" and "binaryChar.h" (n + 1) to
  // add the 'b' prefix if needed. 'const' might break other libraries.
  struct threeInt intBinaryOne = decimal2intBinary(integerOne, maxBinaryLength);
  struct threeInt intBinaryTwo = decimal2intBinary(integerTwo, maxBinaryLength);
  // Convert 'binary' to char[] for 'binaryAddition()'.
  char *binaryOne = intBinary2Char(intBinaryOne); // outputs: char binaryOne[] = "100";
  char *binaryTwo = intBinary2Char(intBinaryTwo); // outputs: char binaryOne[] = "11";
  // printf("%s\n", binaryOne);
  // Returns length of the longest of the two binary digits, difference, and check value used in 'binaryAddition()'.
  struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
  // Adds the two binary char and returns struct with char pointer binary sum and length.
  // 'binaryAddition()' also 'free()'s 'binaryOne' and 'binaryTwo'.
  struct threeInt binaryAdd = binaryAddition(binaryOne, binaryTwo, intBinaryGreat);
  // Converts the char[] binary sum to decimal.
  int intDecimalAdd =  binary2Decimal(binaryAdd.string, binaryAdd.one);
  return intDecimalAdd;
}

// Inputs two integers between '0-200' and returns the difference.
int subtraction(int integerOne, int integerTwo) {
  if (integerOne < integerTwo) {
    //char *errorMessage = malloc(47 * sizeof(char));
    //strcpy(errorMessage, "Error: binaryTwoInvert() inputs neither 1 or 0");
    char errorMessage[76] =  "Error: difference is negative, first number has to be >= the second number\n";
    printf("%s", errorMessage);
    int fileCheck = fileLogCheck();
    fileLog(errorMessage, fileCheck);
    // free(errorMessage);
  }
  /*
  internet says computers don't have a subtraction sign to use the
  same circuit loops for both arithmatic operations. steps:
  0) same length
  1) invert all characters of the second binary.
  2) add '~0001' to the second binary.
  3) add both binary together.
  4) drop the first '1' in the sum
  c = difference
  example:
  5 - 3 = 2
  0) a) 0101
     b) 0011
  1) b) 1100
  2) b) 1101
  3) c) 10010
  4) c) 0010
     c) = 2
  */

  // int realAnswer = integerOne - integerTwo;
  // printf("Difference:  %d - %d = %d\n", integerOne, integerTwo, realAnswer);

  // Steps to convert binaryOne and binaryTwo to char[] are repeated from 'addtion()'
  // and could probably be a function.
  // const 'maxBinaryLength' from "main.c" and "binaryChar.h"
  struct threeInt intBinaryOne = decimal2intBinary(integerOne, maxBinaryLength);
  struct threeInt intBinaryTwo = decimal2intBinary(integerTwo, maxBinaryLength);
  // Convert 'binary' to char[] for 'binaryAddition()'.
  char *binaryOne = intBinary2Char(intBinaryOne); // outputs: char binaryOne[] = "100";
  char *binaryTwo = intBinary2Char(intBinaryTwo); // outputs: char binaryOne[] = "11";
  // printf("%s\n", binaryOne);
  // Returns length of longest binary digits, difference, and
  // check value used in 'binaryPadding()' and 'binaryInvert()'.
  struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
  //printf("char binaryOne: %s  char binaryTwo: %s\n", binaryOne, binaryTwo);
  // Use the longest minus the difference for 'binaryTwo' length.
  int binaryOneLength = intBinaryGreat.one;
  int binaryTwoLength = intBinaryGreat.one - intBinaryGreat.two;
  // 0) Both binary numbers have '0' padding. The longest binary has
  // one '0' in the first character i.e. '0101'.
  char *binaryOnePadding = binaryPadding(binaryOne, binaryOneLength, intBinaryGreat.one);
  char *binaryTwoPadding = binaryPadding(binaryTwo, binaryTwoLength, intBinaryGreat.one);
  // printf("binaryOnePadding: %s\n", binaryOnePadding);
  // printf("binaryTwoPadding: %s\n", binaryTwoPadding);
  // 1) Invert the second number.
  char *binaryTwoInvert = binaryInvert(binaryTwoPadding, intBinaryGreat.one);
  // printf("binaryTwoInvert: %s\n", binaryTwoInvert);
  // 2) Add one to the second number.
  // Length of longest binary digits, difference, and
  // check value used in 'binaryAddition()'.
  struct threeInt intBinaryGreat2;
  intBinaryGreat2.one = intBinaryGreat.one + 1;
  intBinaryGreat2.two = 0;
  intBinaryGreat2.three = 0;
  // char *binaryPlusOne = malloc((intBinaryGreat2.one) * sizeof(char));
  // strcpy(binaryPlusOne, "1");
  // binaryPlusOne = "1 * intBinaryGreat2.one";
  char binaryPlusOne[intBinaryGreat2.one];
  int i = 0;
  while (i < intBinaryGreat2.one) {
    binaryPlusOne[i] = '1';
    i++;
  }
  binaryPlusOne[i] = '\0';
  char *binaryPlusOnePadding = binaryPadding(binaryPlusOne, 1, intBinaryGreat.one);
  struct threeInt binaryInvertPlus = binaryAddition(binaryPlusOnePadding, binaryTwoInvert, intBinaryGreat2);
  // printf("binaryInvertPlus: %s\n", binaryInvertPlus.string);
  // 3) Add the numbers together.
  struct threeInt binaryDifference = binaryAddition(binaryOnePadding, binaryInvertPlus.string, intBinaryGreat2);
  // printf("Subtraction binary: %s\n", binaryDifference.string);
  // printf("Length: %d\n", binaryDifference.one);
  // 4) Convert the leading '1' to '0' if it exists.
  if (binaryDifference.string[0] == 49) {
    binaryDifference.string[0] = '0';
  }
  // Converts the char[] binary sum to decimal from "binaryChar.c".
  int intDecimalAdd = binary2Decimal(binaryDifference.string, binaryDifference.one);
  return intDecimalAdd;
}

// Put the subtraction - division before the textGUI to eliminate the warnings.

int multiplication(int integerOne, int integerTwo) {
  printf("Multiplication\n");
  return 0;
}

int division(int integerOne, int integerTwo) {
  printf("Division\n");
  return 0;
}
