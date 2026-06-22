#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "struct.h"
#include "binaryChar.h"
#include "arithmatic.h"

/*
Notes

gcc binary.c binaryFunctions.c -o binary && ./binary

  // The length of the binary characters (1 or 0) depends on 32 or 64 bit OS (aarch64).
  binaryLength bits (n + 1).

  Moving bits on hardware instead of 1 byte (8 bits) for each character.

  // C int uses less memory than the character arrays (4 vs 8)
  32    vs  16    bit operating system (aarch64) int uses
  4     vs  2     bytes
  +-2m  vs  +-32k numbers)
  C has built in functions for lower memory binary addition.
  Internet example uses bitwise operations similar to
  Assembly AND, XOR, Left Shift:

  // Using character arrays and more memory.
  The length of binary representations of decimals depends on 32 vs 64 bit OS.
  size_t i = sizeof(binaryOne) / sizeof(binaryOne[0]);
  Largest unsigned 64 bit number is 18,446,744,073,709,551,615 or 0xFFFFFFFFFFFFFFFF
  1111111111111111111111111111111111111111111111111111111111111111
  64 digits

  // Change or convert from or to int binary:
  Internet says to multiply 1 by 10 each iteration to store the binary or use bitwise.
  C max int on 32 bit is 2 billion and this method doesn't work to calculate
  numbers around 2000. Try and except or move to calculator.c for bitwise and int arrays.

  // Internet binary addtion example with bitwise operations.
  int binaryOne1 = 0b0000001;
  int binaryTwo2 = 0b0000011;
  while (binaryTwo2 != 0) {
    int carry = binaryOne1 & binaryTwo2;  // Finds the carry.
    binaryOne1 = binaryOne1 ^ binaryTwo2; // Adds the bits without the carry.
    binaryTwo2 = carry << 1;              // Shifts the carry left.
  }
  printf("binaryOne1 plus binaryTwo2 = %d\n", binaryOne1);
*/

// Input to the power of number. ->> moved to arithmatic.c
/*
int exponents(int base, int exponent) {
  int output = 1;
  int i = 1;
  while(i <= exponent){
    output *= base;
    //printf("%d  %d\n", i, output);
    i++;
  }
  return output;
}

*/

// Inputs int decimal and outputs int binary and binary digit length.
struct threeInt decimal2intBinary(int decimal, int binaryLength) {
  // binary is set to a value to avoid memory issues.
  int binary = 0;
  int remainder;
  int i = binaryLength;
  int shift = 1;
  // printf("Decimal: %d\n", decimal);
  while (i > 0) {
    remainder = decimal % 2;
    decimal = decimal / 2; // quotient
    // Internet says multiply by the remainder.
    binary = binary + (remainder * shift);
    if (decimal == 0) {
      // printf("Number of 0 after quotient is zero: %d\n", binaryLength - 1);
      break;
    }
    // C max int on 32 bit is 2 billion and this method doesn't work to calculate > 2000.
    shift *= 10;
    // printf("%d %d  %d  %d  %d        %d\n", i, decimal, remainder, binary, shift);
    i--;
  }
  struct threeInt output;
  output.one = binaryLength - (i - 1);
  // int binary.
  output.two = binary;
  // Place holder value.
  output.three = 0;
  output.string = '\0';
  return output;
}

// Convert int binary to char[] binary.
char* intBinary2Char(struct threeInt intZerosBinary) {
  // char pointer holds the binaryLength.
  char *charBinary = malloc(intZerosBinary.one * sizeof(char));
  strcpy(charBinary, "a");
  // i is decremented for the duration of the loop.
  charBinary[intZerosBinary.one] = '\0';
  int i = intZerosBinary.one - 1;
  int intBinaryDigit = 0;
  while(i >= 0) {
    // Reverse intZerosBinary.two as a char.
    // todo 'int intBinaryDigit = 0;' and see if it compiles.
    // use in 'userInput.c' -> 'inputArithmatic'.
    // int intBinaryDigit = intZerosBinary.two % 10;
    intBinaryDigit = intZerosBinary.two % 10;
    charBinary[i] = intBinaryDigit + '0';
    intZerosBinary.two /= 10;
    // printf("%d  %d    %c\n", i, intBinaryDigit, charBinary[i]);
    i--;
  }
  // change 'charbinary[i] = '\0';' and make sure the malloc is correct.
  // printf("intBinary2Char:  %s\n", charBinary);
  return charBinary;
}

