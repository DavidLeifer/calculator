#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "struct.h"
#include "utility.h"
#include "binaryChar.h"
#include "arithmaticSteps.h"
#include "userInput.h"

// Formats the user input and writes the .log.
// Used in "interface.c" 'textGUI()'.

// Prints name and version for the calculator.
// Asks for user input.
char *charUserInput() {
  char *input = malloc(13 * sizeof(char));
  printf("Text ");
  printf("%s\n\n", version);
  printf("Enter two numbers less than 200\nwith arithmatic operators\n +  ,  -  ,  *  ,  /  ,  **\n\nExample: 1 + 1 (press enter):\n\n");
  scanf("%13[^\n]", input);
  return input;
}

// Splits the input by determining the airthmatic string, converts the two
// int to char and returns both for use in 'arithmaticInput()' and 'fileLog()'.
struct threeInt inputFormat(char input[]) {
  // Allows inputs such as '1+1' and '1+ 1' or '100 +   10' calculated when
  // the user presses 'enter' regardless of formating.
  //char charNumberOne[4] = {0};
  //char charNumberTwo[4] = {0};
  //char *charNumberOne = malloc(4 * sizeof(char));
  //char *charNumberTwo = malloc(4 * sizeof(char));
  char charNumberOne[4]; //= malloc(4 * sizeof(char));
  char charNumberTwo[4]; //= malloc(4 * sizeof(char));
  charNumberOne[0] = '\0';
  //charNumberOne[1] = '\0';
  //charNumberOne[2] = '\0';
  //charNumberOne[3] = '\0';

  charNumberTwo[4] = '\0';
  // char pointerArithmatic[3] = {0};
  char *arithmaticO = malloc(3 * sizeof(char));
  // 'malloc()' contains random data that could be the literal null-terminator 0
  // and is instead set to the ASCII character 48 or '0'.
  arithmaticO[0] = '0';
  //arithmaticO[1] = '0';
  //arithmaticO[2] = '0';
  //arithmaticO[3] = '0';
  int i = 0;
  int j = 0;
  int m = 0;
  // The length of the split char[] are used to convert to int.
  int intNumberTwoLength = 0;
  int intNumberOneLength = 0;

  struct threeInt output;
  // Input negative - 0 = none are, 1 = first number, 2 = second number, 3 = both.
  output.three = 0;

  while (input[i] != '\0') {
    if (input[i] != ' ') {


      // ASCii if it's an int char, set charNumberTwo to those integers.
      if ( (input[i] > 47) && (input[i] < 58)) {
        //printf("input[i]  %c  sizeof(charNumberOne) %d  'intNumberTwo' %d\n", input[i], sizeof(charNumberOne), intNumberTwoLength);
        // charNumberTwo[] when arithmaticO[] is not empty.

        // The code is confusing since english text reads from top to bottom.
        // The 'charNumberTwo' "if" is placed before the 'charNumberOne' "else if"
        // based on three assumptions:
        // 0) The second number 'charNumberTwo' already has a null-terminator in the [4] position (or
        //    the 5th character) in the array and is explicitly stated with the "else if".
        // 1) The order of 'input[i]' reads the first number from top to bottom.
        // 2) The first "if" has not found a 48 or '0' at 'arithmatic[0]' which is the second
        //    pointer char[] being extracted in this loop.
        // The char[] pointer with 'malloc()' without 'strcpy()' contains random data without the
        // character version of a null-terminator '\0' instead of ASCII version: 0. A regular char[]
        // also does this but can't be returned in functions and is why 'arithmatic0' is a pointer char[].
        // The other two 'char[]' are not pointers since they are used in 'charBinary2Int()' and that
        // function returns a different 'malloc()' pointer array.
        if ( arithmaticO[0] != '0' ) {
          charNumberTwo[m] = input[i];
          m++;
          intNumberTwoLength = m;
          charNumberTwo[intNumberTwoLength] = '\0';
        }
        // 'charNumberOne' when 'charNumberTwo[4]' is the null-terminator .
        else if ( charNumberTwo[4] == '\0' ) {
          charNumberOne[j] = input[i];
          j++;
          intNumberOneLength = j;
        }
      }
      // Determines if an integer is negative. These two conditionals work
      // because it occurs after 'arithmatic0[0]' has been set.
      if (output.three == 0 && arithmaticO[0] != '0' && input[i] == 45) {
        // Second number is negative.
        output.three = 2;
      }
      // 'charNumberOne' is negative and set, the operator has been set, and the
      // input is 45, both inputs are negative.
      if (output.three == 1 && arithmaticO[0] != '0' && input[i] == 45) {
        output.three = 3;
      }
      // Exponents
      if ((input[i] == 42) && (input[i+1] == 42)) {
        arithmaticO[0] = '*';
        arithmaticO[1] = '*';
        arithmaticO[2] = '\0';
        // printf("aaaa %d  %c %c %s\n", i, input[i], input[i+1], arithmaticO);
      }
      // Multiplication
      else if ( (input[i-1] != 42) && (input[i] == 42) && (input[i+1] != 42)) {
        arithmaticO[0] = '*';
        arithmaticO[1] = '\0';
        //printf("%d  %c\n", i, input[i]);
      }
      // Addition
      else if (input[i] == 43) {
        arithmaticO[0] = '+';
        arithmaticO[1] = '\0';
        //printf("%d  %c\n", i, input[i]);
      }
      // Subtraction
      // 'charNumberOne[0]' is necessary for negative numbers, otherwise the operator is '-' until a different
      // operand is found and 'output.three' negative check is not set for the first number. 'arithmaticO[0]' prevents
      // changes from '*' or '+' to subtraction '-' if the negative sign is meant for the second input integer.
      else if (charNumberOne[0] != '\0' && input[i] == 45 && arithmaticO[0] == '0') {
        arithmaticO[0] = '-';
        arithmaticO[1] = '\0';
        //printf("%d  %c\n", i, input[i]);
      }
      // Division
      else if (input[i] == 47) {
        arithmaticO[0] = '/';
        arithmaticO[1] = '\0';
        //printf("%d  %c\n", i, input[i]);
      }

      // 'charNumberOne' is negative '-' and the operator has not been set.
      if (input[i] == 45 && arithmaticO[0] == '0') {
        output.three = 1;
      }
    } // Closes the conditional " input[i] != ' ' "
    i++;
  } // Closes the " input[i] != '\0' " loop.

