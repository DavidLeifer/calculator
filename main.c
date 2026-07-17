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
#include "./src/window.h" // migrated first two window functions prior to /.nanok/202607151522_main.c

/*
Notes
- compiling all the files by listing:
gcc main.c binaryFunctions.c arithmatic.c userInput.c -o binary && ./binary
- compiling all the files but shorter:
gcc main.c ./src/*.c -o calculator200 && ./calculator200
Tested on headless Ubuntu with X11 server based GUI.
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
const char version[24] = "Calculator\nVersion 0.05";
const char features[44] = "Features: addition, input logging, testing.";
// 'maxBinaryLength' is used in "binaryChar.c" 'binaryAddition()'
const int maxBinaryLength = 17;



////////////////////////////////////////////////////////////////////////////////////////////////
// Set two structs and several variables
// https://www.man7.org/linux/man-pages/man7/unix.7.html
// used in "server.c" and "client.c" for window.
#include <sys/socket.h>
#include <sys/un.h>
// ./usr/include/unistd.h
#include <unistd.h>

// 'window()' is called after steps 0-3 in 'serverConnect()' and input is the char[]
// contining window and root information from the socket connection's 'read()'.
// 4) Uses the window and root ID to send a packet containing window
//    drawing information to 'X11'.
// 5) Construct and send the MapWindow packet (Opcode 8).
void window(char *parentWindowID, char *windowID, char *visualID) {
  // Construct and send the CreateWindow packet (Opcode 1)
  //char *createWindowBuffer = malloc(32 * sizeof(char));
  //strcpy(createWindowBuffer, "a");
  unsigned char createWindowBuffer[32];
  //memset(createWindowBuffer, 0, sizeof(createWindowBuffer)); // 32
  // Kept everything 8-bit to reduce padding issue. '0b' doesn't count as a bit.
  // Opcode '1' = CreateWindow
  createWindowBuffer[0] = 0b00000001;
  createWindowBuffer[1] = 0;
  // Request length low byte: 8 words total (8 * 4 = 32 bytes)
  createWindowBuffer[2] = 0b00001000;
  createWindowBuffer[3] = 0;
  // Window ID
  // '48' or '0'
  createWindowBuffer[4] = windowID[0];
  createWindowBuffer[5] = windowID[1];
  createWindowBuffer[6] = windowID[2];
  createWindowBuffer[7] = windowID[3];
  // Parent window ID
  // '48' or '0'
  createWindowBuffer[8] = parentWindowID[0];
  createWindowBuffer[9] = parentWindowID[1];
  createWindowBuffer[10] = parentWindowID[2];
  createWindowBuffer[11] = parentWindowID[3];
  // Setting X
  //createWindowBuffer[12] = 0b11111010;
  // 250
  createWindowBuffer[12] = 0;
  createWindowBuffer[13] = 0;
  // Setting Y
  // 250
  //createWindowBuffer[14] = 0b11111010;
  createWindowBuffer[14] = 0;
  createWindowBuffer[15] = 0;
  // Width 128
  createWindowBuffer[16] = 0b10000000;
  // 2
  //createWindowBuffer[17] = 0b00000010;
  createWindowBuffer[17] = 0b00000001;
  // Height 128
  createWindowBuffer[18] = 0b10000000;
  // 1
  // createWindowBuffer[19] = 0b00000001;
  createWindowBuffer[19] = 0b00000001;
  // Border width.
  createWindowBuffer[20] = 0b00000001;
  // Border class.
  createWindowBuffer[21] = 0;
  // Window class - '1' means InputOutput window
  // and use 'visualID[]' in [24]-[27].
  createWindowBuffer[22] = 0b00000001;
  // Copy Parent ID with '0'
  //createWindowBuffer[22] = 0;
  createWindowBuffer[23] = 0;

  createWindowBuffer[24] = visualID[0];
  createWindowBuffer[25] = visualID[1];
  createWindowBuffer[26] = visualID[2];
  createWindowBuffer[27] = visualID[3];

  // Background color.
  createWindowBuffer[28] = 0;
  createWindowBuffer[29] = 0;
  createWindowBuffer[30] = 0;
  createWindowBuffer[31] = 0;
  //createWindowBuffer[32] = '\0';

  int sock = 3;
  // fails here
  // printf("Writing 'createWindowBuffer[]' data\n");
  int intWrite = write(sock, createWindowBuffer, sizeof(createWindowBuffer));
  //printf("write() int returns: %d\n", intWrite);
  unsigned char responseCreateWindow[50];
  int intReadCreateWindow = read(sock, responseCreateWindow, sizeof(responseCreateWindow)); // 50
  // printf("intReadCreateWindow returns: %d\n", intReadCreateWindow);
  printf("responseCreateWindow[0] returns: %d\n", responseCreateWindow[0]);

  int i = 0;
  while (i <= 50) {
    //printf("%d  %d\n", i, responseCreateWindow[i]);
    i++;
  }

  // 5) Construct and send the MapWindow packet (Opcode 8)
  unsigned char mapWindowBuffer[8];
  mapWindowBuffer[0] = 0b00001000;      // Opcode 8
  mapWindowBuffer[1] = 0;               // Unused padding byte
  mapWindowBuffer[2] = 0b00000010;      // Request length low byte
  mapWindowBuffer[3] = 0;               // Request length high byte
  mapWindowBuffer[4] = 0b110000;        // New window ID
  mapWindowBuffer[5] = 0;               //
  mapWindowBuffer[6] = 0;               //
  mapWindowBuffer[7] = 0;               //

  write(sock, mapWindowBuffer, sizeof(mapWindowBuffer)); // 8
  unsigned char responseMap[33];
  int intReadMap = read(sock, responseMap, 32); // 32

  printf("intReadMap() int returns: %d\n", intReadMap);
  printf("responseMap[0] returns: %d\n", responseMap[0]);
  //int ii = 0;
  //while (ii <= 32) {
  //  printf("%d  %d\n", ii, responseMap[ii]);
  //  ii++;
  //}
  printf("Window created via raw sockets! Keep process alive to view.\n");

  //Provide the ChangeWindowAttributes (Opcode 2) to
  //request input events like window closing or mouse clicks.

  // 6. Keep event loop active to maintain socket connection
  if (responseMap[0] == 1) {
    printf("Window created via raw sockets! Keep process alive to view.\n");
    getchar();
  }
  // 'socket()' closed in serverConnect();
}

int serverConnect(char userPath[]) {
  // 0. Format and send X11 Connection Setup request
  // 'userPath' is 'free()' in xAuthority.
  char *charAuthorityPath = xAuthority(userPath);
  // Open the file and get MIT-MAGIC-COOKIE-1 from user's '~/.Xauthority'.
  char *req = openAuthority(charAuthorityPath);
  free(charAuthorityPath);
  // 1. Establish connection to local X server socket
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  // printf("socket int returns: %d\n", sock);

  // From "un.h"
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  // unix path - copies max 108 char using struct from "un.h"
  strncpy(addr.sun_path, "/tmp/.X11-unix/X0", sizeof(addr.sun_path) - 1);
  int intConnect = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
  //printf("connect() int returns: %d\n", intConnect);

  // 2) 'write()' to 'X11' and 3) 'read()' the response to return ID data.
  //printf("Sending connection setup packet\n");
  //int intWrite = write(sock, &req, sizeof(req));

  // 'req' has 0 through 47 == 48 elements.
  int intWrite = write(sock, req, 48); // 48
  free(req);
  // printf("write() int returns: %d\n", intWrite);
  int intResponse = 256;
  // The requested elements are in 0-63 and 64 is the non-existent
  // null-terminator. 'read()' does not include a null-terminator.
  unsigned char responseHeader[intResponse];
  int intRead = read(sock, responseHeader, intResponse);
  // Supposed to be '64'.
  //printf("read() int returns: %d\n", intRead);
  // Supposed to be '1'.
  //printf("responseHeader[0] returns: %d\n", responseHeader[0]);

  // Heap the response char[] and 'free()' in 'getWindowID()'.
  // The length is the length of 'responseHeader' 'intRead'.
  // 'strcpy()' requries a null-terminator.
  // 'readSock' is 'free()' in 'getWindowID()'
  char *readSock = malloc((intResponse + 1) * sizeof(char));
  strcpy(readSock, "a");
  int i = 0;
  while (i <= (intResponse + 1)) {
    if (i == (intResponse + 1)) {
      readSock[i] = '\0';
    }
    else {
    readSock[i] = responseHeader[i];
    }
    //printf("%d readSock: %d\n", i, readSock[i]);
    i++;
  }
  // If the server returns 1, the connection was successfull and send
  // the heap pointer 'readSock' to extract the vendor length.
  if (readSock[0] == 1) {
    printf("ReadSock %d\n", readSock[0]);
    int screenOffset = getScreenOffset(readSock);
    char *parentID = getParentWindowID(readSock, screenOffset);
    char *windowID = getWindowID(readSock);
    char *visualID = getVisualID(readSock, screenOffset);
    if (screenOffset != 0) {
      window(parentID, windowID, visualID);
    }
  }
  else {
    // Otherwise print an error message.
    printf("X11 server connection failed, check read(), write(), connect()\n.");
  }
  close(sock);
  return 0;
}

int main(int argc, char *argv[4]) {
  // Input is usually "/home/<userName>/"
  // Parse the linux file "/etc/passwd" to get the username
  // user  :password: UID : GID
  // ubuntu:x       : 1000: 1000
  // where regular users are +1000 and loop until finding.
  // similar to snprintf with getenv("HOME") but that returns the
  // linux environmental variable.
  // 'snprintf()' is mostly 'xAuthority()' if user path was extracted
  // from "/etc/password". The goal was not to use 'getenv()' since early
  // versions of Python 2 and 3 required the user to write the environmental variable
  // and was annoying since there were multiple nearly identical OS providers.
  //snprintf(auth_path, sizeof(auth_path), "%s/.Xauthority", getenv("HOME"));
  //char *path = getenv("HOME");
  //printf("aaaaaaaa %s\n", path);

  // Steps to build a window using sockets without graphic libraries Xlib or sdl2+.
  // 0) Formats 'mallocPath' in 'xauthority()' through 'charAuthorityPath()' and
  //    'openAuthority()' to retrieve the cookie from '~/.Xauthority' file.
  //    These functions return a struct 'xAuthorityRequest' and are run in
  //    'serverConnect()' located below.
  // 1) Socket is opened with structs from 'un.h' and 'socket.h' at standard
  //    Unix path /tmp/.X11-unix/X0.
  // 2) Connection handshake with 'write()' and '.Xauthority' data to bypass GUI security.
  // 3) Reads the response and extracts the window ID and base resource ID.
  // 4) Formats a request to draw a window.
  char path[] = "/home/ubuntu/";
  int pathLength = charLength(path);
  char *mallocPath = malloc(pathLength * sizeof(char));
  strcpy(mallocPath, path);
  int connect = serverConnect(mallocPath);

  /*
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
  */
}

