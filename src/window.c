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
     - Opcode 1, 2, 14, 8.


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
  //printf("vendor length multiple of 4 bytes = %d\n", vendorRound);
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

// 2b) 'gcID' function is used in 'write()' requests i.e. 'createGC',
//     'polyRectangle', 'polyFillRectangle'. There is a longer explanation and
//      there is seems to be a writeable range for 'graphic context ID'. This one
//      is 'windowID' plus 2 but for larger program GUI there would probably a
//      more exact method of generating ID for each graphic object so they don't conflict.
struct fourInt getGCID(char *readSock, struct fourInt windowID) {
  struct fourInt gcID;
  gcID.one = readSock[4] + 2;
  gcID.two = readSock[5];
  gcID.three = readSock[6];
  gcID.four = readSock[7];

  // The internet did not want to answer this question:
  // The 'resource_id_mask' (windowID) and 'resource_id_base' is the first char[]
  // response. Since the first char[8] captures the first 8 elements, the values are
  // are 'readSock - 8'. You could not use 'windowID' to save 4 bytes but it is less clear.
  //         Blank char[] readSock[4]-[7]  readSock[0]-[3]

  // the setup for ID might be more complciated later on.
  // base - this is used as the windowID but computer said not to.
  //printf("gcID 1-4: %d  %d  %d  %d\n", readSock[0], readSock[1], readSock[2], readSock[3]);
  //int id_base = 12143104;  // 12,143,104 (readSock 0-4)
  //int id_mask = 111303168; // 111,303,168 (readSock 4-7)

  //int min_val = id_base | 1;
  //int max_val = id_base | id_mask;
  //printf("id_base: %d  id_mask: %d  min_val: %d  max_val: %d\n", id_base, id_mask, min_val, max_val);
  // min_val: 12,143,105  max_val: 112,941,568
  //printf("windowID 1-4: %d  %d  %d  %d\n", windowID.one, windowID.two, windowID.three, windowID.four);

  // acceptable range might be between 2 - 4 million
  //printf("gcID 1-4: %d  %d  %d  %d\n", readSock[0], readSock[1], readSock[2], readSock[3]);

  /*
  // The provided example increments the gcID for other graphic changing. Or you could + 1
  // and manually calculate if there is an overflow.
  // 'windowID' is 12-15 from the 'read()' char[] but the first 'read()'
  // length is '8'. (minus 8 elements).
  // Bytes 12–15: resource_id_mask.
  // Bytes 8–11: resource_id_base.
  unsigned char counter[4];
  // & bitwise compares each bit and sets to '1' only if both are '1'.
  // | bitwise compares each bit and sets to '1' if either are '1'.
  gcID.one = (counter[0] & windowID.one) | readSock[0];
  gcID.two = (counter[1] & windowID.two) | readSock[1];
  gcID.three = (counter[2] & windowID.three) | readSock[2];
  gcID.four = (counter[3] & windowID.four) | readSock[3];
  // Helper function to increment a 4-byte array like a standard integer (handles carrying)
  // used in button loop.
  void increment_counter_array(unsigned char *counter) {
    if (++counter[0] == 0) {       // If byte 0 overflows, carry to byte 1
        if (++counter[1] == 0) {   // If byte 1 overflows, carry to byte 2
            if (++counter[2] == 0) {
                ++counter[3];      // Carry to byte 3
            }
        }
    }
  }
  increment_counter_array(x11_counter);
  */

  return gcID;
}

// 2c) Functions to exchange identification data with X11 GUI.
//     'parentWindowID' is used with 'createWindowBuffer' in "drawWindow()".
struct fourInt getParentWindowID(char *readSock, int screenOffset) {
  struct fourInt parentWindowID;
  parentWindowID.one = readSock[screenOffset];
  parentWindowID.two = readSock[screenOffset + 1];
  parentWindowID.three = readSock[screenOffset + 2];
  parentWindowID.four = readSock[screenOffset + 3];
  return parentWindowID;
}

// 2d) Used in 'createWindowBuffer'.
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

////////////////////////////////////////////////////
//    todo - calculator layout -> for int input
//           - continuous mouse coordiantes based on event
//             to verify border values
//    todo - getGCID() for opcode 67 change border color
//    todo - keyboard input display text box
//    todo - arithmatic buttons (todo - division)
//    minor todo - there are decimals randomly in
//                 the char array instead of binary.

////////////////////////////////////////////////////
// Explanation:
// 'drawWindow()' is called after steps 0-3 in 'serverConnect()' and input is several struct
// containing windowID information from the initial socket connection's 'read()' in 'serverConnect()'.
// 4) Uses window, parent ID, and manipulates the signed/unsigned char[] minimum with
// byte encoding to send packets containing window drawing information to the 'X11' server.
//   Opcode 1, 2, 14, 55
//   createWindowBuffer, changeWindowAttributes, getGeometry, createGC
//                                             /\ there's another opcode 2 to disable the window
//                                                manager which removes the top bar and allows
//                                                specific X,Y coordinates.
// There is a 'read()' after 'getGeometry' that returns if the previous 'write()' were successful.
// The returned first value is '1', the while loop in step 6 begins.
// 5) Opcode 8 - send 'MapWindowBuffer' - char[] to draw the window with the above listed attributes.
//    Opcode 67, 70 packets are assembled with 'while' loops.
//      - optional is opcode 42 to resume keyboard input after disabling the window manager in 4).
//      - may or may not have to 'write()' opcode 67 and 70 first before the loop
// 6) Continuous while loop asking for int user input with the 'X11' keyboard codes replaced with ASCII.
//    'polyRectangle' and 'polyFillRectangle' wait for 'eventCode' 12 or 'Expose' event.

