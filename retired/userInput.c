#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "binaryFunctions.h"

void textGUI() {
  // Allows inputs such as '1+1' and '1+ 1' calculating when the user presses 'enter' regardless of formating.
  int numberOne;
  // 0,1,2, '\0'<- marks end of the string. Only works for int below 200 with 3 digits ( hence the [3] ).
  char numberTwo[3];
  char operator;
  printf("Text Calculator\nVersion ");
  printf("%s\n\n", version);
  printf("Enter two numbers less than 200\nwith arithmatic operators\n +  ,  -  ,  *  ,  /  ,  **\n\nExample: 1 + 1 (press enter):\n\n");
  // 'operater' has to not be a char array ' %c '.
  // 'char numberTwo[3]' gets overflow from 'char operator' on user input 'scanf()'.
  // Overflow occurs when there are two operators such as the power symbol (i.e. ' ** ' )
  // the space in front of the % removes whitespaces during input.
  // Operator is highlighted for unkown reasons.
  scanf(" %3d %c %3s", &numberOne, &operator, numberTwo);
  // printf("%d  %c  %s\n\n", numberOne, operator, numberTwo);
  // This is the other way that requires input format '1 + 1'.
  //scanf(" %3d%2s%3d", &numberOne, operator, &numberTwo);
  char additionChar;
  additionChar = '+';
  char subtractionChar;
  subtractionChar = '-';
  char multiplicationChar;
  multiplicationChar = '*';
  char divisionChar;
  divisionChar = '/';
  char *powerChar = "**";

  // Checks if the input matches operator ('+', '-', '*', 'etc') to run the function.
  // This is the overflow into numberTwo[0] '**'.
  if (operator == '*' && numberTwo[0] == '*') {
    // int intNumberTwo = numberTwo[1:] - '0';
    int intPower = power(numberOne, numberTwo[1]);
    // printf("\n%d ** %d = %d\n", numberOne, numberTwo, intPower);
  }
  else if (operator == additionChar) {
    // This is the char binary to int binary function (test if works). needs the length or change the function.
    int i = 0;
    int intNumberTwo = 1;
    while (numberTwo[i] != '\0') {
      intNumberTwo = (intNumberTwo * 10) + numberTwo[i] - '0';
      i++;
    }
    int intNumberTwoPower = power(10, i);
    int intNumberTwoRemainder = intNumberTwo % intNumberTwoPower;
    // printf("%d  %d\n", numberOne, intNumberTwoRemainder);

    // for some reason the addition doesnt work on 200 + 100 and other random two digit sums
    // despite the input being accurate.
    int sum = addition(numberOne, intNumberTwoRemainder);
    printf("\n%d + %d = %d\n", numberOne, intNumberTwoRemainder, sum);




  }
  else if (operator == subtractionChar) {
    int difference = subtraction(numberOne, numberTwo);
  }
  else if (operator == multiplicationChar) {
    int product = multiplication(numberOne, numberTwo);
  }
  else if (operator == divisionChar) {
    int quotient = division(numberOne, numberTwo);
  }
  else {
    printf("\nError : bad input.\n");
  }
}

int interface(int argc, char *argv){
  // printf("aaa %s\n", argv);
  // The calculator text interface is shown if terminal doesn't ask for help.
  if (argc < 2) {
    textGUI();
    return 1;
  }
  // Allows for user designated input i.e. '-help' or '-h' etc.
  int i = 0;
  // Find the length of the input.
  while (argv[i] != '\0') {
    i++;
  }
  //printf("\n%c  %d\n", argv[1], i);
  if (argv[1] == 'h' && i == 5) {
    printf("Text Calculator\nVersion ");
    printf("%s Help\n\n", version);
    printf("Features: addition.\n");
  }
  else if (argv[1] == 'h' && i == 2) {
    printf("Text Calculator\nVersion ");
    printf("%s Help\n\n", version);
    printf("Features: addition.\n");
  }
  else {
    printf("Incorrect input. Exiting.\n");
  }
  return 0;
}
