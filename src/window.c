#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Set two structs and several variables
// https://www.man7.org/linux/man-pages/man7/unix.7.html
// used in "server.c" and "client.c" for window.
#include <sys/socket.h>
#include <sys/un.h>
// ./usr/include/unistd.h
#include <unistd.h>

#include "arithmaticSteps.h"
#include "struct.h"
#include "utility.h"
/*
  Steps to build a window using sockets without graphic libraries Xlib or sdl2+.
  Notes while writing the code with internet help is at the end of the file. The
  orignal documents from the 1990s are heavily advertised and full of spam.

  0) Formats 'mallocPath' in 'xauthority()' through 'charAuthorityPath()' and
     'openAuthority()' to retrieve the cookie from '~/.Xauthority' file.
     These functions return a struct 'xAuthorityRequest' and are run in
     'serverConnect()' located below.
  1) Socket is opened with structs from 'un.h' and 'socket.h' at standard
     Unix path /tmp/.X11-unix/X0.
  2) Connection handshake with 'write()' and '.Xauthority' data to bypass GUI security.
  3) Reads the response and extracts the window ID and base resource ID.
  4) Formats a request to draw a window.

*/

// 0) Extract MIT-MAGIC-COOKIE-1 from ~/.Xauthority
// Input is the user's path 'userPath'. Returns the path to ".Xauthority".
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
//char* xAuthority(char *userPath) {
char* xAuthority(char userPath[]) {
  int userPathLength = charLength(userPath);
  //char *charAuthority = malloc(12 * sizeof(char));
  //strcpy(charAuthority, ".Xauthority");
  char charAuthority[12] = ".Xauthority";
  struct threeInt charAuthorityPath = charAppend(userPath, userPathLength, charAuthority, 12, 0);
  //free(userPath);
  //free(charAuthority);
  // printf("%s\n", charAuthorityPath.string);
  return charAuthorityPath.string;
}

// 0) Opens '~/.Xauthority' using 'charAuthorityPath[]' and returns 'xAuthorityRequest' struct.
char* openAuthority(char *charAuthorityPath) {
  // https://stackoverflow.com/questions/70932880/what-is-the-internal-format-of-xauthority-file
  FILE *file = fopen(charAuthorityPath, "rb");
  char buffer[256];
  // Reads the file into 'buffer[]' with the number of items 'bufferItems'.
  int bufferItems = fread(buffer, sizeof(char), sizeof(buffer), file);
  buffer[bufferItems] = '\0';
  fclose(file);
  // Slices 'buffer[]' with 'slice()' from "utility.h".
  int familyStart = 0;
  int familyEnd = 2;
  char *family = slice(buffer, familyStart, familyEnd);
  unsigned short addressLength = buffer[3];
  int addressStart = 4;
  int addressEnd = 14;
  char *address = slice(buffer, addressStart, addressEnd);
  int nameStart = 20;
  int nameEnd = 37;
  char *name = slice(buffer, nameStart, nameEnd);
  unsigned short dataLength = buffer[39];
  int cookieStart = 40;
  int cookieEnd = 55;
  char *cookie = slice(buffer, cookieStart, cookieEnd);
  // Returning the requested '.Xauthority' data.
  // Keeping it char[]
  int intRequest = 48;
  char *mallocRequest = malloc(intRequest * sizeof(char));
  strcpy(mallocRequest, "a");
  mallocRequest[0] = 108; // ASCII for 'l';
  mallocRequest[1] = 0;
  mallocRequest[2] = 0b001011; // 11;
  mallocRequest[3] = 0;
  mallocRequest[4] = 0;
  mallocRequest[5] = 0;
  mallocRequest[6] = 0b010010; // 18;
  mallocRequest[7] = 0;
  mallocRequest[8] = 0b010000; // 16;
  mallocRequest[9] = 0;
  mallocRequest[10] = 0;
  mallocRequest[11] = 0;
  // Name with padding of three extra zeros.
  int i = 12;
  int j = 0;
  while (i < 33) {
    if (i == 32) {
      break;
    }
    else if (i < 30) {
      mallocRequest[i] = name[j];
    }
    else {
      mallocRequest[i] = 0;
    }
    i++;
    j++;
  }
  // The 'cookie' data into an int array.
  int k = 0;
  while (i < intRequest) {  // 48
    mallocRequest[i] = cookie[k];
    i++;
    k++;
  }
  // free() the char[].
  free(family);
  free(address);
  free(name);
  free(cookie);
  return mallocRequest;
}