//////////////////////////////////////////////////////////////////////////
// 4) This is a long explanation to capture user input for opcode 1 and 2:
  // Note from internet: "If you plan to use CWOverrideRedirect, your Request Length needs to change
  //        to 9,your value mask at byte 28 needs to be 0x00000200, and you must append 4 more bytes
  //        containing the value 1 at the end of the buffer." may or may not be accurate.

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

  // 6) Applicable to the continuous loop.
  // Physical key matrix index
  // 'X11' is not used in recent ubuntu or debian distributions anymore since it was
  // replaced by 'Wayland'. Linux on 'X11' regardless of hardware mostly uses 'ASCII - 39' for
  // number entries. One way is to make a char[] but loops in the continuing 'while' loop is
  // computational expensive. Since a calculator only needs numerical input, a conditional
  // will be used to hardcode each number. The concept is similar to hardware with limited
  // space for transistors but is probably more like the hardware instructions.
  // responseWindowInput[1] + 39;
  // i.e.  10               + 39 = 49
  //                             = 1

  // 36 = enter
  // 9 = esc
  // 61 = delete
  // GetKeyboardMapping also exists to avoid hardcoding for other keyboards.

  // You could probably alter the standard library to bypass the 'enter' requirement
  // for 'getchar();' and do something similar but 'C' and linux versions change very often.

  // todo
  // responseWindowInput[1];
  // 1 = Left Click, 2 = Middle, 3 = Right

  // Extract mouse X and Y coordinates (Bytes 24-27)
    // responseWindowInput[24] | (responseWindowInput[25] << 8);
    // responseWindowInput[26] | (responseWindowInput[27] << 8);


///////////////////////////////////
// This information is more useful.
///////////////////////////////////
// unsigned char is  0 - 255
// signed char is -127 - 127
// little-endian byte-ordering allows larger numbers with
// four 1-byte elements representing a 32 bit integer.
//        [28] 128               =  128     +
//        [29] 62 ( * 256)       =  15872   +
//        [30] 0  ( * 65536)     =  ( 0 ) ) +
//        [31] 0  ( * 26777216)  =  ( 0 ) )
//                               =  16,000

