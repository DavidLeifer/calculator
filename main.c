#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "./src/struct.h"
#include "./src/utility.h"
#include "./src/binaryChar.h"
#include "./src/arithmatic.h"
#include "./src/arithmaticSteps.h"
#include "./src/userInput.h"
#include "./src/interface.h"
/*
Notes
// listing all the files: gcc main.c binaryFunctions.c arithmatic.c userInput.c -o binary && ./binary
gcc main.c ./src/*.c -o binary && ./binary
todo
  - multiplication, division.
minor todo
  - char length function.
  - fix inputArithmatic variable names with new struct int pointers and malloc.
  - use 'maxBinaryLength' in "binaryChar.c"
  - subtraction() and addition() use same function series to convert int to char[] binary.
  - test 'fileLog()' error messages from 'subtraction()' and 'binaryInvert()'.
*/
// 'version' and 'maxBinaryLength' declared in "binaryChar.h"
// 'version' and 'features' declared in the file "interface.h"
const char version[24] = "Calculator\nVersion 0.03";
const char features[44] = "Features: addition, input logging, testing.";
// This is used in "binaryChar.c" 'binaryAddition()'
const int maxBinaryLength = 17;

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
