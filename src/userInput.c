#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "struct.h"
#include "utility.h"
#include "binaryChar.h"
#include "arithmatic.h"
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
  char *charNumberOne = malloc(4 * sizeof(char));
  char *charNumberTwo = malloc(4 * sizeof(char));
  // char pointerArithmatic[3] = {0};
  char *arithmaticO = malloc(3 * sizeof(char));
  int i = 0;
  int j = 0;
  int m = 0;
  // The length of the split char[] are used to convert to int.
  int intNumberTwoLength = 0;
  int intNumberOneLength = 0;
  while (input[i] != '\0') {
    if (input[i] != ' ') {
      // ASCii if it's an int char, set charNumberOne to those integers.
      if ( (input[i] > 47) && (input[i] < 58) ) {
        // charNumberTwo[] when arithmaticO[] is not empty.
        if ( arithmaticO[0] != '\0' ) {
          charNumberTwo[m] = input[i];
          m++;
          intNumberTwoLength = m;
        }
        // charNumberOne[] when charNumberTwo[] is still empty.
        else if ( charNumberTwo[0] == '\0' ) {
          charNumberOne[j] = input[i];
          j++;
          intNumberOneLength = j;
        }
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
      else if (input[i] == 45) {
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
      // Didnt work after an update and then compiled with this and the last else{}
      else {
        int foo = 0;
      }
    }
    else {
      //printf("%d  %d  space\n", i, input[i]);
    }
    i++;
  }

  // The two numbers and arithmaticO as their own char[] with NULL terminators.
  // printf("%s  %s  %s\n", charNumberOne, arithmaticO, charNumberTwo);
  intNumberOneLength++;
  intNumberTwoLength++;
  charNumberOne[intNumberOneLength] = '\0';
  charNumberTwo[intNumberTwoLength] = '\0';
  intNumberOneLength--;
  intNumberTwoLength--;
  // Convert 'charNumberOne' and 'charNumberTwo' to int with function from 'binaryChar.c'.
  // Why are these not pointers and read in the other function? arithmaticO has to be specifically defined.
  int numberOne = charBinary2Int(charNumberOne, intNumberOneLength);
  int numberTwo = charBinary2Int(charNumberTwo, intNumberTwoLength);
  struct threeInt output;
  output.one = numberOne;
  output.two = numberTwo;
  output.three = 0;
  output.string = arithmaticO;

  // printf("input format %d %s %d\n", output.one, output.string, output.two);

  return output;
}

// Prints the answer to arithmatic input. Sends the answer to .log file in 'logFile.c'.
char* inputArithmatic(struct threeInt inputFormatted) {

  int numberOne = inputFormatted.one;
  int numberTwo = inputFormatted.two;
  int answer = 0;
  char *arithmaticO = inputFormatted.string;
  // Exponents
  if (arithmaticO[0] == 42 && arithmaticO[1] == 42 && arithmaticO[2] == '\0') {
    answer = exponents(numberOne, numberTwo);
    printf("%d ** %d = %d\n", numberOne, numberTwo, answer);
  }
  // Multiplication
  else if (arithmaticO[0] == 42 && arithmaticO[1] == '\0') {
    answer = multiplication(numberOne, numberTwo);
    // printf("%d * %d = %d\n", numberOne, numberTwo, answer);
  }
  // Addition
  else if (arithmaticO[0] == 43 && arithmaticO[1] == '\0') {
    answer = addition(numberOne, numberTwo);
    printf("%d + %d = %d\n", numberOne, numberTwo, answer);
  }
  // Subtraction
  else if (arithmaticO[0] == 45 && arithmaticO[1] == '\0') {
    answer = subtraction(numberOne, numberTwo);
    // printf("%d - %d = %d\n", numberOne, numberTwo, answer);
  }
  // Division
  else if (arithmaticO[0] == 47 && arithmaticO[1] == '\0') {
    answer = division(numberOne, numberTwo);
    // printf("%d / %d = %d\n", numberOne, numberTwo, answer);
  }
  else {
    printf("Error: bad input.\n");
  }
  // The malloc string from 'inputFormat()'.
  free(arithmaticO);
  // printf("inputArithmatic:  %d %s %d = %d\n", numberOne, arithmaticO, numberTwo, answer);

  // Find length of answer.
  int answerLength = intLength(answer);

  // Converts int to char passing int and int length to a struct for a function in 'charBinary.c'.
  struct threeInt structAnswer;
  structAnswer.one = answerLength;
  structAnswer.two = answer;
  structAnswer.three = 0;
  structAnswer.string = '\0';
  char *output = intBinary2Char(structAnswer);

  return output;
}

// Opens a the .log file to see if it returns NULL or already exists and returns the boolean.
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
    // Appends the document header and version.
    char *mallocVersion = malloc(24 * sizeof(char));
    strcpy(mallocVersion, version);
    struct threeInt userInputVersion = charAppend(userInputFormatted, 0, mallocVersion, 24, 1);
    // Free all the malloc data.
    free(mallocVersion);

    char header[12] = " Log File\n\n";
    char *mallocHeader = malloc(12 * sizeof(char));
    strcpy(mallocHeader, header);
    struct threeInt userInputHeader = charAppend(userInputVersion.string, userInputVersion.one, mallocHeader, 12, 0);
    free(userInputVersion.string);
    free(mallocHeader);
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
  // printf("%s\n", strUserInput);
  return answer;
}

// Appends the time, equal sign, and answer into a char[].
struct threeInt manualInput(struct threeInt userInputTime, char *strUserInput, char *answer) {
  // Append the userInput without the answer.
  struct threeInt userInputInput = charAppend(userInputTime.string, userInputTime.one, strUserInput, 13, 1);

  // Append the equal sign and answer.
  char *charEqual = malloc(2 * sizeof(char));
  strcpy(charEqual, "=");
  struct threeInt userInputEqual = charAppend(userInputInput.string, userInputInput.one, charEqual, 1, 1);
  free(userInputInput.string);
  free(charEqual);

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