// 2) Extract 'parentWindowID' and 'windowID' for 'createWindow' 'write()'
//    in 'drawWindow()' through 'serverConnect()'.
int getScreenOffset(char *readSock) {
  // Finds the length of the OS vendor and does not account for spaces.
  // For example default Linux Ubuntu is "The X.org foundation" and
  // is 20 characters long. Other distributions might have more or less.
  int i = 0;
  int vendorLength = 0;
  while (i < 65) {
    if ((i > 31) && (readSock[i] > 32 && readSock[i] < 127)) {
      //printf("%d  readSock = %c\n", i, readSock[i]);
      vendorLength++;
    }
    else {
      //printf("%d\n", i);
    }
    i++;
  }
  // Since there are two spaces, 'vendorLength' is incremented by 3 and
  // rounded down to a multiple of four. 'vendorRound' should be '20'.

  int vendorRound = (vendorLength + 3) & ~3;
  printf("vendor length multiple of 4 bytes = %d\n", vendorRound);
  // 'readSock[29]' is minus '8' to account for the first 'read()' that's
  // used to calculate the remaining bytes in the socket 'write()'.
  // The times '8' is unrelated.
  int numberFormats = readSock[21] * 8;
  // 'screenOffset' is also subtracted '8' digits from the usual
  // 40th character in 'readSock[]' = '32'.
  int screenOffset = numberFormats + vendorRound + 32;

  if (vendorRound == 20) {
    return screenOffset;
  }
  else {
    printf("'getScreenOffset' failed for 'getParentWindowID()', check 'vendorRound' length.");
    return 0;
  }
}

// 2a) 'windowID' function is used in 'createWindowBuffer' and all the other
//     'write()' requests i.e. 'getGeometry' or 'mapWindowBuffer'.
struct fourInt getWindowID(char *readSock) {
  struct fourInt windowID;
  // 'windowID' is 12-15 from the 'read()' char[] but
  // the first 'read()' length is '8'.
  windowID.one = readSock[4];
  windowID.two = readSock[5];
  windowID.three = readSock[6];
  windowID.four = readSock[7];
  return windowID;
}

// 2b) Functions to exchange identification data with X11 GUI.
//     'parentWindowID' is used with 'createWindowBuffer' in "drawWindow()".
struct fourInt getParentWindowID(char *readSock, int screenOffset) {
  struct fourInt parentWindowID;
  parentWindowID.one = readSock[screenOffset];
  parentWindowID.two = readSock[screenOffset + 1];
  parentWindowID.three = readSock[screenOffset + 2];
  parentWindowID.four = readSock[screenOffset + 3];
  return parentWindowID;
}

// 2c) Used in 'createWindowBuffer'.
struct fourInt getVisualID(char *readSock, int screenOffset) {
  int visualScreenOffset = screenOffset + 32;
  struct fourInt visualID;
  visualID.one = readSock[visualScreenOffset];
  visualID.two = readSock[visualScreenOffset + 1];
  visualID.three = readSock[visualScreenOffset + 2];
  visualID.four = readSock[visualScreenOffset + 3];
  free(readSock);
  return visualID;
}