  // The two numbers and arithmaticO as their own char[] with NULL terminators.
  // printf("%s  %s  %s\n", charNumberOne, arithmaticO, charNumberTwo);
  intNumberOneLength++;
  intNumberTwoLength++;

  charNumberOne[intNumberOneLength] = '\0';
  charNumberTwo[intNumberTwoLength] = '\0';
  intNumberOneLength--;
  intNumberTwoLength--;
  // Convert 'charNumberOne' and 'charNumberTwo' to int with function from 'binaryChar.c'.
  ///////////////////////////////////////////////////////////////////////////////////////


    // Change one to negative with 'output.three'
  int numberOne = charBinary2Int(charNumberOne, intNumberOneLength);
  int numberTwo = charBinary2Int(charNumberTwo, intNumberTwoLength);

  output.one = numberOne;
  output.two = numberTwo;
  //printf("is negative %d\n", output.three);
  output.string = arithmaticO;

  // printf("input format %d %s %d\n", output.one, output.string, output.two);

  return output;
}

// Prints the answer from 'inputFormat()'. Sends the answer to .log file with 'logFile()'.
char* inputArithmatic(struct threeInt inputFormatted) {

  int numberOne = inputFormatted.one;
  int numberTwo = inputFormatted.two;
  int answer = 0;
  //char *arithmaticO = inputFormatted.string;

  // Difference with negative numbers 0, 1, 2, 3 correspond to: none are negative, first number negative,
  // inputFormatted.three                                       second number negative, both numbers are.
  //

  // 'inputFormatted.three' is changed when the 'answer' is negative '0' positive '1' negative and
  // used in "intBinary2Char()".

  // Exponents
  if (inputFormatted.string[0] == 42 && inputFormatted.string[1] == 42 && inputFormatted.string[2] == '\0') {
    answer = exponents(numberOne, numberTwo);
    printf("%d ** %d = %d\n", numberOne, numberTwo, answer);
  }
  // Multiplication
  else if (inputFormatted.string[0] == 42 && inputFormatted.string[1] == '\0') {
    // One or both input numbers are '0' the answer is '0'.
    if (numberOne == 0 || numberTwo == 0) {
      // 'answer' sign is positive for .log purposes.
      inputFormatted.three = 0;
      // 'answer' is already '0' and 'print()' to terminal.
      printf("%d * %d = 0\n", numberOne, numberTwo);
    }
    else {
      // One of either input numbers are negative, the answer is negative by switching the sign.
      if (inputFormatted.three == 1 || inputFormatted.three == 2) {
        answer = multiplication(numberOne, numberTwo);
        // Indicates 'answer' is negative.
        inputFormatted.three = 1;
        printf("%d * %d = -%d\n", numberOne, numberTwo, answer);
      }
      // Both are positive or negative 'inputFormatted.three == 3'.
      else {
        answer = multiplication(numberOne, numberTwo);
        inputFormatted.three = 0;
        printf("%d * %d = %d\n", numberOne, numberTwo, answer);
      }
    }
  }
  // Addition
  else if (inputFormatted.string[0] == 43 && inputFormatted.string[1] == '\0') {
    // 0  Both are positive and is regular addition.
    if (inputFormatted.three == 0) {
      answer = addition(numberOne, numberTwo);
      // Indicates 'answer' is negative.
      inputFormatted.three = 0;
      printf("%d + %d = %d\n", numberOne, numberTwo, answer);
    }
    // 'numberOne' is negative and 'numberTwo' is positive.
    if (inputFormatted.three == 1) {
      // 'numberTwo' is smaller.
      // -5 + 2 = -3  == 5 - 2 = 3 (without '-').
      if (numberOne > numberTwo) {
        answer = subtraction(numberOne, numberTwo);
        // Indicates 'answer' is negative.
        inputFormatted.three = 1;
        printf("-%d + %d = -%d\n", numberOne, numberTwo, answer);
      }
      // 'numberOne' is negative and smaller.
      // -5 + 10 = 5  ==  10 - 5 = 5
      else if (numberOne < numberTwo) {
        answer = subtraction(numberTwo, numberOne);
        inputFormatted.three = 0;
        printf("-%d + %d = %d\n", numberOne, numberTwo, answer);
      }
      // -5 + 5 = 0
      else {
        // 'answer' is '0'.
        inputFormatted.three = 0;
        printf("-%d + %d = 0\n", numberOne, numberTwo);
      }
    }
    // 'numberTwo' is negative.
    else if (inputFormatted.three == 2) {
      // 'numberTwo' is smaller.
      // 10 + -5 = 5   ==   10 - 5 = 5
      if (numberOne > numberTwo) {
        answer = subtraction(numberOne, numberTwo);
        inputFormatted.three = 0;
        printf("%d + -%d = %d\n", numberOne, numberTwo, answer);
      }
      // 'numberOne' is smaller.
      // 5 + -10 = -5  ==  10 - 5 = 5 (without the sign '-').
      else if (numberOne < numberTwo) {
        answer = subtraction(numberTwo, numberOne);
        // Indicates 'answer' is negative.
        inputFormatted.three = 1;
        printf("%d + -%d = -%d\n", numberOne, numberTwo, answer);
      }
      // 5 + -5 = 0
      else {
        // 'answer' is '0'.
        inputFormatted.three = 0;
        printf("%d + -%d = 0\n", numberOne, numberTwo);
      }
    }
    // Both are negative.
    // -5 + -5 = -10
    else if (inputFormatted.three == 3) {
      answer = addition(numberOne, numberTwo);
      // Indicates 'answer' is negative.
      inputFormatted.three = 1;
      printf("-%d + -%d = -%d\n", numberOne, numberTwo, answer);
    }
  }
  // Subtraction - does negative 'answers' not negative input
  else if (inputFormatted.string[0] == 45 && inputFormatted.string[1] == '\0') {
    // Both inputs are positive.
    if (inputFormatted.three == 0) {
      if (numberOne == numberTwo) {
        // 'answer' is already '0'
        inputFormatted.three = 0;
        printf("%d - %d = 0\n", numberOne, numberTwo);
      }
      // Positive difference i.e. '5 - 4 = 1'
      else if (numberOne > numberTwo) {
        answer = subtraction(numberOne, numberTwo);
        inputFormatted.three = 0;
        printf("%d - %d = %d\n", numberOne, numberTwo, answer);
      }
      // 'answer' is negative i.e. '3 - 10 = -7'
      //        Flipped around it's '10 - 3 = 7' allowing binary subtraction with '-' inserted after.
      else {
        answer = subtraction(numberTwo, numberOne);
        // Indicates 'answer' is negative.
        inputFormatted.three = 1;
        printf("%d - %d = -%d\n", numberOne, numberTwo, answer);
      }
    }
    // 'numberOne' is negative and 'numberTwo' is positive.
    else if (inputFormatted.three == 1) {
      // 'answer' is from adding the number regardless.
      answer = addition(numberOne, numberTwo);
      // Indicates 'answer' is negative.
      inputFormatted.three = 1;
      // -10 - 5 = -15  ==  10 + 5 = 15 (without '-')
      // The first integer is larger.
      if (numberOne > numberTwo) {
        printf("-%d - %d = -%d\n", numberOne, numberTwo, answer);
      }
      // '-3 - 10 = -13'  ==  '3 + 10 = 13' (without '-')
      // 'nmberTwo' is larger or equal.
      // '-3 - 3 = -6'    ==  '3 + 3 = 6'
      else {
        printf("-%d - %d = -%d\n", numberOne, numberTwo, answer);
      }
    }
    // 'numberOne' is positive and 'numberTwo' is negative.
    else if (inputFormatted.three == 2) {
      // 'answer' is from adding the number regardless.
      answer = addition(numberOne, numberTwo);
      inputFormatted.three = 0;
      // The first integer is larger.
      // '5 - -2 = 7'  ==  '5 + 2 = 7'
      if (numberOne > numberTwo) {
        printf("%d - -%d = %d\n", numberOne, numberTwo, answer);
      }
      // 'nmberTwo' is larger or equal.
      // '5 - -7 = 12'  ==  '5 + 7 = 12'
      else {
        //answer = addition(numberTwo, numberOne);
        printf("%d - -%d = %d\n", numberOne, numberTwo, answer);
      }
    }
    // 'numberOne' is negative and 'numberTwo' is negative.
    else if (inputFormatted.three == 3) {
      // The input integers are equal.
      // '-5 - -5 = 0'
      if (numberOne == numberTwo) {
        inputFormatted.three = 0;
        printf("-%d - -%d = 0\n", numberOne, numberTwo);
      }
      // The first integer is larger.
      // '-5 - -2 = -3'  ==  '5 - 2 = 3' (without the sign)
      else if (numberOne > numberTwo) {
        answer = subtraction(numberOne, numberTwo);
        // Indicates 'answer' is negative.
        inputFormatted.three = 1;
        printf("-%d - -%d = -%d\n", numberOne, numberTwo, answer);
      }
      // 'nmberTwo' is larger or equal.
      // '-5 - -7 = 2'  ==  '7 - 5 = 2'
      else if (numberOne < numberTwo) {
        answer = subtraction(numberTwo, numberOne);
        inputFormatted.three = 0;
        printf("-%d - -%d = %d\n", numberOne, numberTwo, answer);
      }
    }
  }
  // Division
  else if (inputFormatted.string[0] == 47 && inputFormatted.string[1] == '\0') {
    answer = division(numberOne, numberTwo);
    // printf("%d / %d = %d\n", numberOne, numberTwo, answer);
  }
  else {
    printf("Error: bad input.\n");
  }
  // The malloc string from 'inputFormat()'.
  //free(arithmaticO);
  // printf("inputArithmatic:  %d %s %d = %d\n", numberOne, arithmaticO, numberTwo, answer);

  // Find length of answer.
  int answerLength = intLength(answer);

  // Converts int to char passing int and int length to a struct for a function in 'charBinary.c'.
  struct threeInt structAnswer;
  structAnswer.one = answerLength;
  structAnswer.two = answer;
  //printf("inputFormatted.three  %d  ", inputFormatted.three);
  structAnswer.three = inputFormatted.three;
  structAnswer.string = '\0';

  //printf("answer length  %d\n\n", structAnswer.one);
  // resuse 'inputFormatted.three' to indicate 'answer' sign at 'structAnswer.three = 0;'
  // change 'intBinary2Char()': if 'structAnswer.three ==' '0' positive or '1' negative.
  char *output = intBinary2Char(structAnswer);
  //printf("answer length: %d\n", structAnswer.one);
  //printf("  output  %s\n", output);
  return output;
}

