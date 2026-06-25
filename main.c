#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "./src/struct.h"
#include "./src/utility.h"
#include "./src/binaryChar.h"
#include "./src/arithmatic.h"
#include "./src/userInput.h"
#include "./src/interface.h"

/*
Notes

  Have to specify function calls in binaryFunctions.h and here besides the
  actual binaryFunctions.c file.

  // listing all the files: gcc main.c binaryFunctions.c arithmatic.c userInput.c -o binary && ./binary

gcc main.c ./src/*.c -o binary && ./binary

todo

  - Char length function.
  - Remove userInput formatting in .log and make function.
    - Modify .log function input to write any char[] input.

  - fix addition using test() and log.

minor todo

  - fix inputArithmatic variable names with new struct int pointers and malloc.

*/

const char version[24] = "Calculator\nVersion 0.02";
const char features[44] = "Features: addition, input logging, testing.";

int main(int argc, char *argv[4]) {
  // The calculator text GUI is shown if terminal doesn't ask for help.
  if (argc == 1) {
    textUI();
    return 1;
  }
  else {
    // Interface functions are help '-h' or '-help' and test '-t' or
    // '-test' and print test '-lt' or '-longTest'.
    int intInterface = interface(argc, argv[1]);
    return 0;
  }
}
