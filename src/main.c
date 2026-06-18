#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./src/binaryChar.h"
#include "./src/arithmatic.h"
#include "./src/userInput.h"

/*
Notes

  Have to specify function calls in binaryFunctions.h and here besides the
  actual binaryFunctions.c file.

  // full way: gcc main.c binaryFunctions.c arithmatic.c userInput.c -o binary && ./binary

gcc main.c ./src/*.c -o binary && ./binary

todo
  - exponents() stopped working from input after name switch and move.
  - append function
  - append function in the log file
  - log user input

  - test are commented out.

*/

const char version[23] = "Calculator\nVersion 0.01";

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


       // Convert the first and second char[] numbers to int.
       // Only does single digits.
       else if ((input[i] > 48) && (input[i] < 58)){
         // If numberOne is still NULL, convert input to int.
         if (numberOne == 0) {
           numberOne = input[i] - '0';
           // printf("%d  %c  %d\n", i, input[i], numberOne);
         }
         // Otherwise convert numberTwo to int.
         else {
           numberTwo = input[i] - '0';
           // printf("%d  %c  %d  %d\n", i, input[i], numberOne, numberTwo);
         }
       }


     i++;
    }
  }
  //printf("%d  %s  %d  =  ?\n", numberOne, arithmaticO, numberTwo);
  // Send to arithmatic functions.
  // Exponents
  if (arithmaticO[0] == 42 && arithmaticO[1] == 42 && arithmaticO[2] == '\0') {
    int exponent = exponents(numberOne, numberTwo);
    printf("%d ** %d = %d\n", numberOne, numberTwo, exponent);
  }
  // Multiplication
  else if (arithmaticO[0] == 42 && arithmaticO[1] == '\0') {
    int product = multiplication(numberOne, numberTwo);
    // printf("%d * %d = %d\n", numberOne, numberTwo, product);
  }
  // Addition
  else if (arithmaticO[0] == 43 && arithmaticO[1] == '\0') {
    int sum = addition(numberOne, numberTwo);
    printf("%d + %d = %d\n", numberOne, numberTwo, sum);
  }
  // Subtraction
  else if (arithmaticO[0] == 45 && arithmaticO[1] == '\0') {
    int difference = subtraction(numberOne, numberTwo);
    // printf("%d - %d = ?\n", numberOne, numberTwo);
  }
  // Division
  else if (arithmaticO[0] == 47 && arithmaticO[1] == '\0') {
    int quotient = division(numberOne, numberTwo);
    // printf("%d / %d = ?\n", numberOne, numberTwo);
  }
  else {
    printf("Error: bad input.\n");
  }

  /*
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
  }
  */
}

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









int main(int argc, char *argv[4]) {
  char testInput[9] = "112 + 100";
  // int foo = fileLog(testInput);
  // fileLogRead("./.calculatorLog");
  // Interface functions including addition test '-t'
  int intInterface = interface(argc, argv[1]);
  return 0;
}
