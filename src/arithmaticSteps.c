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
  int freeCheck = 3;
  struct threeInt binaryAdd = binaryAddition(binaryOne, binaryTwo, intBinaryGreat, freeCheck);
  //free(binaryOne);
  //free(binaryTwo);

  // Converts the char[] binary sum to decimal.
  int intDecimalAdd = binary2Decimal(binaryAdd.string, binaryAdd.one);
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

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // if inputs to 'binaryAddition()' aren't free(), then remove them from heap here.
  int freeCheck = 3;
  struct threeInt binaryInvertPlus = binaryAddition(binaryPlusOnePadding, binaryTwoInvert, intBinaryGreat2, freeCheck);

  //free(binaryPlusOnePadding);
  //free(binaryTwoInvert);

  // printf("binaryInvertPlus: %s\n", binaryInvertPlus.string);
  // 3) Add the numbers together.
  struct threeInt binaryDifference = binaryAddition(binaryOnePadding, binaryInvertPlus.string, intBinaryGreat2, freeCheck);
  //free(binaryOnePadding);
  //free(binaryInvertPlus.string);

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
  int integerOneSwap;
  int integerTwoSwap;
  if (integerOne < integerTwo) {
    integerOneSwap = integerTwo;
    integerTwoSwap = integerOne;
  }
  // Otherwise stays the same.
  else {
    integerOneSwap = integerOne;
    integerTwoSwap = integerTwo;
  }
  struct threeInt intBinaryOne = decimal2intBinary(integerOneSwap, maxBinaryLength);
  struct threeInt intBinaryTwo = decimal2intBinary(integerTwoSwap, maxBinaryLength);
  // 'decimal2intBinary()' '.one' returns the length and '.two' is the 'intBinary'
  //printf("intBinaryOne.one length: %d\nintBinaryTwo.one length: %d\n", intBinaryOne.one, intBinaryTwo.one);
  //printf("intBinaryOne.two: %d\nintBinaryTwo.two: %d\n", intBinaryOne.two, intBinaryTwo.two);
  if (intBinaryTwo.one == 1) {
     return integerOneSwap;
  }
  // Convert 'binary' to char[] for 'binaryAddition()'.
  char *binaryOne = intBinary2Char(intBinaryOne); // i.e. outputs: char binaryOne[] = "100";
  char *binaryTwo = intBinary2Char(intBinaryTwo); // i.e. outputs: char binaryOne[] = "11";
  // Returns length of the longest of the two binary digits, difference, and check value used in 'binaryAddition()'.
  struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);

  int i = intBinaryTwo.one - 1;
  int j = 1;
  int k = 1;
  int m = 0;
  // 'binaryOne' length is returned in 'decimal2intBinary()' and is used to increment the 'zeroStep' which
  // is the length of the previous binary plus one with all the 'char' set to '0'.
  int notZero = intBinaryOne.one;
  // 'stepOneCheck' is incremented when the multiplier 'binaryTwo' digit is '0' and is used to tack more '0' onto the end of
  // 'binaryOne'. This occurs when 'binaryTwo' right most digit is not '1' and 'oneStep[]' has to be initialized
  // with 'binaryOne' plus more '0'.
  int stepOneCheck = 0;
  int stepZeroCheck = 0;
  // Step 2) Set to '1' at the end of 'Step 0' or 'Step 1' if there are two consecutive '0' or '1' binary digits in a row.
  // Used in 'Step 3' for the first addition.
  int stepZeroExists = 0;
  int stepOneExists = 0;

  // 'binaryAddCheck' is set to '0' when 'zeroStep' is sent to 'binaryAddition()' in 'Step 3b)'.
  // It is set to '1' when 'oneStep' is sent to 'Step 3b'.
  int binaryAddCheck = 0;
  // The maximum length of 'oneStep' is 'intBinaryOne.one' plus 'intBinaryTwo.one' with room for '\0' since arrays start at zero.
  // i.e. binaryOne length: 5  binaryTwo length: 4  notOne: 9
  // i.e. '0' '1' '2' '3' '4'  '0' '1' '2' '3'      '0' '1' '2' '3' '4' '5' '6' '7' '8'
  // The other way is to declare both 'char oneStep' and 'zeroStep' in the while loop and resize with 'notOne', I'm not sure
  // if its harder on the CPU or silicon. If the assembly and C code from the compiler instructs electricity on the hardware at
  // the same spot and has to be burned off during clock cycles, it's relying on the hardware language to have the appropriate
  // buffer to avoid damaging the device. Its a different memory address each time a new char[] with the same name
  // is written in the loop which makes the memory size larger since it's a new sizeof() char[] + '1' each iteration instead of
  // a predetermined size.
  // It's harder on the CPU if you were to rewrite hardware code to overwrite existing memory at the same location without knowing
  // the exact clock cycles since it would overheat and crash the computer.
  int notOne = intBinaryOne.one + intBinaryTwo.one;
  char oneStep[notOne];
  oneStep[notOne] = '\0';
  char zeroStep[notOne];
  zeroStep[notOne] = '\0';
  char previousChar[notOne];

  ///////////////////////////
  // might not need to return the length of the char pointer
  // not sure if the struct is too large since its only a 'int' a 'char', and two blank 'int'
  ///////////////////////////
  struct threeInt binaryAdd;
  // 'intBinaryGreatTwo' is used in 'binaryAddition()'.
  struct threeInt intBinaryGreatTwo;

  while (i >= 0) {
    // 'notZero' is the length of the previous row plus one and is incremented at the end of the loop.
    // Step 0)
    // If the multiplier from right to left is '0'.
    // 1 X 0
    // 0 X 1
    // 0 X 0
    if (binaryTwo[i] == '0') {
      // Length of the previous number replaced with all '0' plus one '0'.
      // If you use a function to make an array with all '0' it's
      // extra heap memory with 'malloc()'.
      // Determines if there are two consecutive 'zeroStep' in a row for use in the first addition in 'Step 3'.
      if (stepZeroExists == 1) {
        m = 0;
        while (zeroStep[m] != '\0') {
          previousChar[m] = zeroStep[m];
          m++;
        }
        previousChar[m] = '\0';
        stepZeroExists++;
      }
      // If 'j' is '1' char 'zeroStep' has not been set to the first length of '0'.
      // Step 0a)
      if (i == (intBinaryTwo.one - 1)) {
        // 'j' is reset to '0' and is incremented to make a char[] of zeros the size of the muliplicand.
        j = 0;
        while (j < intBinaryOne.one) {
          zeroStep[j] = '0';
          j++;
        }
      }
      else if ((j == 1) && (stepZeroCheck > 0)) {
        // Step 0b)
        // The conditional handles the cases when '0' occurs after a one or more '0'.
        // 'j = 1' ensures that 'Step 0a)' has not been met and 'zeroStep' is empty and 'stepZeroCheck'
        // counts the number of 'binaryTwo[i]' digits have occured in 'Step 1)' to include additional
        // '0' to the right (similar to the shift operand).
        j = 0;
        while (j < (intBinaryOne.one + stepZeroCheck)) {
          zeroStep[j] = '0';
          j++;
        }
      }
      else {
        // Step 0c)
        // 'notZero' counts the number of rows or the multiplier digits from right to left
        // plus the original multiplicand 'binaryOne' length 'intBinaryOne.one'.
        while (j < notZero) {
          // Since 'j' is only used once for the initial '0' out the length of the 'binaryOne[j]'
          // multiplicand, the value is reused to tack on the additional '0' recorded from 'notZero'.
          zeroStep[j] = '0';
          j++;
        }
      }
      // Null-terminate 'zeroStep' to send to the binary 'addition()' functions.
      zeroStep[j] = '\0';
      // 'stepOneCheck' is used to increment 'stepOne' if one or more 'stepZeros' occurs before.
      stepOneCheck++;
      // Step 1) 'stepZeroExists' to determine if there are two 'zeroStep' in a row, shifting the prior
      // 'zeroStep' to a new char 'previousStep' for the initial addition in 'Step 3'.
      if (i == (intBinaryTwo.one - 1)) {
        stepZeroExists++;
      }
      // Used to send 'zeroStep' to 'binaryAddition()' in 'Step 3b)'.
      ///////////////////////////////////////////
      // probably don't need 'binaryAddCheck' for the if/else since 'intBinaryGreatTwo.three' says which one is longer with '0' '1' '2'
      // but an int is one byte.
      binaryAddCheck = 0;
      //printf("%d  zeroStep: %s zeroStep length: %d\n", i, zeroStep, notZero);
    }
    // Step 1)
    // If the multiplier from right to left is '1'.
    else if (binaryTwo[i] == '1') {
      // Determines if there are two consecutive 'oneStep' in a row for use in the first addition in 'Step 3'.
      if (stepOneExists == 1) {
        m = 0;
        while (oneStep[m] != '\0') {
          previousChar[m] = oneStep[m];
          m++;
        }
        previousChar[m] = '\0';
        stepOneExists++;
      }
      // If 'k' is '1' char 'oneStep' has not been set to the muliplicand 'binaryOne'.
      // Step 1a)
      if (i == (intBinaryTwo.one - 1)) {
        // 'k' is reset to '0' and is incremented to establish the multiplicand 'binaryOne'
        // as 'oneStep[maxBinaryLength]'. 'intBinaryOne.one' is the 'binaryOne' length.
        k = 0;
        while (k < intBinaryOne.one) {
          oneStep[k] = binaryOne[k];
          k++;
        }
      }
      else if ((k == 1) && (stepOneCheck > 0)) {
        // Step 1b)
        // The conditional handles the cases when '1' occurs after a one or more '0'.
        // 'k = 1' ensures that 'Step 1a)' has not been met and 'oneStep' is empty and 'stepOneCheck'
        // counts the number of '0' that have occured in 'Step 0)' to include additional
        // '0' when the multiplier is '10', '1010', '110', etc.
        k = 0;
        while (k < (intBinaryOne.one + stepOneCheck)) {
          //printf("i: %d, binaryOne[k]: %c, k: %d\n", i, binaryOne[k], k);
          if (binaryOne[k] != '\0') {
            oneStep[k] = binaryOne[k];
          }
          else {
            oneStep[k] = '0';
          }
          k++;
        }
      }
      else {
        // Step 1c)
        // 'notZero' counts the number of rows or the multiplier digits from right to left
        // plus the original multiplicand 'binaryOne' length 'intBinaryOne.one'.
        while (k < notZero) {
          // Since 'k' is only used once for the initial 'binaryOne[k]' multiplicand
          // the value is reused to tack on the additional '0' recorded from 'notZero'.
          oneStep[k] = '0';
          k++;
        }
      }
      // 'stepZeroCheck' is used to increment 'stepZero' if one or more 'stepOne' occurs before.
      stepZeroCheck++;
      // 'stepOneExists' to determine if there are two 'oneStep' in a row, shifting the prior
      // 'oneStep' to a new char 'previousStep' for the initial addition in 'Step 3'.
      if (i == (intBinaryTwo.one - 1)) {
        stepOneExists++;
      }
      // Null-terminate 'oneStep' to send to the binary 'addition()' functions.
      oneStep[k] = '\0';
      // Used to send 'oneStep' to 'binaryAddition()' in 'Step 3b)'.
      binaryAddCheck = 1;
      //printf("%d   oneStep: %s  oneStep length: %d\n", i, oneStep, notZero);
    }
    else {
      printf("Step 0 or Step 1 conditional failed.\n");
    }

    // It's confusing because 'i' is counting backwards. If the multiplier or 'intBinaryTwo.one'
    // length is '5', 'i' counts five numbers '4, 3, 2, 1, 0' to read from right to left. 'i' is
    // the length of the longest binary minus 1 and the initial 'while' counts down until after
    // '0' iteration concludes.

    // Step 3a)
    // Adding the first two binary rows determined from steps 0 and 1 requires finding the initial two rows.
    // If the first two '0' occur from right to left 'previousChar' is used and vice versa for two '1'.









    // The last conditional calculates the first two rows if they are '0' and '1' or '1' and '0' based
    // on the value of 'stepZeroExists' or 'stepOneExists'.
    int freeCheck = 0;
    if (i == (intBinaryTwo.one - 2)) {
      //printf("first addition: %d\n  'previousSum': %s\n", i, previousChar);
      // Returns length of the longest of the two binary digits, difference, and check value used in 'binaryAddition()'.
      //struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
      // 'intBinaryGreat' can be avoided with the length of 'previousChar' (shorter), difference with the next binary (1),
      // and check value in a 'struct threeInt intBinaryGreat.
      //struct threeInt intBinaryGreatTwo;
      // The longest is the most recent char and the difference is '1'.
      if (stepZeroExists == 2) {
        intBinaryGreatTwo.one = j;
        // The difference is always 1.
        intBinaryGreatTwo.two = 1;
        // Check value '1' indicates the first or most recent char is larger.
        intBinaryGreatTwo.three = 1;
        //intBinaryGreatTwo.string = '\0';
        // Adds the two binary char and returns struct with char pointer binary sum and length.
        // 'binaryAddition()' no longer 'free()'s 'binaryOne' and 'binaryTwo' for this step.
        binaryAdd = binaryAddition(zeroStep, previousChar, intBinaryGreatTwo, freeCheck);
      }
      else if (stepOneExists == 2) {
        intBinaryGreatTwo.one = k;
        // The difference is always 1.
        intBinaryGreatTwo.two = 1;
        // Check value '1' indicates the first or most recent char is larger.
        intBinaryGreatTwo.three = 1;
        //intBinaryGreatTwo.string = '\0';
        // Adds the two binary char and returns struct with char pointer binary sum and length.
        // 'binaryAddition()' no longer 'free()'s 'binaryOne' and 'binaryTwo' for this step.
        binaryAdd = binaryAddition(oneStep, previousChar, intBinaryGreatTwo, freeCheck);
      }
      else if (stepZeroExists == 1 || stepOneExists == 1) {
        // 'zeroStep' is the first value and longer.
        if (binaryTwo[i] == '0') {
          intBinaryGreatTwo.one = j;
          // The difference is always 1.
          intBinaryGreatTwo.two = 1;
          // Check value '1' indicates the first or most recent char is larger.
          intBinaryGreatTwo.three = 1;
          // Adds the two binary char and returns struct with char pointer binary sum and length.
          // 'binaryAddition()' no longer 'free()'s 'binaryOne' and 'binaryTwo' for this step.
          binaryAdd = binaryAddition(zeroStep, oneStep, intBinaryGreatTwo, freeCheck);
        }
        // 'oneStep' is the first value and longer.
        else if (binaryTwo[i] == '1') {
          intBinaryGreatTwo.one = k;
          // The difference is always 1.
          intBinaryGreatTwo.two = 1;
          // Check value '1' indicates the first or most recent char is larger.
          intBinaryGreatTwo.three = 1;
          // Adds the two binary char and returns struct with char pointer binary sum and length.
          // 'binaryAddition()' no longer 'free()'s 'binaryOne' and 'binaryTwo' for this step.
          binaryAdd = binaryAddition(oneStep, zeroStep, intBinaryGreatTwo, freeCheck);
        }
        else {
          printf("Step 3a) failed to calculate the first sum with non-consecutive binary digits (i:8 '1', i:7 '0'.\n");
        }
      }
      //printf("%d First sum  =  %s\n", i, binaryAdd.string);
    }
    // Step 3b) Uses 'binaryAddCheck' to add 'binaryAdd.string' (the sum of the first two binary) with either 'zeroStep' or 'oneStep'.
    else if (i < (intBinaryTwo.one - 2)) {
      // Returns length of the longest of the two binary digits, difference, and check value for 'binaryAdditionTwo()'.
      //struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
      /////////////////////////////////////////////////////
      // or use the function since 'struct threeInt intBinaryGreatThree' has a blank char[] pointer on heap the entire while loop
      // in addition to 'struct threeInt binaryAdd' which also exists the entire time.
      // 'binaryAdd.string' and either 'zeroStep' or 'oneStep' are the same length
      intBinaryGreatTwo.one = notZero;
      intBinaryGreatTwo.two = 1;
      intBinaryGreatTwo.three = 1;
      if (binaryAdd.one == notZero) {
        intBinaryGreatTwo.three = 0;
      }
      // 'zeroStep' and 'oneStep' are not pointers and don't need 'free()'.
      freeCheck = 2;
      // 'binaryAddCheck' determines if 'zeroStep' or 'oneStep' was the latest calculated row to add onto 'binaryAdd.string'.
      if (binaryAddCheck == 0) {
        binaryAdd = binaryAddition(zeroStep, binaryAdd.string, intBinaryGreatTwo, freeCheck);
      }
      else if (binaryAddCheck == 1) {
        binaryAdd = binaryAddition(oneStep, binaryAdd.string, intBinaryGreatTwo, freeCheck);
      }
      else {
        printf("'Step 3b)' failed at 'binaryAddCheck'.\n");
      }
      //printf("i: %d  binaryAdd.string: %s\n", i, binaryAdd.string);
    }
    // 'notZero' is the length of 'binaryOne' plus one for every multiplier digit.
    notZero++;
    // The decrementer reads 'binaryTwo' from right to left i.e. '4, 3, 2, 1'.
    i--;
  }
  // Converts the char[] binary sum to decimal.
  // 'binaryAdd.string' is 'free()' in 'binary2Decimal()'.
  int intDecimalAdd =  binary2Decimal(binaryAdd.string, binaryAdd.one);
  //printf("%d x %d = %d\n", integerOne, integerTwo, intDecimalAdd);
  //free(binaryAdd.string);
  return intDecimalAdd;
}

int division(int integerOne, int integerTwo) {
  printf("Division\n");
  return 0;
}