// Opens the .log file to see if it returns NULL or already exists and returns the boolean.
int fileLogCheck() {
  int fileCheck = 0;

  FILE *fileLog;
  fileLog = fopen("./.log", "r");
  // If the file exists, set the 'fileCheck' to '1'.
  if (fileLog != NULL) {
    fileCheck = 1;
    fclose(fileLog);
  }
  else {
    fileCheck = 0;
  }
  return fileCheck;
}



////////////////////////////////////////////////////////
// Mostly run the above functions and append the char[].

// Uses the 'logCheck' from "fileLogCheck()' to format the beginning of '.log'.
struct threeInt dateTime(int logCheck) {
  // Uses 'time()' and 'strftime()' from <time.h> for date and time.
  // Could also make an epoch and count CPU rotations similar to 1969++;
  time_t now = time(NULL);
  struct tm *localTime = localtime(&now);
  char date[11];
  char time[9];
  int userInputLength = 13;
  strftime(date, sizeof(date), "%d-%m-%Y", localTime);
  strftime(time, sizeof(time), "%H:%M:%S", localTime);

  // The char[] for the entire string.
  char userInputFormatted[100];
  // Not sure why this is tolerable but the other char[] were malloc.

  // The date struct is declared early since it's a compiled language.
  struct threeInt userInputDate;

