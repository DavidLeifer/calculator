#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "struct.h"
#include "interface.h"
#include "utility.h"
#include "arithmaticSteps.h"
#include "binaryChar.h"
#include "userInput.h"

// Input is userInput '1 + 100', outputs the answer '1 + 100 = 101' and 'fileLogCheck()' '1' or not 1.
struct threeInt userOutput(char *strUserInput) {
  // 0) Splits 'strUserInput' into int and arithmatic char[] with 'inputFormat()'.
  // 1) Sends to 'inputArithmatic()' and returns the answer as a char[] i.e. 'addition()'.
  // 3) Returns the 'logCheck' to create the .log file and append or only append.
  char *answer = outputArithmatic(strUserInput);
  // Check to see if the file exists and format the date and time identification.
  int logCheck = fileLogCheck();

  struct threeInt userInputTime = dateTime(logCheck);
  // Appends the date, time, user input and answer into one char[].
  struct threeInt userInputAnswer = manualInput(userInputTime, strUserInput, answer);
  free(userInputTime.string);
  free(strUserInput);
  userInputAnswer.two = logCheck;
  free(answer);
  return userInputAnswer;
}

// 'textUI()' handles input and is used when the program is executed without '-h', '-longTest', etc.
// Runs the user input, determines arithmatic functions, and writes the output to a '.log' file.
void textUI() {
  // User input with 'scanf()'.
  char *strUserInput = charUserInput();
  struct threeInt userInputAnswer = userOutput(strUserInput);
  fileLog(userInputAnswer.string, userInputAnswer.two);
}

// 'test()' completes calculation for every integer between 0 and 200 for debugging.
// Compares C calculation results with "binaryChar.c".
// 'userOutput()' and 'fileLog()' from 'textUI()' to write the incorrect input.
int test(int small, int large, int mid, int longPrint) {
  int realSum;
  int mainSum;
  int smallTwo = 200;
  int bad = 1;
  int total = 1;
  while (small <= large) {
    while (smallTwo <= mid) {
      realSum = small + smallTwo;
      mainSum = addition(small, smallTwo);
      //realSum = small - smallTwo;
      //mainSum = subtraction(small, smallTwo);
      if (realSum != mainSum) {
      //if ((realSum == mainSum) && (small >= smallTwo)) {
        bad++;
        if (longPrint == 1) {
          printf("%d + %d = %d realSum %d\n", small, smallTwo, mainSum, realSum);
          //printf("%d - %d = %d realDifference %d\n", small, smallTwo, mainSum, realSum);
          // Convert int small and smallTwo to char[] with "binaryChar.c" 'intBinary2Char()'.
          int smallLength = intLength(small);
          int smallTwoLength = intLength(smallTwo);
          struct threeInt structSmall;
          structSmall.one = smallLength;
          structSmall.two = small;
          structSmall.three = 0;
          structSmall.string = '\0';
          struct threeInt structSmallTwo;
          structSmallTwo.one = smallTwoLength;
          structSmallTwo.two = smallTwo;
          structSmallTwo.three = 0;
          structSmallTwo.string = '\0';
          char *charSmall = intBinary2Char(structSmall);
          char *charSmallTwo = intBinary2Char(structSmallTwo);
          free(structSmall.string);
          free(structSmallTwo.string);
          // Appends the char[] numbers, arithmatic, equals sign, and answer and send to 'fileLog()'.
          char *mallocUserInput = malloc(13 * sizeof(char));
          struct threeInt strUserSmall = charAppend(mallocUserInput, 0, charSmall, 4, 1);
          char *mallocArithmatic = malloc(3 * sizeof(char));
          strcpy(mallocArithmatic, "+");
          struct threeInt strUserArithmatic = charAppend(strUserSmall.string, strUserSmall.one, mallocArithmatic, 1, 1);
          free(strUserSmall.string);
          free(mallocArithmatic);
          struct threeInt strUserSmallTwo = charAppend(strUserArithmatic.string, strUserArithmatic.one, charSmallTwo, 4, 0);
          struct threeInt userInputAnswer = userOutput(strUserSmallTwo.string);
          //printf("%s realSum %d\n", userInputAnswer.string, realSum);

          fileLog(userInputAnswer.string, userInputAnswer.two);
        }
      }
      smallTwo++;
      total++;
    }
    smallTwo = 0;
    //printf("%d ", small);
    small++;
  }
  // total may or may not be -1.
  printf("Not right: %d  Right %d\n", bad - 1, total - 1);
  return 0;
}

int interface(int argc, char *argv){
  // printf("aaa %s\n", argv);
  // Allows for user designated input i.e. '-help', '-h', 'test', 't', longTest, 'lt', etc.
  int i = 0;
  // Find the length of the input.
  while (argv[i] != '\0') {
    i++;
  }
  // printf("%c  %d\n", argv[1], i);
  if ((argv[1] == 'h' && i == 5) || (argv[1] == 'h' && i == 2)) {
    printf("Text %s", version);
    printf(" Help.\n\n");
    printf("%s\n", features);
  }
  else if ((argv[1] == 't' && i == 5) || (argv[1] == 't' && i == 2)) {
    // Short test output.
    printf("bad\n");
    int small = 0;
    int large = 200;
    int mid = 200;
    int longPrint = 0;
    int testOut = test(small, large, mid, longPrint);
  }
  else if ((argv[1] == 'l' && argv[2] == 't' && i == 3) || (argv[1] == 'l' && argv[2] == 'o' && i == 9)) {
    // Long test ouput.
    int small = 0;
    int large = 200;
    int mid = 200;
    int longPrint = 1;
    int testOut = test(small, large, mid, longPrint);
  }
  else {
    printf("Incorrect input. Exiting.\n");
  }
  return 0;
}