// 'drawWindow()' is called after steps 0-3 in 'serverConnect()' and input is the char[]
// containing window and root information from the socket connection's 'read()'.
// 4) Uses the window and root ID to send a packet containing window
//    drawing information to 'X11'.
// 5) Construct and send the MapWindow packet (Opcode 8).
void drawWindow(struct fourInt windowID, struct fourInt parentWindowID, struct fourInt visualID) {
  // 4) Construct the CreateWindow packet (Opcode 1)
  unsigned char createWindowBuffer[32];
  // Kept everything 8-bit to reduce padding issue, I think you use regular decimals.
  // '0b' doesn't count as a bit.
  // Opcode '1' = CreateWindow
  createWindowBuffer[0] = 0b00000001;
  createWindowBuffer[1] = 0;
  // Request length low byte: 8 words total (8 * 4 = 32 bytes)
  createWindowBuffer[2] = 0b00001000;
  createWindowBuffer[3] = 0;
  // Window ID
  createWindowBuffer[4] = windowID.one;
  createWindowBuffer[5] = windowID.two;
  createWindowBuffer[6] = windowID.three;
  createWindowBuffer[7] = windowID.four;
  // Parent window ID
  createWindowBuffer[8] = parentWindowID.one;
  createWindowBuffer[9] = parentWindowID.two;
  createWindowBuffer[10] = parentWindowID.three;
  createWindowBuffer[11] = parentWindowID.four;
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
  // Width 128 + (2 * 256) = 640;
  createWindowBuffer[16] = 0b10000000;
  createWindowBuffer[17] = 0b00000010;
  // Height 128 + (2 * 256) = 640;
  createWindowBuffer[18] = 0b10000000;
  createWindowBuffer[19] = 0b00000010;
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
  createWindowBuffer[24] = visualID.one;
  createWindowBuffer[25] = visualID.two;
  createWindowBuffer[26] = visualID.three;
  createWindowBuffer[27] = visualID.four;
  // CWOverrideRedirect value is '1'.
  createWindowBuffer[28] = 0;
  createWindowBuffer[29] = 0;
  createWindowBuffer[30] = 0;
  createWindowBuffer[31] = 0;
  //createWindowBuffer[32] = '\0';

  // There's more explaination in latest window .c in the folder
  // '/windowStruct', the getID functions were changed to
  // not use char[] and heap memory and stopped working
  // when the window manager is disabled in 'createWindowBuffer'.

  // Maximum unsigned char array decimal is 128.
  // But little-endian byte-ordering allows larger numbers with
  // four 1-byte elements representing a 32 bit integer.
  //        [28] 128               =  128     +
  //        [29] 62 ( * 256)       =  15872   +
  //        [30] 0  ( * 65536)     =  ( 0 ) ) +
  //        [31] 0  ( * 26777216)  =  ( 0 ) )
  //                               =  16,000

  /*
  // not sure what [8] and [9] do.  2  "Lowest byte of 0x00000802 (CWBackPixel flag)" and
                                    8  "Highest active byte of 0x00000802 (CWEventMask flag)"
                                   // if you comment either out the background color is clear.
                                   // and if you change the high value to 4 instead of 8 it's the same window.

    // CWOverrideRedirect - 0x00004000 -    0b100000000000000  - 16384


    // This list is in hexidecimal but 'changeWindowAttributes' is mostly binary.
    // changeWindowAttributes[16] and [17] are a range of events to be captured with 'read()'    char[] int max is 128 (above) a>
    // "to listen for key presses and window exposure at the same time"                          you'd have to use an int array
    // For a calculator, you only need mouse button events but for other applciations            /\
                                                                                                 ints are 4 bytes vs char[] 1 by>

    // Key press            0x00000001
    // Event exposure       0x00008000 -    0b1000000000000000 - 32768

    // CWEventMask          0x00000800 -    0b100000000000     - 2048

    Core X11 Event Mask OptionsWhen using ChangeWindowAttributes, the value mask flag CWEventMask is 0x00000800.
    The actual event options you pass in the value list to select which events your window receives are defined below
    in hexadecimal format:
      Keyboard Events             0x00000001 (KeyPress): Fires when a key is pressed down.
                                  0x00000002 (KeyRelease): Fires when a key is released.
      Mouse Button Events         0x00000004 (ButtonPress): Fires when a mouse button is clicked.
                                  0x00000008 (ButtonRelease): Fires when a mouse button is released.
      Mouse Motion Events         0x00000010 (EnterWindow): Pointer enters the window boundaries.
                                  0x00000020 (LeaveWindow): Pointer leaves the window boundaries.
                                  0x00000040 (PointerMotion): Pointer moves inside the window.
     Window Exposure & Rendering  0x00008000 (Exposure): Part of the window becomes visible and needs redrawing.
     Window Lifetime & Management 0x00020000 (StructureNotify): Window is resized, moved, or destroyed.
                                  0x00040000 (ResizeRedirect): Intercepts resize requests from window managers.
                                  0x00100000 (PropertyChange): Window properties (like titles) are altered.
  */

  // 4) 'changeWindowAttributes' socket request (Opcode 2).
  char changeWindowAttributes[20];
  changeWindowAttributes[0] = 0b00000010;     // Opcode 2
  changeWindowAttributes[1] = 0;              //
  changeWindowAttributes[2] = 0b00000101;     // Request length ( n / 4)
  changeWindowAttributes[3] = 0;
  changeWindowAttributes[4] = windowID.one;
  changeWindowAttributes[5] = windowID.two;
  changeWindowAttributes[6] = windowID.three;
  changeWindowAttributes[7] = windowID.four;
  changeWindowAttributes[8] = 0b00000010;     // 2 CWBackPixel   - the background - defines which attributes are being altered.
  changeWindowAttributes[9] = 0b00001000;     // 8 CWBorderPixel - na
  changeWindowAttributes[10] = 0;
  changeWindowAttributes[11] = 0;
  changeWindowAttributes[12] = 0b11011100;    // B Background color.
  changeWindowAttributes[13] = 0b11011100;    // G
  changeWindowAttributes[14] = 0b11011100;    // R
  changeWindowAttributes[15] = 0;
  // Input Value List - The Event Mask (4 bytes, Little-Endian)
  // KeyPress + KeyRelease + ButtonPress + ButtonRelease + PointerMotion
  // Combined Input Mask (79)
  // KeyPress (0x00000001) + KeyRelease (0x00000002) = 0x00000003
  // ButtonPress (0x00000004) + ButtonRelease (0x00000008) = 0x0000000C
  // PointerMotion (Mouse movement) = 0x00000040
  changeWindowAttributes[16] = 0b01001111;    // Key press: 1 "to listen for key presses and window exposure at the same time"
  changeWindowAttributes[17] = 0;
  changeWindowAttributes[18] = 0;
  changeWindowAttributes[19] = 0;

  /*
  changeWindowAttributes[16] = 0b00000001;    // Key press: 1 "to listen for key presses and window exposure at the same time"
  changeWindowAttributes[17] = 0b00001000;    // 0b10000000; Event exposure - 128
  changeWindowAttributes[18] = 0;
  changeWindowAttributes[19] = 0;

  unsigned char captureWindowInput[16];
  captureWindowInput[0] = 0b00000010;     // Opcode 2 (ChangeWindowAttributes)
  captureWindowInput[1] = 0;              // Pad byte
  captureWindowInput[2] = 0b00000100;     // Length: 4 words total (16 bytes)
  captureWindowInput[3] = 0;              // Length high byte
  captureWindowInput[4] = windowID.one;
  captureWindowInput[5] = windowID.two;
  captureWindowInput[6] = windowID.three;
  captureWindowInput[7] = windowID.four;
  // Attribute Mask (4 bytes, Little-Endian)
  captureWindowInput[8] = 0;          // Mask: 0x00000800 (CWEventMask flag)
  captureWindowInput[9] = 0b00001000; // 8
  captureWindowInput[10] = 0;
  captureWindowInput[11] = 0;
  // Input Value List - The Event Mask (4 bytes, Little-Endian)
  // KeyPress + KeyRelease + ButtonPress + ButtonRelease + PointerMotion
  // Combined Input Mask (79)
  // KeyPress (0x00000001) + KeyRelease (0x00000002) = 0x00000003
  // ButtonPress (0x00000004) + ButtonRelease (0x00000008) = 0x0000000C
  // PointerMotion (Mouse movement) = 0x00000040
  captureWindowInput[12] = 0b01001111;
  captureWindowInput[13] = 0;
  captureWindowInput[14] = 0;
  captureWindowInput[15] = 0;
  */

  // Write geometry packet.
  unsigned char getGeometry[8];
  getGeometry[0] = 14;
  getGeometry[1] = 0;
  getGeometry[2] = 2;
  getGeometry[3] = 0;
  getGeometry[4] = windowID.one;
  getGeometry[5] = windowID.two;
  getGeometry[6] = windowID.three;
  getGeometry[7] = windowID.four;

  // 5) Construct the MapWindow packet (Opcode 8)
  unsigned char mapWindowBuffer[8];
  mapWindowBuffer[0] = 0b00001000;      // Opcode 8
  mapWindowBuffer[1] = 0;               // Unused padding byte
  mapWindowBuffer[2] = 0b00000010;      // Request length low byte
  mapWindowBuffer[3] = 0;               // Request length high byte
  mapWindowBuffer[4] = windowID.one;    // New window ID
  mapWindowBuffer[5] = windowID.two;    //
  mapWindowBuffer[6] = windowID.three;  //
  mapWindowBuffer[7] = windowID.four;   //

  // 4) 'write()' the 'createWindow' socket request.
  int sock = 3;
  int windowWrite = write(sock, createWindowBuffer, sizeof(createWindowBuffer));

  // 4) 'write()' the 'changeWindowAttributes'.
  int changeWindowWrite = write(sock, changeWindowAttributes, sizeof(changeWindowAttributes));

  // 4) 'write()' the 'getGeometry' socket request to avoid hanging program.
  int geometryWrite = write(sock, getGeometry, sizeof(getGeometry));
  // printf("write() int returns: %d\n", intWrite);
  //unsigned char responseCreateWindow[32];
  //int intReadCreateWindow = read(sock, responseCreateWindow, sizeof(responseCreateWindow));
  // printf("intReadCreateWindow returns: %d\n", intReadCreateWindow);
  //printf("responseCreateWindow[0] returns: %d\n", responseCreateWindow[0]);

  // 5) 'write()' the 'mapWindowBuffer' socket request to view the window.
  write(sock, mapWindowBuffer, sizeof(mapWindowBuffer)); // 8


  unsigned char responseMap[32];
  int intReadMap = read(sock, responseMap, sizeof(responseMap)); // 32

  //printf("intReadMap() int returns: %d\n", intReadMap);
  //printf("responseMap[0] returns: %d\n", responseMap[0]);
  //int ii = 0;
  //while (ii <= 32) {
  //  printf("%d  %d\n", ii, responseMap[ii]);
  //  ii++;
  //}
  //printf("Window created via raw sockets! Keep process alive to view.\n");

  //Provide the ChangeWindowAttributes (Opcode 2) to
  //request input events like window closing or mouse clicks.
  // 6) Keep event loop active to maintain socket connection
  if (responseMap[0] == 1) {
  //if (responseMap[0] == 0) {
    printf("Window created via raw sockets! Keep process alive to view.\n");
    //getchar();
    while (1) {
      unsigned char responseWindowInput[32];
      int changeWindowInput = read(sock, responseWindowInput, 32);
      int eventCode = responseWindowInput[0];
      //printf("eventcode = %d\n", eventCode);
      if (eventCode == 2) {
        // KeyPress Event occurred!
        int keyInput = responseWindowInput[1] + 29; // Physical key matrix index
        printf("keypress = %d\n", keyInput);
        int zero = 48;
        if (keyInput == zero) {
          printf("a = %d\n", keyInput);
        }
      }
      else if (eventCode == 4) {
        // ButtonPress (Mouse click) occurred!
        int button = responseWindowInput[1]; // 1 = Left Click, 2 = Middle, 3 = Right
        // Extract mouse X and Y coordinates (Bytes 24-27)
        int mouseX = responseWindowInput[24] | (responseWindowInput[25] << 8);
        int mouseY = responseWindowInput[26] | (responseWindowInput[27] << 8);

        printf("button = %d\n", button);

        printf("responseWindowInput[24] = %d\n", responseWindowInput[24]);
        printf("responseWindowInput[25] = %d\n", responseWindowInput[25]);

        printf("mouseX = %d\n", mouseX);
        printf("mouseY = %d\n", mouseY);

        printf("responseWindowInput[26] = %d\n", responseWindowInput[26]);
        printf("responseWindowInput[27] = %d\n", responseWindowInput[27]);
      }
    }
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

  // getHeader();
  // printf("write() int returns: %d\n", intWrite);
  // The first response is used to calculate the length of
  // the entire response.
  int intHeaderLength = 8;
  unsigned char header[intHeaderLength];
  int intHeaderReturn = read(sock, header, intHeaderLength);
  // This is a bitwise comparison between the binary representation
  // of '3' and '147' where if either of the values is '1', the result
  // is '1'.  The '<<' shifts binary digits to the left and its equivalent
  // is (2 to the power of 8).
  // int reply_length = ((header[7] << 8) | header[6]) * 4;
  // printf("reply_length  %d\n", reply_length);
  ///////////////////////////////////////////////////////////////////////

  // The workflow without bitwise is converting
  // to 'char binary[]' as outlined in 'arithmaticSteps.c' and writing
  // a function that loops over the length of the longest binary roughly:
  // int i = 0;
  // while (i < longestBinary) {
  //   if (binaryHeader[7][i] == 1 || binaryHeader[6][i] == 1) {
  //     outputHeader[i] = 1;
  //   else {
  //     outputHeader[i] = 0;
  //   }
  //   i++;
  // }
  int baseDecimal = 2;
  int exponentDecimal = 8;
  int replyExponents = exponents(baseDecimal, exponentDecimal);
  int intResponse = ( (header[7] * (replyExponents) ) | header[6]) * 4;

  // printf("write() int returns: %d\n", intWrite);
  // '256' was the last length tested before the internet suggested
  // using two 'read()' and calculating the length of the second with
  // values from the first response to avoid overflows when
  // drawing the window.
  // int intResponse = 256;
  //int intResponse = 64;
  // If the 'read()' char[] length is '64', the requested elements are mostly in
  // 0-63 and 64 is the non-existent null-terminator. 'read()' does not include
  // a null-terminator. The parentID and windowID are calculated using the length
  // of the vendor name i.e. 'The X.org foundation'. The internet said the entire
  // 'responseHeader' from 'read()' was 100-16,000.
  //printf("replyLength     %d\n", intResponse);
  // The requested elements are in 0-63 and 64 is the non-existent
  // null-terminator. 'read()' does not include a null-terminator.
  unsigned char responseHeader[intResponse];
  int intRead = read(sock, responseHeader, intResponse);
  // / *
  //int count = 0;
  //while (count < intResponse) {
    //printf("%d  %c\n", count, responseHeader[count]);
  //  count++;
  //}
  // * /

  // Supposed to be 'intResponse'.
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
  // the heap pointer 'readSock' to 'getScreenOffset()' and extract
  // the vendor length.
  if (header[0] == 1) {
    printf("ReadSock %d\n", header[0]);
    // screenOffset + 8
    int screenOffset = getScreenOffset(readSock);
    struct fourInt windowID = getWindowID(readSock);
    struct fourInt parentID = getParentWindowID(readSock, screenOffset);
    struct fourInt visualID = getVisualID(readSock, screenOffset);
    if (screenOffset != 0) {
      drawWindow(windowID, parentID, visualID);
    }
  }
  else {
    // Otherwise print an error message.
    printf("X11 server connection failed, check read(), write(), connect()\n.");
  }
  close(sock);
  return 0;
}


/*
// This is from Google AI responses.
// "c draw a window x11 without libraries"

To draw an X11 window without using any graphics libraries (like libX11 or libxcb),
you must communicate directly with the X Server by opening a Unix domain socket
and manually sending raw X11 network protocol packets.

Because the X11 Protocol is fundamentally a network protocol, a standard C program
can bypass all dynamic linking and manually craft the byte buffers required to
initiate the connection setup, allocate IDs, and issue a CreateWindow request.

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
  // from the standard library '#include <stdint.h>' to use 'uint8_t = 1' or single byte int inste>
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

Length Fields: The X11 server expects sizes to be declared in 4-byte words. For instance,
a packet size of 32 bytes means your packet length field must read 8.

Bit Mask Values: Setting elements like background colors, event handlers, or custom borders
requires setting a bit inside a value-mask parameter field. You then append the raw settings
to the end of your transmission payload block.
*/