  // If '.log' exists the returned char starts witha  new line and the date.
  if (logCheck == 1) {
    // printf("Exists and appending.\n");
    //FILE *fileLogAppend;

    // if (date != previousDate) {
    userInputFormatted[0] = '\n';
    // Append the date.
    userInputDate = charAppend(userInputFormatted, 1, date, 11, 1);
  }
  else {
    // If '.log' doesn't exist, append the version, header, and date.
    // Appends the document header and version. 'version' is a const
    // and is reassigned to reuse 'charAppend()' in "utility.c".
    //char *mallocVersion = malloc(24 * sizeof(char));
    //strcpy(mallocVersion, version);
    char notConstVersion[24];
    int i = 0;
    while (version[i] != '\0') {
      notConstVersion[i] = version[i];
      i++;
    }
    notConstVersion[i] = '\0';
    struct threeInt userInputVersion = charAppend(userInputFormatted, 0, notConstVersion, 24, 1);
    // Free the 'mallocVersion' data.
    //free(mallocVersion);

    char header[10] = "Log File\n\n";
    //char *mallocHeader = malloc(12 * sizeof(char));
    //strcpy(mallocHeader, header);
    //struct threeInt userInputHeader = charAppend(userInputVersion.string, userInputVersion.one, mallocHeader, 12, 0);
    struct threeInt userInputHeader = charAppend(userInputVersion.string, userInputVersion.one, header, 10, 0);
    free(userInputVersion.string);
    //free(mallocHeader);
    // printf("userInputHeader = %s\n", userInputHeader.string);

    userInputDate = charAppend(userInputHeader.string, userInputHeader.one, date, 11, 1);
    free(userInputHeader.string);
  }
  // Writes the time identification to the output.
  struct threeInt userInputTime = charAppend(userInputDate.string, userInputDate.one, time, 9, 1);
  free(userInputDate.string);
  return userInputTime;
}

