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
#include "./src/window.h"

/*
Tested on headless Ubuntu with X11 server based GUI.

- compiling all the files but shorter:
gcc main.c ./src/*.c -o calculator200 && ./calculator200

Notes and todo - moved to "README.md"

  - multiplication 'test()' since it's only tested with these numbers:
  //printf("-lt\n");
  // Long print tests addition but 'interface()' expects a length of 0-4 char.
  // printf("Addition tests:\n");
  // int intInterface = interface(argc, "-lt\0");
  //int testOne = 21; // last row has a '?' at the end
  //int testTwo = 10;
  //int testOne = 15;
  //int testTwo = 11;
  //int testOne = 1;
  //int testTwo = 9;
  //int outputMultiplication =  multiplication(testOne, testTwo);
  //printf("output: %d\n", outputMultiplication);
*/

// 'version' and 'maxBinaryLength' declared in "binaryChar.h"
// 'version' and 'features' declared in the file "interface.h"
const char version[24] = "Calculator\nVersion 0.08\0";
const char features[44] = "Features: addition, input logging, testing.\0";

// 'maxBinaryLength' is used in "binaryChar.c" 'binaryAddition()'
const int maxBinaryLength = 17;

int main(int argc, char *argv[4]) {
  // Build a window using sockets without graphic libraries.
  // The 'char path[]' works since 'charLength()' is returning an int
  // and not the the orignal char[].
  char path[] = "/home/ubuntu/";
  int pathLength = charLength(path);
  char *mallocPath = malloc(pathLength * sizeof(char));
  strcpy(mallocPath, path);
  int connect = serverConnect(mallocPath);
  // The calculator CLI aka text GUI is shown if terminal doesn't ask for help.
  // change to ask for '-c' or something similar after the GUI.
  if (argc == 1) {
    //textUI();
    return 1;
  }
  else {
    // Interface functions are help '-h' or '-help' and test '-t' or
    // '-test' and print test '-lt' or '-longTest'.
    // int intInterface = interface(argc, argv[1]);
    return 0;
  }
}
