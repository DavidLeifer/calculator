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
Notes
- compiling all the files by listing:
gcc main.c binaryFunctions.c arithmatic.c userInput.c -o binary && ./binary
- compiling all the files but shorter:
gcc main.c ./src/*.c -o calculator200 && ./calculator200
Tested on headless Ubuntu with X11 server based GUI.

Notes
1) char *string = "Hello";
   string[0] = "Y"; -> doesn't work since pointer is immutable unlike 'malloc' -> 'strcpy'.
2) char otherString[6] = "Hello"; -> includes space for a null-terminator and can be
   returned from array with 'malloc' -> 'strcpy'.
     - sizeof(otherString) returns the length of the char[].
- Reviewed "binaryChar.c", "arithmatic.c, arithmaticSteps.c, interface.c, userInput.c"
  - line 112 the 2 'charBinary2Int()' functions exist because the first 'struct' has three
    int and one char.
3) 'const' can't be sent from "oneFile.c" to use another function from "twoFile.c" unless
   the 'const' is set as another variable with 'malloc()' or 'while{}' even if it's a
   pointer. The other function has to already accept a 'const' which limits basic
   functions from "utility.c".
4) 20260726 - 'char' pointer with 'malloc()' doesn't need 'strcpy(dest, "a")' unlike
              previous versions. It also doesn't fill the array with duplicate "a" the
              length of the 'malloc()' 'char' pointer since 'memset()' suddenly exists.
            - 'charAppend()' in "utility.c" 'strcpy()' was commented out but it might break stuff later.

todo
  - multiplication, division.
    - line 521 - 584: reduce times this is set:
      diffrence 'intBinaryGreatTwo.two = 1' and (first input is longer) 'intBinaryGreatTwo.three = 1'
  - "window.c" GUI.

minor todo
  - fix inputArithmatic variable names with new struct int pointers and malloc.
  - subtraction() and addition() use same function series to convert int to char[] binary.
  - test 'fileLog()' error messages from 'subtraction()' and 'binaryInvert()'.
  - 'serverConnect()' in "window.c" has a bitwise comparison that's mostly in "binaryChar.c".
  - 'multiplication()' in "arithmaticSteps.c"
    - Probably don't need 'binaryAddCheck' for the if/else since 'intBinaryGreatTwo.three' says
      which one is longer with '0' '1' '2' but the extra int is four extra bytes.
    - 'binaryAddition()' in "arithmatic.c" uses a 'freeCheck' to 'free()' the inputs and the "arithmatic.c".
      functions were changed. The deceased 'binaryAdditionTwo()' used 'free()' on the second input char[] pointer
      since 'struct binaryAdd' is both the input and the output. An 'int' check of '0' '1' '2' '3'
      is used to free in the first 'binaryAddition()', removing 'binaryAdditionTwo()' but doesn't
      reduce heap usage (actually increases stack memory by (4 * the number) of times it's used throughout
      // the entire program (~4-6 times)).
      // might not need to return the length of the char pointer or use a struct at all
      // not sure if the struct is too large since its only a 'int' a 'char pointer', and two blank 'int'.

      //struct threeInt intBinaryGreat = intBinaryCheck(intBinaryOne, intBinaryTwo);
      /////////////////////////////////////////////////////
      // or use the function since 'struct threeInt intBinaryGreatThree' has a blank char[] pointer on heap the entire while loop
      // in addition to 'struct threeInt binaryAdd' which also exists the entire time.
      // 'binaryAdd.string' and either 'zeroStep' or 'oneStep' are the same length


*/
// 'version' and 'maxBinaryLength' declared in "binaryChar.h"
// 'version' and 'features' declared in the file "interface.h"
const char version[24] = "Calculator\nVersion 0.06\0";
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
  //int connect = serverConnect(mallocPath);
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
  // The CLI
  // The calculator text GUI is shown if terminal doesn't ask for help.
  // change to ask for '-c' or something similar after the GUI.
  if (argc == 1) {
    textUI();
    return 1;
  }
  else {
    // Interface functions are help '-h' or '-help' and test '-t' or
    // '-test' and print test '-lt' or '-longTest'.
    // int intInterface = interface(argc, argv[1]);
    return 0;
  }
}
