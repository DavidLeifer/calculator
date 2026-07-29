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
  // If the second number is longer, swap the integers.
  int intgerOneSwap;
  int intgerTwoSwap;
  if (integerOne < integerTwo) {
    intgerOneSwap = integerTwo;
    intgerTwoSwap = integerOne;
  }
  // Otherwise stays the same.
  else {
    intgerOneSwap = integerOne;
    intgerTwoSwap = integerTwo;
  }
  struct threeInt intBinaryOne = decimal2intBinary(intgerOneSwap, maxBinaryLength);
  struct threeInt intBinaryTwo = decimal2intBinary(intgerTwoSwap, maxBinaryLength);
  // 'decimal2intBinary()' '.one' returns the length and '.two' is the 'intBinary'
  //printf("intBinaryOne.one length: %d\nintBinaryTwo.one length: %d\n", intBinaryOne.one, intBinaryTwo.one);
  printf("intBinaryOne.two: %d\nintBinaryTwo.two: %d\n", intBinaryOne.two, intBinaryTwo.two);

  // Convert 'binary' to char[] for 'binaryAddition()'.
  char *binaryOne = intBinary2Char(intBinaryOne); // outputs: char binaryOne[] = "100";
  char *binaryTwo = intBinary2Char(intBinaryTwo); // outputs: char binaryOne[] = "11";
  //printf("%s\n", binaryOne);
  // Returns length of the longest of the two binary digits, difference, and check value used in 'binaryAddition()'.
  struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
  //printf("char  binaryOne: %s\nchar  binaryTwo: %s\n", binaryOne, binaryTwo);
  //printf("intBinaryGreat.one longest: %d\n", intBinaryGreat.one);

  int i = intBinaryTwo.one - 1;
  int j;
  int k = 1;
  // 'binaryOne' length is returned in 'decimal2intBinary()' and is used to increment the 'zeroStep' which
  // is the length of the previous binary plus one with all the 'char' set to '0'.
  int notZero = intBinaryOne.one + 1;
  char oneStep[maxBinaryLength];
  // Two char additions for the iterator, for example:
  // 'i = 0' :: char one[length] = ;
  // char charOne[];
  // char charTwo[];
  int extraZeros = 0;
  int multipleZeros;
  int m;
  //printf("%d  %d\n", i, intBinaryGreat.one);
  //while (i < intBinaryGreat.one) {
  // Second number 'binaryOne' is the multiplier.
  while (i >= 0) {
    char zeroStep[notZero];
    //printf("The binary:  %d  %c  %c\n", i, binaryOne[i], binaryTwo[i]);
    // Step 0 ///////////////////////////////////////////////////////////////////////////
    // If the product results in '0' i.e.
    // 1 X 0
    // 0 X 1
    // 0 X 0
    if ((binaryTwo[i] == '0') || (binaryTwo[i] == '1' && binaryOne[i] == '0')) {
      // Length of the previous number plus one '0' filled with all '0'.
      // if you use a function to make an array with all '0' it's
      // extra heap memory with 'malloc()'.
      j = 0;
      //printf("zeroStep %d   notZero: %d\n", sizeof(zeroStep), notZero);
      while (j < notZero) {
        zeroStep[j] = '0';
        //printf("      zeroStep[j]: %c\n", zeroStep[j]);
        j++;
      }
      zeroStep[j] = '\0';
      ////////////////////////////////////////////////////
      // 'notZero' is the length of the previous '1' step.
      notZero = j + 1;
      // 'm' is incremented everytime 'binaryOne[i]' is '0'.
      extraZeros++;
      printf("%d   zeroStep: %s\n", i, zeroStep);
    }
    // Step 1) ///////////////////////////////////////////////////////////////////////////
    // If the product results in '1' i.e.
    // 1 X 1
    // Length of the first binary 'notZero' incremented every time 'binaryTwo[i]' is '1'.
    else if (binaryOne[i] == '1' && binaryTwo[i] == '1') {
      // If 'k' is '1' char 'oneStep' has not been set to the muliplicand 'binaryOne'.
      // Step 1a) ////////////////////////////////////////////////////////////////////////
      if (k == 1) {
        // 'k' is reset to '0' and is incremented.
        k = 0;
        while (k < (notZero - 1)) {
          oneStep[k] = binaryOne[k];
          k++;
        }
      }
      else {
        // Step 1b) //////////////////////////////////////////////////////////////////////
        // 'extraZeros' accounts for the extra '0' when the multiplier is '0'.
        //multipleZeros = extraZeros + oneStep;
        //m = 0;
        //while (m < multipleZeros) {
          // Since 'k' is only used once for the initial 'binaryOne[k]' multiplicand
          // the value is reused to tack on the additional '0' recorded from 'extraZeros'.
          //oneStep[k] = '0';
          //m++;
          //k++;
        //}
        //printf("m %d + oneStep %d = %d", extraZeros, oneStep, (extraZeros+oneStep));
        //char oneStep[notZero];
      }
      //printf("oneStep %s    k %d\n", oneStep, k);
      oneStep[k] = '0';
      // may or may not need the 'k++' depending on 'Step 1b)'
      // k++;
      if (i == 0) {
        oneStep[k] = '\0';
      }
      printf("%d   oneStep: %s\n", i, oneStep);
      notZero++;
    }
    else {
      printf("Step 0 or Step 1 conditional failed.\n");
    }
    // Step 1c)
    // The other case is if binaryOne[i] is longer with a '1' or '0'
      // if its a '0', the row is the length of the previous filled with '0' plus a '0'
      // else if its a '1', it's the multiplciant 'binaryOne[i]' with 'k+1' number of '0' after.

    // Step 2) //////////////////////////////////////////////////////////////////////////
    // Resize 'oneStep[17]' with a null terminator.
    //printf("%d  zeroStep: %s   oneStep: %s\n", i, zeroStep, oneStep);

    // Step 3) //////////////////////////////////////////////////////////////////////////
    // Add every two char binary together.
    // It's confusing because 'i' is counting backwards. If the multiplier or 'intBinaryTwo.one'
    // length is '5', 'i' counts five numbers '4, 3, 2, 1, 0' to read from right to left.
    // Adding the first two binary rows determined from steps 0 and 1 requires finding the
    // initial 'previousSum' with '5 - 2 = 3'.
    //if ((i + 1) % 2 == 0) { // return 0, 1, 3, 5 etc.
    if (i == (intBinaryTwo.one - 2)) {
      //printf("first addition: %d for 'previousSum'\n", i);
      /*
      // 'previousSum' hasn't been determined.
      char previousSum[] = char charOne + char charTwo;

      // Returns length of the longest of the two binary digits, difference, and check value used in 'bina>
      struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
      // Adds the two binary char and returns struct with char pointer binary sum and length.
      // 'binaryAddition()' also 'free()'s 'binaryOne' and 'binaryTwo'.
      struct threeInt binaryAdd = binaryAddition(binaryOne, binaryTwo, intBinaryGreat);
      */
    }
    else if (i != intBinaryTwo.one) {
      //printf("addition %d\n", i);
      /*
      previousSum = previousSum + charTwo;

      // Returns length of the longest of the two binary digits, difference, and check value used in 'bina>
      struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
      // Adds the two binary char and returns struct with char pointer binary sum and length.
      // 'binaryAddition()' also 'free()'s 'binaryOne' and 'binaryTwo'.
      struct threeInt binaryAdd = binaryAddition(binaryOne, binaryTwo, intBinaryGreat);
      */
    }
    i--;
  }

  // The final portion is the sum of every 'oneStep' and 'twoStep'.
  //int intBinaryProduct = binaryMultiplication();
  // Converts the char[] binary sum to decimal.
  //int intDecimalAdd =  binary2Decimal(binaryAdd.string, binaryAdd.one);

  return 0;
}

int division(int integerOne, int integerTwo) {
  printf("Division\n");
  return 0;
}