// Returns length of the longest binary digits, difference, and check value used in binaryAddition().
struct threeInt intBinaryCheck(struct threeInt intBinaryOne, struct threeInt intBinaryTwo) {
  struct threeInt output;
  // int intBinaryLength = 0;
  // output.one = 0;
  // Stays the same if int binary is the same length.
  if (intBinaryOne.one == intBinaryTwo.one) {
    // Length of the longest binary.
    output.one = intBinaryOne.one + 1;
    // Difference between lengths.
    output.two = 0;
    // Marks which output value is equal, greater, or less than.
    output.three = 0;
    output.string = '\0';
  }
  else if (intBinaryOne.one > intBinaryTwo.one) {
    output.one = intBinaryOne.one;
    output.two = intBinaryOne.one - intBinaryTwo.one;
    output.three = 1;
    output.string = '\0';
  }
  else if (intBinaryOne.one < intBinaryTwo.one) {
    output.one = intBinaryTwo.one;
    output.two = intBinaryTwo.one - intBinaryOne.one;
    output.three = 2;
    output.string = '\0';
  }
  return output;
}

// Adds two char binary. Returns the addition.
struct threeInt binaryAddition(char binaryOne[], char binaryTwo[], struct threeInt intBinaryGreat) {
  // Allocates memory that is later deallocated with free().
  char *binarySum = malloc(intBinaryGreat.one * sizeof(char));
  strcpy(binarySum, "a");
  // 'carry' is used in binary addtion.
  int carry = 0;
  // The length of the longest binary is decremented.
  int i = intBinaryGreat.one - 1;
  // Null terminate the sum.
  binarySum[intBinaryGreat.one] = '\0';
  // Used to convert binary to decimal in another function.
  int binarySumLength = intBinaryGreat.one - 1;
  int intBinary = 1;
  // printf("binaryAddition char input: %s %s\n", binaryOne, binaryTwo);
  // printf("length  difference  check: %d   %d   %d\n", intBinaryGreat.one, intBinaryGreat.two, intBinaryGreat.three);

  // 'i' is decremented starting at the length of the longest binary length.
  while (i >= 0) {
    // Convert char arrays to int for addition.
    int binaryOne3;
    int binaryTwo3;
    // Checks if the lengths are the same with '.three' and aligns the
    // last binary digit using the difference in length 'intBinaryGreat.two'.
    if (intBinaryGreat.three == 0) {
      // " -'0' " converts char array to int with ascii ordering.
      binaryOne3 = binaryOne[i - 1] - '0';
      binaryTwo3 = binaryTwo[i - 1] - '0';
    }
    else if (intBinaryGreat.three == 1) {
      binaryOne3 = binaryOne[i] - '0';
      // If 'binaryOne' is longer, 'binaryTwo' char array iterator 'i' is
      // moved back the 'difference' to align the digits for the binary addition step.
      binaryTwo3 = binaryTwo[i - intBinaryGreat.two] - '0';
    }
    else if (intBinaryGreat.three == 2) {
      // If the other is longer, the opposite occurs.
      binaryOne3 = binaryOne[i - intBinaryGreat.two] - '0';
      binaryTwo3 = binaryTwo[i] - '0';
    }
    // printf("binaryAddition int input: %d   %d\n", binaryOne3, binaryTwo3);

    // If one is longer than the other, '-48' is returned on the shorter
    // int and that binary digit is set to 0.
    if ( (binaryOne3 != 1 && binaryOne3 != 0) && (binaryTwo3 != 1 && binaryTwo3 != 0) ) {
      binaryOne3 = 0;
      binaryTwo3 = 0;
    }
    else if (binaryOne3 != 1 && binaryOne3 != 0) {
      binaryOne3 = 0;
    }
    else if (binaryTwo3 != 1 && binaryTwo3 != 0) {
      binaryTwo3 = 0;
    }
    // printf("after setting NULL: %d    %d %d\n", i, binaryOne3, binaryTwo3);
    // Add the bits.
    int binaryAdd = binaryOne3 + binaryTwo3;
    // Convert int back to char[].
    // The first if handles edge cases '100', '1000', '10000' that adds an extra digit
    // to the length of digits after using the rest to calculate the carry with the
    // added binary digits.
    // WARNING probably doesn't work for 4 to 6 digits.
    if (binaryAdd == 1 && carry == 1 && i == 0) {
      int j = intBinaryGreat.one;
      binarySumLength = intBinaryGreat.one;
      while (j >= 0) {
        if (j == intBinaryGreat.one) {
          binarySum[j] = '0';
        }
        else if (j == 0) {
          binarySum[j] = '1';
        }
        // printf("1 plus 0*n: %d  %c\n", j, binarySum[j]);
        j--;
      }
    }
    else if (binaryAdd == 1 && carry == 0) {
      binarySum[i] = binaryAdd + '0';
      carry = 0;
    }
    else if (binaryAdd == 0 && carry == 1) {
      binaryAdd = 1;
      binarySum[i] = binaryAdd + '0';
      carry = 0;
    }
    else if (binaryAdd == 1 && carry == 1) {
      binaryAdd = 0;
      binarySum[i] = binaryAdd + '0';
      carry = 1;
    }
    else if (binaryAdd == 0 && carry == 0) {
      binarySum[i] = binaryAdd + '0';
      carry = 0;
    }
    else if (binaryAdd == 2 && carry == 0) {
      binaryAdd = 0;
      binarySum[i] = binaryAdd + '0';
      carry = 1;
    }
    else if (binaryAdd == 2 && carry == 1) {
      binaryAdd = 1;
      binarySum[i] = binaryAdd + '0';
      carry = 1;
    }
    else {
      binarySum[i] = '0';
    }
    // printf("%d    %d    %d + %d = %d    %s\n\n", i, carry, binaryOne3, binaryTwo3, binaryAdd, binarySum);
    i--;
  }
  free(binaryOne);
  free(binaryTwo);
  struct threeInt output;
  output.one = binarySumLength;
  output.two = 0;
  output.three = 0;
  output.string = malloc(binarySumLength * sizeof(char));
  strcpy(output.string, binarySum);
  free(binarySum);
  // printf("Binarysum struct %s\n", output.string);
  return output;
}

