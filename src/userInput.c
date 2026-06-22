#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "struct.h"
#include "utility.h"
#include "binaryChar.h"
#include "arithmatic.h"
#include "userInput.h"

void charUserInput(char *input) {
  printf("Text ");
  printf("%s\n\n", version);
  printf("Enter two numbers less than 200\nwith arithmatic operators\n +  ,  -  ,  *  ,  /  ,  **\n\nExample: 1 + 1 (press enter):\n\n");
  scanf("%13[^\n]", input);
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
  // todo This is the 'intLen()' function.
  // Similar to function at the end of 'charBinary.c'.
  int answerReduce = answer;
  int answerLength = 0;
  while (answerReduce != 0) {
    answerReduce /= 10;
    answerLength++;
  }

  // Converts int to char passing int and int length to a struct for a function in 'charBinary.c'.
  struct threeInt structAnswer;
  structAnswer.one = answerLength;
  structAnswer.two = answer;
  structAnswer.three = 0;
  structAnswer.string = '\0';
  char *output = intBinary2Char(structAnswer);

  return output;
}

// Uses 'fopen()' and 'fgets()' to parse the document in 'r' mode.
void fileLogRead(char fileName[]) {
  FILE *fileLog;
  fileLog = fopen(fileName, "r");
  int fileLogLength = 100;
  char logString[fileLogLength];
  int i = 0;
  while(1) {
    if (fgets(logString, fileLogLength, fileLog) != NULL) {
      printf("Line %d: %s\n", i, logString);
      i++;
    }
    else {
      break;
    }
  }
  printf("Number of lines: %d\n", i);
}

// Writes a file '.log' with a date and time ID to record the userInput and answer with 'charAppend()'.
int fileLog(char userInput[13], char answer[4]) {
  FILE *fileLog;
  fileLog = fopen("./.log", "r");
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
  // The date struct is declared early since it's a compiled language.
  struct threeInt userInputDate;
  FILE *fileLogTwo;

  // If the file exists, open in append mode with a line break and date.
  if (fileLog != NULL) {
    fclose(fileLog);
    // printf("Exists and appending.\n");
    //FILE *fileLogAppend;
    fileLogTwo = fopen("./.log", "a");

    // if (date != previousDate) {
    userInputFormatted[0] = '\n';
    // Append the date.
    userInputDate = charAppend(userInputFormatted, 1, date, 11, 1);
  }
  else {
    // Make the file with write mode and append the version, header, and date if it doesn't exist.
    fileLogTwo = fopen("./.log", "w");
    // Appends the document header and version.
    char *mallocVersion = malloc(24 * sizeof(char));
    strcpy(mallocVersion, version);
    struct threeInt userInputVersion = charAppend(userInputFormatted, 0, mallocVersion, 24, 1);

    char header[12] = " Log File\n\n";
    char *mallocHeader = malloc(12 * sizeof(char));
    strcpy(mallocHeader, header);
    struct threeInt userInputHeader = charAppend(userInputVersion.string, userInputVersion.one, mallocHeader, 12, 0);

    printf("userInputHeader = %s\n", userInputHeader.string);

    userInputDate = charAppend(userInputHeader.string, userInputHeader.one, date, 11, 1);
  }

  // Writes the date, time, and user input.
  struct threeInt userInputTime = charAppend(userInputDate.string, userInputDate.one, time, 9, 1);
  struct threeInt userInputInput = charAppend(userInputTime.string, userInputTime.one, userInput, 13, 1);

  // Append the equal sign and answer.
  char *charEqual = malloc(2 * sizeof(char));
  strcpy(charEqual, "=");
  struct threeInt userInputEqual = charAppend(userInputInput.string, userInputInput.one, charEqual, 1, 1);

  struct threeInt userInputAnswer = charAppend(userInputEqual.string, userInputEqual.one, answer, 5, 0);
  printf("charAppend:  %s\n\n", userInputAnswer.string);

  fprintf(fileLogTwo, userInputAnswer.string);
  fclose(fileLogTwo);
  return 0;
}

// Runs the arithmatic user input and file log functions.
void textGUI() {
  char *strUserInput = malloc(13 * sizeof(char));
  //char strUserInput[13];
  charUserInput(strUserInput);
  // int log = fileLog(strUserInput);
  // The malloc string 'arithmaticO' is free() in inputArithmatic.
  struct threeInt splitUserInput = inputFormat(strUserInput);
  // Send to arithmatic functions and print the answer.
  char *answer = inputArithmatic(splitUserInput);
  // printf("%s\n", strUserInput);
  int log = fileLog(strUserInput, answer);
  free(answer);
  free(strUserInput);
}

int test(int small, int large, int mid, int longPrint) {
  int realSum;
  int mainSum;
  int smallTwo = 0;
  int bad = 1;
  int total = 1;
  while (small <= large) {
    while (smallTwo <= mid) {
      realSum = small + smallTwo;
      mainSum = addition(small, smallTwo);
      if (realSum != mainSum) {
        bad++;
        if (longPrint == 1) {
          // char *output = malloc(19 * sizeof(char));
          // strcpy(output, "a");
          // output[bad] = small;
          // int smallLength = binaryInt2Char(small); append function()
          // output[bad+smallLength] = ' ';
          // output[bad] = small;
          printf("%d + %d = %d != %d\n", small, smallTwo, realSum, mainSum);
        }
      }
      smallTwo++;
      total++;
    }
    smallTwo = 0;
    //printf("%d ", small);
    small++;
  }
  printf("Not right: %d  Right %d\n", bad, total);
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
