#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "binaryChar.h"
#include "arithmatic.h"
#include "userInput.h"

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

int fileLog(char userInput[9]) {
  FILE *fileLog;
  fileLog = fopen("./.calculatorLog", "r");
  // Uses 'time()' and 'strftime()' from <time.h> for date and time.
  time_t now = time(NULL);
  struct tm *localTime = localtime(&now);
  char date[11];
  char time[9];
  strftime(date, sizeof(date), "%d-%m-%Y", localTime);
  strftime(time, sizeof(time), "%H:%M:%S", localTime);
  // printf("Current date: %s\n", date);
  // printf("Current time: %s\n", time);
  // Probably around 60 characters at most.
  char userInputFormatted[100];
  // If the file exists, append information to the end.
  if (fileLog != NULL) {
    fclose(fileLog);
    // printf("Exists and appending.\n");
    FILE *fileLogAppend;
    fileLogAppend = fopen("./.calculatorLog", "a");
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
      // printf("%c  %c\n", userInputFormatted[j], userInput[i]);
      i++;
      j++;
    }
    // printf("%s\n", userInputFormatted);
    fprintf(fileLogAppend, userInputFormatted);
    fclose(fileLogAppend);
  }
  else {
    // Make the file and append the output if it doesn't exist.
    // printf("Doesn't exist and write mode.\n");
    FILE *fileLogWrite;
    fileLogWrite = fopen("./.calculatorLog", "w");

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
    userInputFormatted[j] = '\0';
    // printf("%s\n", userInputFormatted);
    fprintf(fileLogWrite, userInputFormatted);
    fclose(fileLogWrite);
  }
  return 0;
}

/*
void textGUI() {
  // Allows inputs such as '1+1' and '1+ 1' calculating when the user presses 'enter' regardless of formating.
  char input[13];
  int numberOne = 0;
  int numberTwo;
  char arithmaticO[3];
  printf("Text ");
  printf("%s\n\n", version);
  printf("Enter two numbers less than 200\nwith arithmatic operators\n +  ,  -  ,  *  ,  /  ,  **\n\nExample: 1 + 1 (press enter):\n\n");

  // scanf(" %3d %c %10s", &numberOne, &operator, numberTwo);
  scanf("%13[^\n]", input);

  // fileLog(struct threeInt);

  int i = 0;
  while (input[i] != '\0') {
     if (input[i] != ' ') {
       // Exponents
       if ((input[i] == 42) && (input[i+1] == 42)) {
         arithmaticO[0] = '*';
         arithmaticO[1] = '*';
         arithmaticO[2] = '\0';
         // printf("aaaa %d  %c%c %s\n", i, input[i], input[i], arithmaticO);
       }
       // Multiplication
       else if ( (input[i-1] != 42) && (input[i] == 42) && (input[i+1] != 42)) {
         printf("%d  %c\n", i, input[i]);
       }
       // Addition
       else if (input[i] == 43) {
         printf("%d  %c\n", i, input[i]);
       }
       // Subtraction
       else if (input[i] == 45) {
         printf("%d  %c\n", i, input[i]);
       }
       // Division
       else if (input[i] == 47) {
         printf("%d  %c\n", i, input[i]);
       }
       else if ((input[i] > 48) && (input[i] < 57)){
         if (numberOne == 0) {
           numberOne = input[i] - '0';
           printf("%d  %c  %d\n", i, input[i], numberOne);
         }
         else {
           numberTwo = input[i] - '0';
           printf("%d  %c  %d  %d\n", i, input[i], numberOne, numberTwo);
         }
       }
       else {
         int fooo = 0;
         // printf("Error not a number: %c\n", input[i]);
       }
     }
     else {
       int foo = 0;
       // printf("white space\n");
     }
     i++;
  }
  i++;

  // Checks if the input matches operator ('+', '-', '*', 'etc') to run the function.
  // This is the overflow into numberTwo[0] '**'.
  //if (operator == '*' && numberTwo[0] == '*') {
    // todo append function

    //int intNumberTwo;
    /*
    int i = 1;
    while (numberTwo[i] != '\0') {
      intNumberTwo = numberTwo[i] - '0';
      //printf("%d    %c  %d\n", i, numberTwo[i], intNumberTwo);
      i++;
    }
    //
    // int intNumberTwo = numberTwo[1:] - '0';
    // int intExponent = exponents(numberOne, intNumberTwo);
    //printf("\n%d ** %d = %d\n", numberOne, intNumberTwo, intExponent);
  //}
  //
  else if (operator == additionChar) {
    // This is the char binary to int binary function (test if works). needs the length or change the function.
    int i = 0;
    int intNumberTwo = 1;
    while (numberTwo[i] != '\0') {
      intNumberTwo = (intNumberTwo * 10) + numberTwo[i] - '0';
      i++;
    }
    int intNumberTwoExponent = exponents(10, i);
    int intNumberTwoRemainder = intNumberTwo % intNumberTwoExponent;
    // printf("%d  %d\n", numberOne, intNumberTwoRemainder);

    // for some reason the addition doesnt work on 200 + 100 and other random two digit sums
    // despite the input being accurate.
    int sum = addition(numberOne, intNumberTwoRemainder);
    printf("\n%d + %d = %d\n", numberOne, intNumberTwoRemainder, sum);
  }
  else if (operator == subtractionChar) {
    int foo = 0;
    // int difference = subtraction(numberOne, numberTwo);
  }
  else if (operator == multiplicationChar) {
    int foo = 0;
    // int product = multiplication(numberOne, numberTwo);
  }
  else if (operator == divisionChar) {
    int foo = 0;
    // int quotient = division(numberOne, numberTwo);
  }
  else {
    printf("\nError : bad input.\n");
  } //
}
*/

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

/*
int interface(int argc, char *argv){
  // printf("aaa %s\n", argv);
  // The calculator text interface is shown if terminal doesn't ask for help.
  if (argc < 2) {
    textGUI();
    return 1;
  }
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
    printf("Features: addition.\n");
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
*/
