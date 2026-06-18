#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

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
  char charNumberOne[4] = {0};
  char charNumberTwo[4] = {0};
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
  // The two numbers and arithmaticO as their own char[].
  // printf("%s  %s  %s\n", charNumberOne, arithmaticO, charNumberTwo);

  // Convert 'charNumberOne' and 'charNumberTwo' to int with function from 'binaryChar.c'.
  // Why are these not pointers and read in the other function? arithmaticO has to be specifically defined.
  int numberOne = charBinary2Int(charNumberOne, intNumberOneLength);
  int numberTwo = charBinary2Int(charNumberTwo, intNumberTwoLength);
  //printf("\n\n%d  %s  %d\n", numberTwo, arithmaticO, numberOne);

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
  // free(arithmaticO);
  // printf("inputArithmatic:  %d %s %d = %d\n", numberOne, arithmaticO, numberTwo, answer);

  // Find length of answer.
  // todo This is the 'intLen()' function.
  // Both 'while's are similar as two functions 'intBinary2Char' in 'charBinary.c'.
  int answerReduce = answer;
  int answerLength = 0;
  while (answerReduce != 0) {
    answerReduce /= 10;
    answerLength++;
  }

  // int2char
  char *output = malloc(4 * sizeof(char));
  strcpy(output, "a");
  int i = answerLength - 1;
  // printf("answer %d answerLength %d  i: %d\n", answer, answerLength, i);

  int intDigit = 0;
  while (i >= 0) {
    intDigit = answer % 10;
    output[i] = intDigit + '0';
    // printf("%d    output: %c\n", i, output[i]);
    answer /= 10;
    i--;
  }
  output[answerLength] = '\0';
  //output = intBinary2Char(structAnswer);

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

