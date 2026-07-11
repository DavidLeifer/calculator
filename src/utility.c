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

// Inputs 'charInput[]' and the character position 'start' ('n - 1' i.e. '0')
// through last character 'end'. Have to use 'free()' on return 'charOutput'.
char *slice(char charInput[], int start, int end) {
  // printf("%s\n", charInput);
  int charOutputLength = end - start;
  char *charOutput = malloc(charOutputLength * sizeof(char));
  strcpy(charOutput, "a");
  int i = 0;
  while (start <= end) {
    // printf("%x ", charInput[start]);
    charOutput[i] = charInput[start];
    i++;
    start++;
    if (i == (end+1)) {
      break;
    }
  }
  charOutput[i] = '\0';
  //printf("\n");
  return charOutput;
}

// Returns the length of a char[].
int charLength(char charInput[]) {
  int i = 1;
  while (charInput[i] != '\0') {
    i++;
  }
  return i;
}

// Finds the number of digits in an integer through division.
int intLength(int intInput) {
  int lengthOutput = 0;
  while (intInput != 0) {
    intInput /= 10;
    lengthOutput++;
  }
  return lengthOutput;
}

// Appends a char[] to another char[] using lengths of both. The first
// length has to be known without the null terminator, the second
// can be longer and is explained below. '0' for no space at the end of
// the output char[] or '1' for space i.e. 'one '.
struct threeInt charAppend(char firstChar[], int firstLength, char appendChar[], int appendLength, int space)  {
  int i = 0;
  int j = 0;
  int k = 0;
  // The total length of the returned char 'totalChar'.
  int totalLength = firstLength + appendLength;
  // Length of 'totalChar' is incremented by one for an end space.
  if (space == 1) {
    totalLength++;
  }
  char *totalChar = malloc(totalLength * sizeof(char));
  strcpy(totalChar, "a");
  // 'i' keeps track of the 'totalChar'.
  // 'j' counts the 'firstChar'.
  // Loops for the length of 'appendChar[]'.
  while (appendChar[k] != '\0') {
    if (i < firstLength) {
      // The first if is broken by 'i' before the '\0'.
      totalChar[i] = firstChar[j];
      j++;
    }
    else {
      // 'k' increments 'appendChar'.
      totalChar[i] = appendChar[k];
      k++;
    }
    i++;
  }
  // One ' ' is included if 'space' equals '1'.
  if (space == 1) {
    totalChar[i] = ' ';
    i++;
  }
  // Null terminator.
  totalChar[i] = '\0';
  // Returns a struct with length of 'totalChar' and the string.
  struct threeInt appendOutput;
  // 'appendLength' may or may not be accurate and returns the
  // difference between 'appendLength' and 'appendLength' through
  // the null terminator '\0' subtracted from 'totalLength'.

  // The example below does not include a space at the end.
  // 'randomIt'  = 'random'    + 'It'
  // totalLength = firstLength + appendLength
  // 12          = 6           + 6
  // 'k++' was incremented 3 times before reaching '\0' instead of 6
  // since 'It' only has 2 characters and terminates at '\0'. The
  // difference between the listed 'appendLength' 6 and the recorded
  // 'k++' of 3 is 3. The returned length for 'totalChar' is:
  //               =             -  (6 - 3)
  // 9             = 12          -  3
  appendOutput.one = totalLength - (appendLength - k);
  appendOutput.two = 0;
  appendOutput.three = 0;
  // The 'firstChar' with the 'appendChar'.
  appendOutput.string = totalChar;

  return appendOutput;
};
