#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "struct.h"
#include "binaryChar.h"
#include "arithmatic.h"
#include "arithmaticSteps.h"
#include "userInput.h"

// Specific math functions in "arithmaticSteps.c" for 'addition()' and 'subtraction()' independent
// of binary character transformations. Versions <= 0.03 'binaryAddition()' was in "binaryChar.c"
// and functions from "arithmaticSteps.c" were in "arithmatic.c".

// Inputs are two binary as char[] and results from 'intBinaryCheck()'.
// Output the sum as a binary char[] with the length.
struct threeInt binaryAddition(char binaryOne[], char binaryTwo[], struct threeInt intBinaryGreat) {
  // 'intBinaryGreat.one' length of longest binary char[];
  int i = intBinaryGreat.one - 1;
  // 'j' is the length of 'binarySum[]'.
  int j = 1;
  // 'k' is used to add one digit to binarySum[] for the 'carry'.
  int k = 0;
  // 'digitSum' is the sum of the aligned binary digits plus the carry.
  int digitSum = 0;
  int carry = 0;
  // 0 - 3 scale based on the 'carry' sorting for right shift 'binarySum[]' when 'i' is '0'.
  int category = 0;

  // The length of the longest binary int.
  // char *binarySum = malloc(17 * sizeof(char));
  // strcpy(binarySum, "a");
  char binarySum[17];

  int intDigitOne = 4;
  int intDigitTwo = 4; ///////////

  // Decimal addition is reversed
  while (i >= 0) {
    intDigitOne = 4; // might have to move these up.
    intDigitTwo = 4; //
    // Uses output from 'intBinaryCheck()' to align digits for addition.
    if (intBinaryGreat.three == 0) {
      intDigitOne = binaryOne[i] - '0';
      intDigitTwo = binaryTwo[i] - '0';
    }
    else if (intBinaryGreat.three == 1) {
      // binaryOne[] is longer and align the digits with the difference.
      intDigitOne = binaryOne[i] - '0';
      intDigitTwo = binaryTwo[i - intBinaryGreat.two] - '0';
    }
    else if (intBinaryGreat.three == 2) {
      // binaryTwo[] is longer and align the digits with the difference.
      intDigitOne = binaryOne[i - intBinaryGreat.two] - '0';
      intDigitTwo = binaryTwo[i] - '0';
    }
    // If either integers don't exist they are set to 0.
    if (intDigitOne == -48) {
      intDigitOne = 0;
    }
    if (intDigitTwo == -48) {
      intDigitTwo = 0;
    }

    // Add the binary digits from right to left.
    digitSum = intDigitOne + intDigitTwo + carry;

    // Calculate the carry.
    if (digitSum == 0) {
      // '0 + 0' is 0.
      category = 0;
      digitSum == 0;
    }
    else if (digitSum == 1) {
      // '1 + 0' or '0 + 1' is 1.
      category = 1;
      digitSum = 1;
      carry = 0;
    }
    else if (digitSum == 2) {
      // '1 + 1 with carry 0'.
      category = 2;
      digitSum = 0;
      carry = 1;
    }
    else if (digitSum == 3) {
      // '1 + 1 with carry 1' is 1.
      category = 3;
      digitSum = 1;
      carry = 1;
    }

    // Convert the binary digit sum to the char[].
    binarySum[i] = digitSum + '0';

    //printf("i:  %d   %d + %d = %d\n", i, intDigitOne, intDigitTwo, digitSum);

    // Shift 'binarySum[]' right if there are no more digits in 'i' and 'carry' is 1.
    if ( (i == 0) && ( (category == 2) || (category == 3) ) ) {
      k = j;
      char *currentBinarySum = malloc((k+1) * sizeof(char));
      strcpy(currentBinarySum, binarySum);
      while (k >= 0) {
        binarySum[k+1] = currentBinarySum[k];
        // printf("k: %d  %c  %c = %s\n", k, binarySum[k+1], currentBinarySum[k], binarySum);
        k--;
      }
      // Add the 'carry' to the left most [0] digit.
      binarySum[0] = '1';
      i++;
      // 'j' is incremented to the actual length for the null terminator below.
      j++;
      j++;
      break;
    }
    // 'j' is the length of the 'binarySum' - 1  and exists because 'i' is decrementing.
    j++;
    i--;
  }
  free(binaryOne);
  free(binaryTwo);
  // printf("%s + %s = %s\n", binaryOne, binaryTwo, binarySum);
  // Null terminate the char[].
  binarySum[j] = '\0';
  struct threeInt output;
  output.one = j - 1; //length of char binary addition answer
  output.two = 0;
  output.three = 0;
  output.string = malloc(j * sizeof(char));
  strcpy(output.string, binarySum);
  return output;
}

// Inputs are a 'charBinary[]', the length, and length of the longest binary in the subtraction.
// Returns 'charBinary[]' the length of the longest binary plus one zero (shorter would have more '0').
// i.e. '0100' and '0011'.
char *binaryPadding(char charBinary[], int binaryLength, int intBinaryGreatOne) {
  // The returned char[] is the length of the longest plus 2 for extra '0' and null terminator.
  char *charBinaryPadding = malloc((intBinaryGreatOne + 2) * sizeof(char));
  strcpy(charBinaryPadding, "a");
  int i = 0;
  int j = 0;
  // The number of leading zeros.
  int intBinaryPlus = (intBinaryGreatOne + 1) - binaryLength;
  // printf("intBinaryPlus %d\n", intBinaryPlus);
  while (i <= intBinaryGreatOne) {
    //if (charBinary[i] != 48 && charBinary[i] != 49) {
    // Stops '0' padding.
    if (i < intBinaryPlus) {
      charBinaryPadding[i] = '0';
    }
    else {
      // Switches to the char[] binary using 'j'.
      charBinaryPadding[i] = charBinary[j];
      // printf("%d  charBinary %c  %s\n", j, charBinary[j], charBinaryPadding);
      j++;
    }
    i++;
  }
  // Null terminator.
  charBinaryPadding[i] = '\0';
  return charBinaryPadding;
}

// The inverted char[] is returned.
char *binaryInvert(char *binaryTwoPadding, int intBinaryGreatOne) {
  char *binaryTwoNegative = malloc(intBinaryGreatOne + 2 * sizeof(char));
  strcpy(binaryTwoNegative, "a");
  int i = 0;
  while (binaryTwoPadding[i] != '\0') {
    // Inverts all '0' characters based on ASCII '48' = '0'.
    if (binaryTwoPadding[i] == 48) {
      binaryTwoNegative[i] = '1';
    }
    else if (binaryTwoPadding[i] == 49) {
      // Inverts all '1' characters.
      binaryTwoNegative[i] = '0';
    }
    else {
      //char *errorMessage = malloc(47 * sizeof(char));
      //strcpy(errorMessage, "Error: binaryTwoInvert() inputs neither 1 or 0");
      char errorMessage[47] =  "Error: binaryTwoInvert() inputs neither 1 or 0";
      printf("%s", errorMessage);
      int fileCheck = fileLogCheck();
      fileLog(errorMessage, fileCheck);
      // free(errorMessage);
    }
    i++;
  }
  // Add the null terminator.
  binaryTwoNegative[i] = '\0';
  return binaryTwoNegative;
}