void drawWindow(struct fourInt windowID, struct fourInt gcID, struct fourInt parentWindowID, struct fourInt visualID) {
  // 4) Construct the CreateWindow packet (Opcode 1)
  unsigned char createWindowBuffer[32];
  // You can use regular decimals but input is 8-bit to retain > 256 binary.
  // '0b' doesn't count as a bit.
  // Opcode '1' = CreateWindow
  createWindowBuffer[0] = 0b00000001;
  createWindowBuffer[1] = 0; // original value: 0 // suggested "Depth must match visualID depth (24 or 32)"
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
  // Setting X from left to right
  createWindowBuffer[12] = 0;
  createWindowBuffer[13] = 0b00000100; // 4 X 256 maybe
  // Setting Y from top to bottom
  createWindowBuffer[14] = 0b11111010; // 250 X 1
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
  changeWindowAttributes[8] = 0b00000010;     // 2 CWBackPixel   - the background - defines which attributes are being>
  changeWindowAttributes[9] = 0b00001000;     // 8 CWBorderPixel - na
  changeWindowAttributes[10] = 0;
  changeWindowAttributes[11] = 0;
  changeWindowAttributes[12] = 0b11011100;    // B  Background color.
  changeWindowAttributes[13] = 0b11011100;    // G
  changeWindowAttributes[14] = 0b11011100;    // R
  changeWindowAttributes[15] = 0;             //
  // Input Value List - The Event Mask (4 bytes, Little-Endian)
  // KeyPress + KeyRelease + ButtonPress + ButtonRelease + PointerMotion
  // Combined Input Mask (79)
  // KeyPress (0x00000001) + KeyRelease (0x00000002) = 0x00000003
  // ButtonPress (0x00000004) + ButtonRelease (0x00000008) = 0x0000000C
  // PointerMotion (Mouse movement) = 0x00000040
  // This got altered to capture the 'Expose' event in the continuous loop 6).
  //changeWindowAttributes[16] = 0b01001111;
  //                           = 0;
  //                           ... etc
  // + the 'Expose' redraw event is 0x800D or '32781'
  // 0D, 80
  // 13, 128
  changeWindowAttributes[16] = 13;
  changeWindowAttributes[17] = 128;
  changeWindowAttributes[18] = 0;
  changeWindowAttributes[19] = 0;

  // 4) 'CWOverrideRedirect' using 'ChangeWindowAttributes'
  unsigned char overrideRedirect[16];
  overrideRedirect[0] = 0b00000010;           // Opcode 2 (ChangeWindowAttributes)
  overrideRedirect[1] = 0;
  overrideRedirect[2] = 0b00000100;           // Length = 4 words 16
  overrideRedirect[3] = 0;
  overrideRedirect[4] = windowID.one;
  overrideRedirect[5] = windowID.two;
  overrideRedirect[6] = windowID.three;
  overrideRedirect[7] = windowID.four;
  overrideRedirect[8] = 0;                    // Value Mask 0 X 1
  overrideRedirect[9] = 0b00000010;           //          512 X 2 (CWOverrideRedirect flag)
  overrideRedirect[10] = 0;
  overrideRedirect[11] = 0;
  overrideRedirect[12] = 0b00000001;          // CWOverrideRedirect value is '1' - bypasses window manager.
  overrideRedirect[13] = 0;
  overrideRedirect[14] = 0;
  overrideRedirect[15] = 0;

  // 4) Geometry packet.
  unsigned char getGeometry[8];
  getGeometry[0] = 14;
  getGeometry[1] = 0;
  getGeometry[2] = 0b00000010;
  getGeometry[3] = 0;
  getGeometry[4] = windowID.one;
  getGeometry[5] = windowID.two;
  getGeometry[6] = windowID.three;
  getGeometry[7] = windowID.four;

  // 4) opcode 55 change the rectangle fill color
  unsigned char createGC[20];
  createGC[0] = 55; // CreateGC
  createGC[1] = 0;
  createGC[2] = 5; // Bytes 2-3: Request length in 4-byte units (24 bytes / 4 = 6)
  createGC[3] = 0;
  createGC[4] = gcID.one;
  createGC[5] = gcID.two;
  createGC[6] = gcID.three;
  createGC[7] = gcID.four;
  createGC[8] = windowID.one;
  createGC[9] = windowID.two;
  createGC[10] = windowID.three;
  createGC[11] = windowID.four;
  // Bytes 12-15: Value Mask (Bit 2 is Foreground attribute -> 0x00000004)
  createGC[12] = 4;
  createGC[13] = 0;
  createGC[14] = 0;
  createGC[15] = 0;
  // BGR color
  createGC[16] = 255;
  createGC[17] = 255;
  createGC[18] = 255;
  createGC[19] = 0;

  // 5) Construct the MapWindow packet (Opcode 8)
  unsigned char mapWindowBuffer[8];
  mapWindowBuffer[0] = 0b00001000;      // Opcode 8
  mapWindowBuffer[1] = 0;               // Unused padding byte
  mapWindowBuffer[2] = 0b00000010;      // Request length low byte 2
  mapWindowBuffer[3] = 0;               //
  mapWindowBuffer[4] = windowID.one;    // New window ID
  mapWindowBuffer[5] = windowID.two;    //
  mapWindowBuffer[6] = windowID.three;  //
  mapWindowBuffer[7] = windowID.four;   //
  // the 32 byte response has 12-19 for x,y,width,height information.

  // 5) 'polyRectangle' opcode 67 for border
  // 20 rectangle borders arranged in 4 X 5 format used for clicking screen buttons in step
  // 6 that records user input.
  int intButtonX = 100;
  int intButtonXPlus = 100;
  int intButtonY = 100;
  int intButtonWidth = 50;
  int intButtonHeight = 50;
  int numberButtons = 21; // n - 1
  int buttonElements = 20;
  int buttonXSpace = 60;
  int buttonYSpace = 60;
  int buttonColumns = 4;
  // a  a  a  /
  // 7  8  9  X
  // 4  5  6  -
  // 1  2  3  +
  // (  )  0  =
  // dynamic example:
  // char (*arrays)[10] = malloc(N * sizeof(*arrays));
  // 5 X 4 = 20 buttons
  // Reused variables in the loop for the int to unsigned char 'buttonBorder[12]-[15]' overflow.
  int intButtonXPlusRemainder;
  int intButtonXPlusDivision;
  int intButtonYRemainder;
  int intButtonYDivision;
  unsigned char buttonBorder[numberButtons][buttonElements];
  int i = 1;
  //int j = 0; //
  int k = 0;
  // Reused variables in the loop for the int to unsigned char 'buttonBorderXXYY[0]-[3]' overflow.
  int buttonBorderXDivision;
  int buttonBorderXRemainder;
  int buttonBorderX12;
  int buttonBorderX13;
  int buttonBorderYDivision;
  int buttonBorderYRemainder;
  int buttonBorderY14;
  int buttonBorderY15;
  // 4 * 20 elements since its 2 bytes for 'X' and 2 for 'Y'.
  char buttonBorderXXYY[80];
  while (i < numberButtons) {
    // 'k' is used to increment elements for 'buttonBorderXXYY[0] - [79]'.
    k = (i - 1) * 4;
    // Accounts for overflow since unsigned char range is 0-255.
    // Each int coordinate is represented with two sequential bytes in the char[].
    // The X or horizontal coordinate.
    if (intButtonXPlus < 256) {
      buttonBorder[i][12] = intButtonXPlus;
      buttonBorder[i][13] = 0;
    }
    else {
      // If the incrementing 'intButtonXPlus' exceeds 256, it overflows to the next element using division.
      // i.e. buttonBorder[i][12] = 'value'; // below 256
      //      buttonBorder[i][13] = 'value' * 256;
      intButtonXPlusRemainder = intButtonXPlus % 256;
      intButtonXPlusDivision = intButtonXPlus / 256;
      //printf("intButtonXPlusRemainder %d = intButtonXPlus %d  %  256 \n\n", intButtonXPlusRemainder, intButtonXPlus);
      //printf("intButtonXPlusDivision %d = intButtonXPlus %d  /  256 \n\n", intButtonXPlusDivision, intButtonXPlus);
      buttonBorder[i][12] = intButtonXPlusRemainder;
      buttonBorder[i][13] = intButtonXPlusDivision;
    }
    // The Y or vertical coordinate is calculated identical to the X.
    if (intButtonY < 256) {
      buttonBorder[i][14] = intButtonY;
      buttonBorder[i][15] = 0;
    }
    else {
      intButtonYRemainder = intButtonY % 256;
      intButtonYDivision = intButtonY / 256;
      buttonBorder[i][14] = intButtonYRemainder;
      buttonBorder[i][15] = intButtonYDivision;
    }
    //printf("low\nx buttonBorder[%d][12]: %d buttonBorder[%d][13]: %d\n", i, buttonBorder[i][12], i, buttonBorder[i][13]);
    //printf("y buttonBorder[%d][14]: %d buttonBorder[%d][15]: %d\n\n", i, buttonBorder[i][14], i, buttonBorder[i][15]);
    // Calculates the X limit border for the '20' buttons using the first width 'intButtonWidth' or 'buttonBorder[1][16]'.
    // Mouse click input is defined in the while (1) loop and formatted
    // as a 2 byte overflow since 'read()' uses 'char[]'.
      // X [24] and [25]
      // Y [26] and [27]
    // if '[12] + width' is > 255, add overflow to '[13]'.
      // useful if you resize the initial window.
    // The logic is similar to the 2 byte for one int from above for 'buttonBorder[i][12]-[15]'.
    if ((buttonBorder[i][12] + intButtonWidth) > 256) {
      buttonBorderXDivision = (buttonBorder[i][12] + intButtonWidth) / 256;
      buttonBorderXRemainder = (buttonBorder[i][12] + intButtonWidth) % 256;
      buttonBorderX12 = buttonBorderXRemainder;
      buttonBorderX13 = buttonBorder[i][13] + buttonBorderXDivision;
    }
    else {
      buttonBorderX12 = buttonBorder[i][12] + intButtonWidth;
      buttonBorderX13 = buttonBorder[i][13];
      //printf("buttonBorderX12 %d = buttonBorder[%d][12] %d + intButtonWidth %d;\n", buttonBorderX12, i, buttonBorder[i][12], intButtonWidth);
    }
    buttonBorderXXYY[k] = buttonBorderX12;
    buttonBorderXXYY[k + 1] = buttonBorderX13;
    //printf("high\nx buttonBorderXXYY[%d]: %d buttonBorderXXYY[%d+1]: %d\n", k, buttonBorderXXYY[k], k, buttonBorderXXYY[k+1]);
    // Calculates the Y limit border for the '20' buttons using the dimensions plus the first height 'intButtonHeight' or 'buttonBorder[1][16]'.
    if ((buttonBorder[i][14] + intButtonHeight) > 256) {
      buttonBorderYDivision = (buttonBorder[i][14] + intButtonHeight) / 256;
      buttonBorderYRemainder = (buttonBorder[i][14] + intButtonHeight) % 256;
      buttonBorderY14 = buttonBorderYRemainder;
      buttonBorderY15 = buttonBorder[i][15] + buttonBorderYDivision;
    }
    else {
      buttonBorderY14 = buttonBorder[i][14] + intButtonHeight;
      buttonBorderY15 = buttonBorder[i][15];
    }
    buttonBorderXXYY[k + 2] = buttonBorderY14;
    buttonBorderXXYY[k + 3] = buttonBorderY15;
    //printf("y buttonBorderXXYY[%d+2]: %d buttonBorderXXYY[%d+3]: %d\n\n", k, buttonBorderXXYY[k+2], k, buttonBorderXXYY[k+3]);
    // Assign default values from 0-19 excluding 12-15 since those were previously calculated.
    buttonBorder[i][0] = 67;
    buttonBorder[i][1] = 0;
    buttonBorder[i][2] = 0b00000101;       // '5' Request length ( n / 4)
    buttonBorder[i][3] = 0;
    buttonBorder[i][4] = windowID.one;
    buttonBorder[i][5] = windowID.two;
    buttonBorder[i][6] = windowID.three;
    buttonBorder[i][7] = windowID.four;
    buttonBorder[i][8] = gcID.one;
    buttonBorder[i][9] = gcID.two;
    buttonBorder[i][10] = gcID.three;
    buttonBorder[i][11] = gcID.four;
    //              12
    //              13
    //              14
    //              15
    buttonBorder[i][16] = intButtonWidth;
    buttonBorder[i][17] = 0;
    buttonBorder[i][18] = intButtonHeight;
    buttonBorder[i][19] = 0;

    intButtonXPlus = intButtonXPlus + buttonXSpace;
    if ((i % buttonColumns == 0) && (i != 0)) {
      //printf("\n\n");
      intButtonXPlus = intButtonX;
      intButtonY = intButtonY + buttonYSpace;
    }
    //printf("i: %d k: %d\n", i, k);
    //printf("i %d  intButtonXPlus %d  intButtonY %d\n", i, intButtonXPlus, intButtonY);
    i++;
  }

  // 5) Opcode 70 'polyFillRectangle'
  unsigned char fillRectangle[20];
  fillRectangle[0] = 70;             // polyFillRectangle
  fillRectangle[1] = 0;
  fillRectangle[2] = 0b00000101;     // '5' Request length ( n / 4)
  fillRectangle[3] = 0;
  // Target Window ID
  fillRectangle[4] = windowID.one;
  fillRectangle[5] = windowID.two;
  fillRectangle[6] = windowID.three;
  fillRectangle[7] = windowID.four;
  fillRectangle[8] = gcID.one;
  fillRectangle[9] = gcID.two;
  fillRectangle[10] = gcID.three;
  fillRectangle[11] = gcID.four;
  // Geometry data
  // x
  fillRectangle[12] = intButtonX;
  fillRectangle[13] = 0;
  // y
  fillRectangle[14] = intButtonY;
  fillRectangle[15] = 0;
  // Width
  fillRectangle[16] = intButtonWidth;
  fillRectangle[17] = 0;
  // Height
  fillRectangle[18] = intButtonHeight;
  fillRectangle[19] = 0;
  /*
  // x
  fillRectangle[12] = 1;
  fillRectangle[13] = 0;
  // y
  fillRectangle[14] = 1;
  fillRectangle[15] = 0;
  // Width 128 + (0 * 256) = 128;
  //fillRectangle[16] = 0b10000000;
  fillRectangle[16] = 10;
  fillRectangle[17] = 0;
  // Height 128 + (0 * 256) = 128;
  //fillRectangle[18] = 0b10000000;
  fillRectangle[18] = 10;
  fillRectangle[19] = 0;
  */

  // 5) Retains keyboard input from the window.
  unsigned char setInputFocus[12];
  setInputFocus[0] = 42;              // Opcode 42
  setInputFocus[1] = 0b00000001;      // Focus to new window.
  setInputFocus[2] = 0b00000011;      // Request length low byte: '3'
  setInputFocus[3] = 0;               // Request length high byte
  setInputFocus[4] = windowID.one;    // New window ID
  setInputFocus[5] = windowID.two;    //
  setInputFocus[6] = windowID.three;  //
  setInputFocus[7] = windowID.four;   //
  setInputFocus[8] = 0;               // Current time.
  setInputFocus[9] = 0;               //
  setInputFocus[10] = 0;              //
  setInputFocus[11] = 0;              //

  // 4) 'write()' the 'createWindow' socket request.
  int sock = 3;
  int windowWrite = write(sock, createWindowBuffer, sizeof(createWindowBuffer));

  // 4) 'write()' the 'changeWindowAttributes'.
  int changeWindowWrite = write(sock, changeWindowAttributes, sizeof(changeWindowAttributes));

  // suggested to combine the two opcode 2 but didnt work
  // 4) 'write()' 'changeWindowAttributes' to disable window manager with 'CWOverrideRedirect'.
  //int overrideWrite = write(sock, overrideRedirect, sizeof(overrideRedirect));

  // 4) 'write()' the 'getGeometry' socket request to avoid hanging program.
  int geometryWrite = write(sock, getGeometry, sizeof(getGeometry));

  unsigned char geometryRead[32];
  int intGeometryRead = read(sock, geometryRead, sizeof(geometryRead));
  printf("intGeometryRead: %d  geometryRead[0]: %d\n", intGeometryRead, geometryRead[0]);

  // 4) 55 'createGC' - change the rectangle color.
  int createGCWrite = write(sock, createGC, sizeof(createGC));

  // The success packet "'geometryRead[0]': '1'" is sent back after the 'geometryWrite' instead
  // of the 'mapWindowBuffer'. 20260824.
  //int intGeometryRead = read(sock, geometryRead, sizeof(geometryRead));
  //printf("intGeometryRead: %d  geometryRead[0]: %d\n", intGeometryRead, geometryRead[0]);

  // 5) 'write()' the 'mapWindowBuffer' socket request to view the window.
  int intMapWindow = write(sock, mapWindowBuffer, sizeof(mapWindowBuffer)); // 8

  //////////////
  // 5) 'write()' the 'setInputFocus' to retain keyboard input when the window
  // window manager is disabled.
  //int inputFocus = write(sock, setInputFocus, sizeof(setInputFocus));
  //unsigned char setInputFocusRead[32];
  //int intSetInputFocusRead = read(sock, setInputFocusRead, sizeof(setInputFocusRead));

  ////////////////////////////////
  // This avoids redrawing the buttons everytime. The internet specifically said
  // not to use any iterations in the mouse and keyboard input stream - the 'while'
  // loop below 'intMapRead'.
  // Other windows clear the rectangles when dragged over, either write an OS and call this whenever the
  // calculator window is blocked or insert in a loop and call at various intervals.
  unsigned char mapRead[32];
  //int intMapRead; // = read(sock, mapRead, sizeof(mapRead));
  // This was in a while (1) loop before, there might be cases when the second
  // 'read()' [0] is not '12' but the third one is.
  int intMapRead = read(sock, mapRead, sizeof(mapRead));
  //numberButtons = 21
  //buttonBorder[i][j]
  int borderRectangleWrite;
  unsigned char subsetButtonBorder[buttonElements];
  int j = 0; // moved from the previous 'while'
  i = 1;
  if (mapRead[0] == 12) {
    printf("mapRead[0] == 12 drawing rectangles\n");
    // 6) 67 'borderRectangle' - rectangle outline.
    while (i < numberButtons) {
      j = 0;
      while (j < sizeof(subsetButtonBorder)) {
        subsetButtonBorder[j] = buttonBorder[i][j];
        //printf("subsetButtonBorder[%d][%d]:  %d\n", i, j, subsetButtonBorder[j]);
        j++;
      }
      //printf("\n");
      borderRectangleWrite = write(sock, subsetButtonBorder, sizeof(subsetButtonBorder));
      i++;
    }
    // 5) 70 'write()' the 'polyFillRectangle' to draw a button without a border.
    //int fillRectangleWrite = write(sock, fillRectangle, sizeof(fillRectangle));
  }

  // Step 6) Continuous loop requesting user input with 'read()'.
  // The first char[] used in: read(sock, responseWindowInput, 32);
  // every loop iteration to return input.
  unsigned char responseWindowInput[32];
  // Holds the size of the previous char[].
  int changeWindowInput;
  int eventCode;
  int keyInput;
  int button;
  int mouseX;
  int mouseY;
  int error;

  int x2LowCheck;
  int x1LowCheck;
  int y2LowCheck;
  int y1LowCheck;
  int x2HighCheck;
  int x1HighCheck;
  int y2HighCheck;
  int y1HighCheck;

  if (geometryRead[0] == 1) {
    printf("Window created via raw sockets! Keep process alive to view.\n");
    while (1) {
      // responseWindowInput[0]; to route the intitial if/else.
      responseWindowInput[32];
      changeWindowInput = read(sock, responseWindowInput, 32);
      eventCode = responseWindowInput[0];
      //printf("eventCode: %d\n", eventCode);
      if (eventCode == 0) {
        // Prints the error packet.
        error = 0;
        while (error < 32) {
          printf("responseWindowInput[%d]: %d\n", error, responseWindowInput[error]);
          error++;
        }
      }
      // The 'keyInput' indicates the keyboard has been pressed.
      else if (eventCode == 2) {
        // Physical key matrix index
        keyInput = responseWindowInput[1] + 39;
        printf("keypress = %d\n", keyInput);
        // int keyboard map.
        if (keyInput > 47 && keyInput < 59) {
          // The first '48' 'esc' doesn't conflict with the second '48' '0' since
          // the 'while' loop asks for input and overwrites 'keyInput'. Typing '0'
          // doesn't escape the program.
          if (keyInput == 48) {
            printf("esc %d\n", keyInput);
            break;
          }
          else if (keyInput == 49) {
            printf("a = 1 %d\n", keyInput);
          }
          else if (keyInput == 50) {
            printf("a = 2 %d\n", keyInput);
          }
          else if (keyInput == 51) {
            printf("a = 3 %d\n", keyInput);
          }
          else if (keyInput == 52) {
            printf("a = 4 %d\n", keyInput);
          }
          else if (keyInput == 53) {
            printf("a = 5 %d\n", keyInput);
          }
          else if (keyInput == 54) {
            printf("a = 6 %d\n", keyInput);
          }
          else if (keyInput == 55) {
            printf("a = 7 %d\n", keyInput);
          }
          else if (keyInput == 56) {
            printf("a = 8 %d\n", keyInput);
          }
          else if (keyInput == 57) {
            printf("a = 9 %d\n", keyInput);
          }
          else if (keyInput == 58) {
            keyInput = keyInput - 10;
            printf("a = 0 %d\n", keyInput);
          }
        }
      }
      else if (eventCode == 4) {
        // ButtonPress (Mouse click) occurred.
        // Right handed: 1 = Left Click, 2 = Middle, 3 = Right
        button = responseWindowInput[1];
        // '0' button mouse click.
        //printf("button = %d\n", button);
        //printf("mouseX = %d\n", mouseX);
        //printf("mouseY = %d\n", mouseY);

        // Drawing window from the top left (or right).
        //printf("    X1 responseWindowInput[24] = %d\n", responseWindowInput[24]);
        //printf("    X2 responseWindowInput[25] = %d\n", responseWindowInput[25]);
        //printf("    Y1 responseWindowInput[26] = %d\n", responseWindowInput[26]);
        //printf("    Y2 responseWindowInput[27] = %d\n", responseWindowInput[27]);

        // The below 'while' loop calculates the coordinate borders for '20' buttons.
        // Tested on new char 4 columns of * 5 rows = '20'.
          // [12] and [13] have overflow accounted for x
          // [14] and [15] have overflow accounted for y
            // if '[12] + width' is > 255, add overflow to '[13]'.
        // Method is more complicated but useful if you resize the initial window and button sizes.
          // The variables to resize are in the '67' packet 'while' loop above. Examples:
            //printf("width: 50  buttonBorder[1][16]: %d\n", buttonBorder[1][16]);
            //printf("height: 50  buttonBorder[1][18]: %d\n", buttonBorder[1][18]);

        // 2 binary divisions are 0(nlogn) for worst case which is more than comparions ('>=', '>', etc) 0(logn)
          // Newton-Raphson is another binary division method that seems to work for containerized numbers
          // (i.e. below a small threshold 'n' like 255) but doesn't scale well (i.e. multiplication tables
          // with any of the various search algorithms). Internet implied O(nlogn).
        // The mouse clicks 'responseWindowInput[24]-[27]' are compared with 'buttonBorder[j][12]-[15]' for
        // the low and high x and y boundary coordinates to return 8 checks. If all the checks are '1' the button
        // number within the x and y boundaries are returned. Another method uses bit shifting to convert which
        // simplifies the comparison step.
          // Extract mouse X and Y coordinates (Bytes 24-27) with bit shifting.
          //mouseX = responseWindowInput[24] | (responseWindowInput[25] << 8);
          //mouseY = responseWindowInput[26] | (responseWindowInput[27] << 8);
        // A more detailed explanation exists for the first 'if' statement.
          // hypothetical not tested until the window and button xy coordinates are expanded to the screen widths

        // todo is the calculator template:
        // a  a  a  /
        // 7  8  9  X
        // 4  5  6  -
        // 1  2  3  +
        // (  )  0  =

        // 'j' starts at '1' and is the same as the first packet loop used in the 'write()' step.
        j = 1;
        k = 0;
        // The check values for the first button.
        x1LowCheck = 0;
        y1LowCheck = 0;
        x1HighCheck = 0;
        y1HighCheck = 0;
        x2LowCheck = 0;
        y2LowCheck = 0;
        x2HighCheck = 0;
        y2HighCheck = 0;
        while (j < numberButtons) {
          // The check values are reset to '0' each iteration.
          x1LowCheck = 0;
          y1LowCheck = 0;
          x1HighCheck = 0;
          y1HighCheck = 0;
          x2LowCheck = 0;
          y2LowCheck = 0;
          x2HighCheck = 0;
          y2HighCheck = 0;
          // 'k' is used in the high limit for x and y coordinates i.e. 'buttonBorderXXYY[k]', 'buttonBorderXXYY[k+1]', etc.
          k = (j - 1) * buttonColumns;
          //printf("j: %d  k: %d\n", j,k);
          // char limits int to 255 and this method overflows into two char elements which uses multiplication to achieve
          // larger numbers (i.e. the mouse click input responseWindowInput[24] and [25]). To find the bottom border for the
          // button, the larger input '[25]' is compared using greater than or equal '>=' to the previously calculated array
          // 'buttonBorder[j][13]' that contains the larger bottom 'x' coordinate border. The larger number is the second
          // element in the two char array elements used to represent larger int numbers and are refered to as 'x2' or 'y2' in
          // contrast with the first element 'x1' or 'y1'. The equation is ((x1 * 256^0) + (x2 * 256^1)...etc). If the 'x2' input
          // (responseWindowInput[25]) is '>=' to the comparison (buttonBorder[j][13]), 'x2LowCheck' passes and proceeds to the
          // next check for 'x1LowCheck'.
          // x2 low
          if (responseWindowInput[25] >= buttonBorder[j][13]) {
            x2LowCheck = 1;
            // The next comparison splits based on if the larger 'x2' number is '==' or '>'. The first 'if' has another
            // condition for the 'x1' value which asks if the mouse click input 'responseWindowInput[24]' is '>=' the
            // 'buttonBorder[j][12]' 'x1' border. If both those conditions are true, 'x1Lowcheck' is set to '1'. If they're
            // not, the second split asks if only the 'x2' is '>' 'buttonBorder[j][13]' and sets 'x1LowCheck' to '1'. Otherwise
            // 'x1LowCheck' remains '0' and a 'break;' would probably work to reduce further iterations.
            if (responseWindowInput[25] == buttonBorder[j][13] && responseWindowInput[24] >= buttonBorder[j][12]) {
              x1LowCheck = 1;
            }
            else if (responseWindowInput[25] > buttonBorder[j][13]) {
              x1LowCheck = 1;
            }
            else {
              ;
            }
          }
          // 'y2' low is identical to the above explanation.
          if (responseWindowInput[27] >= buttonBorder[j][15]) {
            y2LowCheck = 1;
            if (responseWindowInput[27] == buttonBorder[j][15] && responseWindowInput[26] >= buttonBorder[j][14]) {
              y1LowCheck = 1;
            }
            else if (responseWindowInput[27] > buttonBorder[j][15]) {
              y1LowCheck = 1;
            }
            else {
              ;
            }
          }
          // 'x2' high border reverses the greater than '>' comparisions to less than '<' and follows the same logic.
          if (responseWindowInput[25] <= buttonBorderXXYY[k+1]) {
            x2HighCheck = 1;
            if (responseWindowInput[25] == buttonBorderXXYY[k+1] && responseWindowInput[24] <= buttonBorderXXYY[k]) {
              x1HighCheck = 1;
            }
            else if (responseWindowInput[25] < buttonBorderXXYY[k+1]) {
              x1HighCheck = 1;
            }
            else {
              ;
            }
          }
          // y2 high
          if (responseWindowInput[27] <= buttonBorderXXYY[k+3]) {
            y2HighCheck = 1;
            if (responseWindowInput[27] == buttonBorderXXYY[k+3] && responseWindowInput[26] <= buttonBorderXXYY[k+2]) {
              y1HighCheck = 1;
            }
            else if (responseWindowInput[27] < buttonBorderXXYY[k+3]) {
              y1HighCheck = 1;
            }
            else {
              ;
            }
          }
          //printf("x2LowCheck = %d  y2LowCheck = %d  x2HighCheck = %d  y2HighCheck = %d  ", x2LowCheck, y2LowCheck, x2HighCheck, y2HighCheck);
          //printf("x1LowCheck = %d  y1LowCheck = %d  x1HighCheck = %d  y1HighCheck = %d\n", x1LowCheck, y1LowCheck, x1HighCheck, y1HighCheck);
          // If all the checks are true, the appropriate button is used for input.
          if (x2LowCheck == 1) {
            if (y2LowCheck == 1) {
              if (x2HighCheck == 1) {
                if (y2HighCheck == 1) {
                  if (x1LowCheck == 1) {
                    if (y1LowCheck == 1) {
                      if (x1HighCheck == 1) {
                        if (y1HighCheck == 1) {
                          printf("button '  %d  '\n", j);
                          break;
                        }
                      }
                    }
                  }
                }
              }
            }
          }

          /*
          // The previous attempt had too many confusing variable names to debug but inspired time complexity investigation:
          //printf("buttonBorder[%d] buttonBorderXXYY[%d]\n", j, k);
          ///////       // might be  '>='
          if (responseWindowInput[25] == buttonBorder[j][13]) {             // x2 0    = 0
            if (responseWindowInput[24] >= buttonBorder[j][12]) {            // x1 255  > 220
              // Tests the x2 against the high boundary.
                            // might be  '=='
              if (responseWindowInput[25] <= buttonBorderXXYY[k+1]) {         // x2 0    >= 1  // no
                if (responseWindowInput[24] < buttonBorderXXYY[k]) {        // x1 255  >
                  // x works because x2 input is the same as the low and high x2 boundary (0 and 3 if)
                  //                 x1 input is greater than low and less than high x1 boundary
                  //printf("1st if, 1st nested: x within boundary\n");
                  ///////////////////////////
                  // test for y low
                  if (responseWindowInput[27] == buttonBorder[j][15]) {      // y2 1   = 1
                    if (responseWindowInput[26] > buttonBorder[j][14]) {     // y1 109 > 84
                    //printf("y within boundary\n");
                      if (responseWindowInput[27] == buttonBorderXXYY[k+3]) {  // y2 1   = 1
                        if (responseWindowInput[26] < buttonBorderXXYY[k+2]) { // y1 109 < 134
                          printf("x2 when [25] and [k+1] are =. y2 within boundary when y2 is equal\n");
                          printf("button '  %d  '\n", j);
                          break;
                        }
                      }
                      // probably dont need this one
                      //if (responseWindowInput[27] < buttonBorderXXYY[k+3]) {   // y2 0   < 1
                      //  if (responseWindowInput[26] < buttonBorderXXYY[k+2]) { // y1 109 < 134
                      //    printf("y within boundary - 1st if, 2nd nested, 2nd nested\n");
                      //  }
                      //}
                    }
                  }
                  // hypothetical not tested until the window and button xy coordinates are expanded to the screen widths
                  else if (responseWindowInput[27] < buttonBorder[j][15]) {  // y2 0   < 1
                    if (responseWindowInput[26] > buttonBorder[j][14]) {     // y1 109 > 84
                      // test for y high
                      if (responseWindowInput[27] == buttonBorderXXYY[k+3]) {  // y2 0   = 1    // no
                        if (responseWindowInput[26] < buttonBorderXXYY[k+2]) {
                          //printf("y2 within boundary when y2 is greater than low\n");
                        }
                      }
                      //
                      if (responseWindowInput[27] < buttonBorderXXYY[k+3]) {   // y2 0   < 1
                        if (responseWindowInput[26] < buttonBorderXXYY[k+2]) { // y1 109 < 134
                          //printf("y2 within boundary when y2 greater than low and less than high\n");
                        }
                      }
                    }
                  }
                  //////////////////////////////
                }
              }
              else if (responseWindowInput[25] < buttonBorderXXYY[k+1]) {      // x2 0   < 1   // yes
                // input: x responseWindowInput[24] 225  responseWindowInput[25] 0
                if (responseWindowInput[24] > buttonBorderXXYY[k]) {         // x1 255 > 14  // or?
                  // x works because x2 input is the same as the low and high x2 boundary (0 and 3 if)
                  //                 x1 input is greater than low and less than high x1 boundary
                  //printf("1st if, 2nd nested: x within boundary\n");
                  ///////////////////////////////// todo - copy into the other two x coordinate blocks
                  // The y low
                  if (responseWindowInput[27] == buttonBorder[19][15]) {      // y2 1   = 1
                    if (responseWindowInput[26] > buttonBorder[19][14]) {     // y1 109 > 84
                      //printf("y within boundary - 1st if, 2nd nested\n");
                      // test for y high
                      if (responseWindowInput[27] == buttonBorderXXYY[k+3]) {  // y2 1   = 1
                        if (responseWindowInput[26] < buttonBorderXXYY[k+2]) { // y1 109 < 134
                          //printf("x2 when [25] < [k+1]. y2 within boundary when y2 is equal\n");
                          printf("button '  %d  '\n", j);
                          break;
                        }
                      }
                      // probably dont need this one
                      //if (responseWindowInput[27] < buttonBorderXXYY[k+3]) {   // y2 0   < 1
                      //  if (responseWindowInput[26] < buttonBorderXXYY[k+2]) { // y1 109 < 134
                      //    printf("y within boundary - 1st if, 2nd nested, 2nd nested\n");
                      //  }
                      //}
                    }
                  }
                  // hypothetical not tested until the window and button xy coordinates are expanded to the screen widths
                  else if (responseWindowInput[27] < buttonBorder[j][15]) {  // y2 0   < 1
                    if (responseWindowInput[26] > buttonBorder[j][14]) {     // y1 109 > 84
                      // test for y high
                      if (responseWindowInput[27] == buttonBorderXXYY[k+3]) {  // y2 0   = 1    // no
                        if (responseWindowInput[26] < buttonBorderXXYY[k+2]) {
                          //printf("y2 within boundary when y2 is greater than low\n");
                        }
                      }
                      //
                      if (responseWindowInput[27] < buttonBorderXXYY[k+3]) {   // y2 0   < 1
                        if (responseWindowInput[26] < buttonBorderXXYY[k+2]) { // y1 109 < 134
                          //printf("y2 within boundary when y2 greater than low and less than high\n");
                        }
                      }
                    }
                  }
                  /////////////////////////////////////
                }
              }
            }
          }
          // x2 greater than the low
          else if (responseWindowInput[25] > buttonBorder[j][13]) {         // x2 1 >  0
            // dont have to test x1 against x1 low
            // x2 less than high x2
            if (responseWindowInput[25] <= buttonBorderXXYY[k+1]) {           // x2 1 <= 1
              // x1 less than high x1
              if (responseWindowInput[24] < buttonBorderXXYY[k]) {          // x1 13 <  14
                //printf("2nd if: x within boundary\n");
                ///////////////////////////////////
                // test for y low
                if (responseWindowInput[27] == buttonBorder[j][15]) {       // y2 1   = 1
                  if (responseWindowInput[26] > buttonBorder[j][14]) {      // y1 133 > 84
                    //printf("y within boundary\n");
                    if (responseWindowInput[27] == buttonBorderXXYY[k+3]) {   // y2 1   = 1
                      if (responseWindowInput[26] < buttonBorderXXYY[k+2]) {  // y1 133 < 134
                        //printf("x2 when [25] and [k+1] are =. y2 within boundary when y2 is equal\n");
                        printf("button '  %d  '\n", j);
                        break;
                      }
                    }
                    // probably dont need this one
                    //if (responseWindowInput[27] < buttonBorderXXYY[k+3]) {   // y2 0   < 1
                    //  if (responseWindowInput[26] < buttonBorderXXYY[k+2]) { // y1 133 < 134
                    //    printf("y within boundary - 1st if, 2nd nested, 2nd nested\n")
                    //  }
                    //}
                  }
                }
                // hypothetical not tested until the window and button xy coordinates are expanded to the screen widths
                else if (responseWindowInput[27] < buttonBorder[j][15]) {  // y2 0   < 1
                  if (responseWindowInput[26] > buttonBorder[j][14]) {     // y1 133 > 84
                  // test for y high
                    if (responseWindowInput[27] == buttonBorderXXYY[k+3]) {  // y2 0   = 1    // no
                      if (responseWindowInput[26] < buttonBorderXXYY[k+2]) {
                        //printf("y2 within boundary when y2 is greater than low\n");
                      }
                    }
                    //
                    if (responseWindowInput[27] < buttonBorderXXYY[k+3]) {   // y2 0   < 1
                      if (responseWindowInput[26] < buttonBorderXXYY[k+2]) { // y1 133 < 134
                        //printf("y2 within boundary when y2 greater than low and less than high\n");
                      }
                    }
                  }
                }
                //////////////////////////////////////////////////
              }
            }
          }
          */
          //printf("%d = (%d - 1) * %d\n", k, j, buttonColumns);

          j++;
        }     // 'while' end bracket for the XY coordinate check

/*
// these were the coordinates that worked for the 19th button or '0'
input: y responseWindowInput[26] 109  responseWindowInput[27] 1
low
x buttonBorder[19][12]: 220 buttonBorder[19][13]: 0
y buttonBorder[19][14]: 84 buttonBorder[19][15]: 1
high
x buttonBorderXXYY[72]: 14 buttonBorderXXYY[72+1]: 1
y buttonBorderXXYY[72+2]: 134 buttonBorderXXYY[72+3]: 1
input: x responseWindowInput[24] 255  responseWindowInput[25] 0
input: y responseWindowInput[26] 107  responseWindowInput[27] 1
///////////////// hypothetical not tested until the window and button xy coordinates are expanded to the screen widths
*/




        ///////////////////////////////////////
        // the first attempt:
        // '=' button mouse click.
        //printf("input: responseWindowInput[24] %d  responseWindowInput[25] %d\n", responseWindowInput[24], responseWindowInput[25]);
        //printf("buttonBorder2012: %d\nbuttonBorder2013: %d\n", buttonBorderX2012, buttonBorderX2013);
        //printf("buttonBorderXXYY[76]: %d\nbuttonBorderXXYY[77]: %d\n", buttonBorderXXYY[76], buttonBorderXXYY[77]);
        // x low is default [24] and [25]
        /*
        if ((responseWindowInput[24] > buttonBorder[20][12]) && (responseWindowInput[25] == buttonBorder[20][13])) {
          // x high is the converted Division and Remainder
          if ((responseWindowInput[24] < buttonBorderXXYY[76]) && (responseWindowInput[25] == buttonBorderXXYY[77])) {
            //printf("x '  =  ' works\n");
            // y low is default [26] and [27]
            if ((responseWindowInput[26] > buttonBorder[20][14]) && (responseWindowInput[27] == buttonBorder[20][15])) {
              // y high is the converted Division and Remainder
              if ((responseWindowInput[26] < buttonBorderXXYY[78]) && (responseWindowInput[27] == buttonBorderXXYY[79])) {
                printf("button '  =  '\n");
              }
            }
          }
        }
        */
        //////////////////////////////


        // event code 4 else close
      }
      // while close
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
    // printf("ReadSock %d\n", header[0]);
    // screenOffset + 8
    int screenOffset = getScreenOffset(readSock);
    struct fourInt windowID = getWindowID(readSock);

    struct fourInt gcID = getGCID(readSock, windowID);

    struct fourInt parentID = getParentWindowID(readSock, screenOffset);
    struct fourInt visualID = getVisualID(readSock, screenOffset);
    if (screenOffset != 0) {
      drawWindow(windowID, gcID, parentID, visualID);
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