// This step is not needed since binary2Decimal() accesses each binary digit as a char[].
// Convert char[] binary to int binary.
// Internet uses bitwise left shift and OR in a loop.
int charBinary2Int(char binaryAdd[], int intBinaryGreatOne) {
  // printf("binary: %s\n", binary);
  int intBinaryPlus = 1;
  // The length of the longest binary plus 1 in case the first digit
  // has a carry (the last in the calculation from 'binaryAddition()').
  int j = intBinaryGreatOne;
  int i = 0;
  while (i < j) {
    // intBinaryPlus is multiplied by 10 to make room for each digit from the char[] iteration.
    intBinaryPlus = (intBinaryPlus * 10) + (binaryAdd[i] - '0');
    // printf("%d  binary:  %c  intBinaryPlus:  %d\n", i, binary[i], intBinaryPlus);
    i++;
  }
  free(binaryAdd);
  // The internet suggested to calculate log10 of the number to find the length
  // (which was calculated before) and proceed with the following steps.
  // Writing a log(n) function would reduce memory storage each time the length struct is
  // needed but increase iterations.

  // The first digit '1' is removed with 10 to the power of the binary length.
  int lengthPower = exponents(10, intBinaryGreatOne);
  // intBinaryPlus removes the first digit using the remainder.
  int intBinary = intBinaryPlus % lengthPower;
  // printf("intBinary:  %d\n", intBinary);
  return intBinary;
}

// Convert binary to decimal int.
int binary2Decimal(int binary, int intBinaryGreatOne, int binaryAddOne) {
  int decimal = 0;
  int i = 0;
  int j;
  // j is the digit length for '100' '1000' or '10000' that needed extra length in binaryAdd().
  if (intBinaryGreatOne == binaryAddOne) {
    j = intBinaryGreatOne;
  }
  else {
    j = intBinaryGreatOne - 1;
  }
  // Convert int binary to char binary.
  struct threeInt intBinaryAdd;
  intBinaryAdd.one = intBinaryGreatOne;
  intBinaryAdd.two = binary;
  intBinaryAdd.three = 0;
  char *charBinaryAdd = intBinary2Char(intBinaryAdd);
  // printf("%s \n", charBinaryAdd);
  while (i < intBinaryGreatOne) {
    decimal = decimal + ((charBinaryAdd[i] - '0') * (exponents(2, j)));
    // printf("%d binary2Decimal: %c      %d\n", i, charBinaryAdd[i], decimal);
    i++;
    j--;
    //printf("%d\n", j); j might have to start as 0.
  }
  // printf("decimal: %d\n", decimal);
  return decimal;
}
