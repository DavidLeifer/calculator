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

  // listing all the files: gcc main.c binaryFunctions.c arithmatic.c userInput.c -o binary && ./binary

gcc main.c ./src/*.c -o binary && ./binary

todo

  - append function in the log file
  - fix addition using test() and log

minor todo

  - fix inputArithmatic variable names with new struct int pointers and malloc.

*/

const char version[23] = "Calculator\nVersion 0.02";
const char features[44] = "Features: addition, input logging, testing.";

int main(int argc, char *argv[4]) {

  // fileLogRead("./.calculatorLog");
  // The calculator text interface is shown if terminal doesn't ask for help.
  if (argc == 1) {
    textGUI();
    return 1;
  }
  else {
    // Interface functions including addition test '-t'
    int intInterface = interface(argc, argv[1]);
  return 0;
  }
}