// This is from Google AI.
// "c draw a window x11 without libraries"

/*
To draw an X11 window without using any graphics libraries (like libX11 or libxcb),
you must communicate directly with the X Server by opening a Unix domain socket
and manually sending raw X11 network protocol packets.

Because the X11 Protocol is fundamentally a network protocol, a standard C program
can bypass all dynamic linking and manually craft the byte buffers required to
initiate the connection setup, allocate IDs, and issue a CreateWindow request.

The Core Protocol Steps
To get a window on the screen using only basic POSIX socket system calls, your
code must perform these sequential actions:

0-3 Completed.

  // The below code was in 'serverConnect()' and is replaced with 'openAuthority()'.
  // In one version the commmented out workflow returned the requested '1' that was
  // reversed with 'read()' since the request packet was 'B'. Writing the char[] to
  // heap with 'malloc()' failed at 'strcpy()' since ascii '0' are interpreted as
  // null-terminators and the remaining array values printed nonsense.

  // The next attempts included individual character spaces at '4 bytes'
  // using 4 single byte 'char' or '2 byte' ascii '0' and
  // also a single 4 byte int. The directions were confusing:
    // " The sizes of strings packed into the array must be padded to a multiple of 4 bytes.
    // "MIT-MAGIC-COOKIE-1" is 18 bytes -> needs 2 bytes padding to become 20.
    // The 16-byte cookie is already a multiple of 4 -> 0 bytes padding. "
  // 'X11' apparently expects bits from the compiler at that exact location regardless of
  // datatype byte length since a different method from the internet included another header
  // from the standard library '#include <stdint.h>' to use 'uint8_t = 1' or single byte int instead
  // of the two byte 'short int' = 1' found in the current working 'struct xAuthorityRequest = {}'.

  // Also tried the 1 byte 'uint8_t' datatype with it's header file but decided against even though
  // <stdint.h> is standard in the C99 library. It's an unsigned char[] or positive ASCII numbers.

  // The only other imports not from the regular three or my basic implementations of C++ or
  // Python functions include data structures for using the 'socket()' function to connect to
  // 'X11' with '/tmp/.X11-unix/X0'.

  / *
  char *setupInit = malloc(13 * sizeof(char)); // {0}
  strcpy(setupInit, "a");
  short int test = 48;
  setupInit[0] = 'B';       // Big endian byte order ('l' is little endian)
  setupInit[1] = test;
  setupInit[2] = '1';  // Protocol major version
  setupInit[3] = '1';         // Protocol major version
  setupInit[4] = '0';  // Protocol minor version
  setupInit[5] = test;         // Protocol minor version
  setupInit[6] = '1';  // 'name' length
  setupInit[7] = '8';         // 'name' length
  setupInit[8] = '1';  // 'cookie' length
  setupInit[9] = '6';         // 'cookie' length
  setupInit[10] = test;
  setupInit[11] = test;
  setupInit[12] = test;
  setupInit[13] = '\0';
  // Use 'charAppend()' from "utility.c" to append each character from the cookie name.
  // Allocate extra space for 2 ascii 0 (2 byte) instead of char '0' (1 byte)
  char *name = malloc(21 * sizeof(char));
  strcpy(name, "MIT-MAGIC-COOKIE-1  ");
  struct threeInt setupMid = charAppend(setupInit, 12, name, 20, 0);
  free(setupInit);
  free(name);
  setupMid.string[30] = test;
  setupMid.string[31] = test;
  setupMid.string[32] = '\0';
  // Same method to append the cookie data without padding.
  // 'charAppend()' null terminates.
  struct threeInt setup = charAppend(setupMid.string, setupMid.one, cookie, 16, 0);
  free(setupMid.string);
  free(cookie);
  printf("Socket char length %d\n", setup.one);
  int i = 0;
  while (i <= setup.one) {
    if (setup.string[i] == '\0') {
      printf("null terminated at %d\n", i);
      //break;
    }
    else if ((setup.string[i] >= 32) && (setup.string[i] <= 126)) {
      printf("%d  %c\n", i, setup.string[i]);
    }
    else {
      printf("%d  %02x\n", i, setup.string[i]);
    }
    i++;
  }
  * /

4-5 in progress

6.) Loop: Enter an infinite loop to prevent the connection from closing.

Length Fields: The X11 server expects sizes to be declared in 4-byte words. For instance,
a packet size of 32 bytes means your packet length field must read 8.

Bit Mask Values: Setting elements like background colors, event handlers, or custom borders
requires setting a bit inside a value-mask parameter field. You then append the raw settings
to the end of your transmission payload block.
*/