// 0) Splits 'strUserInput' into int and arithmatic char with 'inputFormat()'.
// 1) Sends to 'inputArithmatic()' returns the answer as a char.
char* outputArithmatic(char *userInput) {
  // The malloc string 'arithmaticO' is free() in inputArithmatic.
  struct threeInt splitUserInput = inputFormat(userInput);
  // Send to arithmatic functions and print the answer.
  char *answer = inputArithmatic(splitUserInput);
  //printf("    %s\n", strUserInput);
  return answer;
}

// Appends the time, equal sign, and answer into a char[].
struct threeInt manualInput(struct threeInt userInputTime, char *strUserInput, char *answer) {
  // Append the userInput without the answer.
  struct threeInt userInputInput = charAppend(userInputTime.string, userInputTime.one, strUserInput, 13, 1);

  // Append the equal sign and answer.
  //char *charEqual = malloc(2 * sizeof(char));
  //strcpy(charEqual, "=");
  char charEqual[2];
  charEqual[0] = '=';
  charEqual[1] = '\0';
  struct threeInt userInputEqual = charAppend(userInputInput.string, userInputInput.one, charEqual, 1, 1);
  free(userInputInput.string);
  //free(charEqual);

  struct threeInt userInputAnswer = charAppend(userInputEqual.string, userInputEqual.one, answer, 5, 0);
  //printf("charAppend:  %s\n\n", userInputAnswer.string);
  free(userInputEqual.string);
  return userInputAnswer;
}

// Writes (makes the .log file) or appends 'userInput' based on output from 'fileLogCheck()'.
void fileLog(char *userInput, int fileCheck) {
  FILE *fileLogTwo;
  if (fileCheck == 1) {
    fileLogTwo = fopen("./.log", "a");
  }
  else if (fileCheck == 0) {
    fileLogTwo = fopen("./.log", "w");
  }
  else {
    printf("No file log check: %d.\n", fileCheck);
  }

  // Write the file and close.
  fprintf(fileLogTwo, userInput);
  fclose(fileLogTwo);
  free(userInput);
}