/* todo char string parser
char charAppend(char stringInput, int len, char append) {
  i = 0;
  while (append[i] != '\0'){
    stringInput[len] = append[i];
    len++;
    i++;
  }
  return stringInput, len;
};
*/
int fileLog(char userInput[13], char answer[4]) {
  // printf("write the append for fileLog.c\n%d  %s  %d = %s\n", userInput.one, userInput.string, userInput.two, answer);
  // userInput.one 2 char
  // userInput.two 2 char
  FILE *fileLog;
  fileLog = fopen("./.log", "r");
  // Uses 'time()' and 'strftime()' from <time.h> for date and time.
  time_t now = time(NULL);
  struct tm *localTime = localtime(&now);
  char date[11];
  char time[9];
  int userInputLength = 13;
  // char *userInput = malloc(userInputLength * sizeof(char));
  // strcpy(userInput, mallocInput);
  strftime(date, sizeof(date), "%d-%m-%Y", localTime);
  strftime(time, sizeof(time), "%H:%M:%S", localTime);
  // printf("Current date: %s\n", date);
  // printf("Current time: %s\n", time);
  // printf("Input: %s\n", userInput);

  // Probably around 60 characters at most.
  char userInputFormatted[100];
  // If the file exists, append information to the end.
  // printf("%s\n", userInput);
  if (fileLog != NULL) {
    fclose(fileLog);
    // printf("Exists and appending.\n");
    FILE *fileLogAppend;
    fileLogAppend = fopen("./.log", "a");
    int i = 0;
    int j = 1;
    userInputFormatted[0] = '\n';
    // if (date != previousDate) {
    // Writes the date and time.
    while (date[i] != '\0') {
      userInputFormatted[j] = date[i];
      j++;
      i++;
    }
    userInputFormatted[j] = ' ';
    i = 0;
    j++;
    while (time[i] != '\0') {
      userInputFormatted[j] = time[i];
      j++;
      i++;
    }
    userInputFormatted[j] = ' ';
    i = 0;
    j++;
    userInputFormatted[j] = ' ';
    j++;
    // Appends the information to the document.
    while (userInput[i] != '\0') {
      userInputFormatted[j] = userInput[i];
      //printf("%d %c  %d %c\n", j, userInputFormatted[j], i, userInput[i]);
      i++;
      j++;
    }
    // Include the answer.
    userInputFormatted[j] = ' ';
    userInputFormatted[j+1] = '=';
    userInputFormatted[j+2] = ' ';
    i = 0;
    j = j + 3;
    while (answer[i] != '\0') {
      userInputFormatted[j] = answer[i];
      // printf("%d %c  %d %c\n", j, userInputFormatted[j], i, answer[i]);
      i++;
      j++;
    }
    userInputFormatted[j] = '\0';
    // printf("%s\n", userInputFormatted);
    fprintf(fileLogAppend, userInputFormatted);
    fclose(fileLogAppend);
  }
  else {
    // Make the file and append the output if it doesn't exist.
    // printf("Doesn't exist and write mode.\n");
    FILE *fileLogWrite;
    fileLogWrite = fopen("./.log", "w");

    int i = 0;
    int j = 23;
    int k = 0;
    char header[10] = " Log File\n";
    // The first loop includes the document title and version.
    while (j < 33) {
      if (i < 23) {
        userInputFormatted[i] = version[i];
        // printf("%d  %c\n", i, userInputFormatted[i]);
        i++;
      }
      else {
        // printf("%d  %c\n", j, header[k]);
        userInputFormatted[j] = header[k];
        j++;
        k++;
      }
    }
    userInputFormatted[j] = '\n';
    i = 0;
    j++;
    // printf("%s\n%s\n%d\n", userInputFormatted, userInput, i);
    // The next two include the date and time.
    while (date[i] != '\0') {
      // printf("%d  %c    %d\n", i, date[i], j);
      userInputFormatted[j] = date[i];
      i++;
      j++;
    }
    userInputFormatted[j] = ' ';
    i = 0;
    j++;
    while (time[i] != '\0') {
      // printf("%d  %c    %d\n", i, time[i], j);
      userInputFormatted[j] = time[i];
      i++;
      j++;
    }
    userInputFormatted[j] = ' ';
    i = 0;
    j++;
    userInputFormatted[j] = ' ';
    j++;
    // Writes the user input.
    while (userInput[i] != '\0') {
      // printf("%d  %c    %d\n", i, userInput[i], j);
      userInputFormatted[j] = userInput[i];
      i++;
      j++;
    }
    // Include the answer.
    userInputFormatted[j] = ' ';
    userInputFormatted[j+1] = '=';
    userInputFormatted[j+2] = ' ';
    i = 0;
    j = j + 3;
    while (answer[i] != '\0') {
      userInputFormatted[j] = answer[i];
      i++;
      j++;
    }
    userInputFormatted[j] = '\0';
    fprintf(fileLogWrite, userInputFormatted);
    fclose(fileLogWrite);
  }
  return 0;
}

// Runs the arithmatic user input and file log functions.
void textGUI() {
  char *strUserInput = malloc(13 * sizeof(char));
  //char strUserInput[13];
  charUserInput(strUserInput);
  // int log = fileLog(strUserInput);
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
  // printf("\n%c  %d\n", argv[1], i);
  if ((argv[1] == 'h' && i == 5) || (argv[1] == 'h' && i == 2)) {
    printf("Text %s", version);
    printf(" Help.\n\n");
    // printf("Features: addition, logging.\n");
    printf("%s\n", features);
  }
  else if ((argv[1] == 't' && i == 5) || (argv[1] == 't' && i == 2)) {
    // Short test output.
    int small = 0;
    int large = 200;
    int mid = 200;
    int longPrint = 0;
    //int testOut = test(small, large, mid, longPrint);
  }
  else if ((argv[1] == 'l' && argv[2] == 't' && i == 3) || (argv[1] == 'l' && argv[2] == 'o' && i == 9)) {
    // Long test ouput.
    int small = 0;
    int large = 200;
    int mid = 200;
    int longPrint = 1;
    //int testOut = test(small, large, mid, longPrint);
  }
  else {
    printf("Incorrect input. Exiting.\n");
  }
  return 0;
}
