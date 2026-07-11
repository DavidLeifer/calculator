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

// 0) Extract MIT-MAGIC-COOKIE-1 from ~/.Xauthority
// Input is the user's path 'userPath'. Returns the path to ".Xauthority".
char *xAuthority(char *userPath) {
  int userPathLength = charLength(userPath);
  char *charAuthority = malloc(12 * sizeof(char));
  strcpy(charAuthority, ".Xauthority");
  struct threeInt charAuthorityPath = charAppend(userPath, userPathLength, charAuthority, 12, 0);
  free(userPath);
  free(charAuthority);
  // printf("%s\n", charAuthorityPath.string);
  return charAuthorityPath.string;
}

// 0) Opens '~/.Xauthority' using 'charAuthorityPath[]' and returns 'xAuthorityRequest' struct.
struct xAuthorityRequest openAuthority(char *charAuthorityPath) {
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
  // Returning the requested '.Xauthority' data into 'struct xAuthorityRequest'.
  struct xAuthorityRequest req;
  req.byteOrder = 'l'; // 'B' is the alternative.
  req.padOne = 0;
  req.majorVersion = 11;
  req.minorVersion = 0;
  req.authNameLen = 18;
  req.authDataLen = 16;
  req.padTwo = 0;
  int i = 0;
  int j = 0;
  // Name with padding of three extra zeros.
  while (i < 21) {
    if (i < 18) {
      req.authName[i] = name[i];
    }
    else {
      req.authName[i] = '0';
    }
    // Also returns the 'cookie' data into an int array.
    if (i < 16) {
      req.authData[j] = cookie[j];
      j++;
    }
    i++;
  }
  // free() the char[].
  free(family);
  free(address);
  free(name);
  free(cookie);
  /*
  int i = 0;
  while (cookie[i] != '\0') {
    // ascii chart
    // //Prints the character when the operands are switched.
    // Prints the binary.
    //if (buffer[i] <= 32 && buffer[i] >= 126) {
      //printf("%d  %c\n", i, buffer[i]);
      //printf("%d  %02x\n", i, buffer[i]);
    printf("%d  %02x\n", i, cookie[i]);
    //}
    i++;
  }*/
  return req;
}

// 'window()' is called after steps 0-3 in 'serverConnect()' and input is the char[]
// contining window and root information from the socket connection's 'read()'.
// 4) Uses the window and root ID to send a packet containing window
//    drawing information to 'X11'.
// 5) Construct and send the MapWindow packet (Opcode 8).
void window(char *replyHeader) {
  int i = 0;
  while (i <= 9) {
    printf("%d window heap: %d\n", i, replyHeader[i]);
    i++;
  }
  free(replyHeader);

  /*
  one method
    // ----------------------------------------------------
    // [PREVIOUS CONTEXT]: Assume socket 'sock' is active
    // and you just read 'reply_status == 1'.
    // ----------------------------------------------------
    int sock = 3; // Placeholder for your active socket descriptor
    uint16_t extra_len = 0; // Read this from the 8-byte response header

    // 1. READ THE REST OF THE SUCCESSFUL CONNECTION SELECTION PAYLOAD
    // 'extra_len' is specified in 4-byte units.
    size_t reply_payload_size = extra_len * 4;
    uint8_t *reply_payload = malloc(reply_payload_size);

    size_t total_read = 0;
    while (total_read < reply_payload_size) {


        // using reply_payload for resource_id_base
        ssize_t n = read(sock, reply_payload + total_read, reply_payload_size - total_read);




        if (n <= 0) {
            perror("Failed reading server setup payload");
            free(reply_payload);
            return 1;
        }
        total_read += n;
    }



        // using reply_payload for resource_id_base

  // a different method.

  // Parse necessary identifiers directly out of the reply buffer
  unsigned int resource_id_base = *(unsigned int*)&replySock.string[12];
  unsigned short vendor_len     = *(unsigned short*)&replySock.string[16];
  //int vendor_len = reply[16];
  unsigned short num_formats    = *(unsigned char*)&replySock.string[29];
  // Compute the accurate memory offset for the screen information
  int screen_offset = 40 + ((vendor_len + 3) & ~3) + (num_formats * 8);
  unsigned int root_window_id = *(unsigned int*)&replySock.string[screen_offset];

  // Create a new unique Window ID using the provided resource base
  unsigned int client_window_id = resource_id_base + 1;

  // Construct and send the CreateWindow packet (Opcode 1)
  unsigned int create_window_buf[12];
  create_window_buf[0] = (8 << 16) | 1;      // Request length (8 blocks of 4 bytes) | Opcode 1
  create_window_buf[1] = client_window_id;   // New Window ID
  create_window_buf[2] = root_window_id;     // Parent Window ID
  create_window_buf[3] = (100 << 16) | 100;  // X position (100) | Y position (100)
  create_window_buf[4] = (400 << 16) | 600;  // Width (600px) | Height (400px)
  create_window_buf[5] = (0 << 16) | 1;      // CopyFromParent Window Class | Border Width
  create_window_buf[6] = 0;                  // CopyFromParent Visual ID
  create_window_buf[7] = 0x00000002;         // Value mask flag (setting background pixel)
  create_window_buf[8] = 0x00000001;         // Background pixel value (0 = Black)

  //int ii = 0;
  //while (ii < 13) {
  //  printf("%d  %d\n", ii, create_window_buf[ii]);
  //  ii++;
  //}

  write(replySock.one, create_window_buf, 36);

  // 5) Construct and send the MapWindow packet (Opcode 8)
  unsigned int map_window_buf[2];
  map_window_buf[0] = (2 << 16) | 8;        // Request length (2 blocks) | Opcode 8
  map_window_buf[1] = client_window_id;     // ID of the target window to draw
  write(replySock.one, map_window_buf, 8);

  printf("a  %d\n", map_window_buf[0]);

  //printf("Window created via raw sockets! Keep process alive to view.\n");

  // 6. Keep event loop active to maintain socket connection
  while (1) {
    char dummy[256];
    if (read(replySock.one, dummy, sizeof(dummy)) <= 0) {
    //  int iii = 0;
    //  while (iii < 257) {
    //    printf("%d  %c\n", iii, dummy[iii]);
    //    iii++;
    //  }
      break;
    }
  }
  //close(sock); // closed in serverConnect();
  */
}

int serverConnect(char userPath[]) {
  // 0. Format and send X11 Connection Setup request
  // 'userPath' is free() in xAuthority.
  char *charAuthorityPath = xAuthority(userPath);
  // Open the file and get MIT-MAGIC-COOKIE-1 from user's '~/.Xauthority'.
  struct xAuthorityRequest req = openAuthority(charAuthorityPath);
  free(charAuthorityPath);

  // 1. Establish connection to local X server socket
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  //printf("socket int returns: %d\n", sock);
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
  int intWrite = write(sock, &req, sizeof(req));
  //printf("write() int returns: %d\n", intWrite);
  unsigned char responseHeader[8];
  int intRead = read(sock, responseHeader, 8);
  //printf("read() int returns: %d\n", intRead);
  printf("response_header[] returns: %d\n", responseHeader[8]);

  // Heap the response char[] and free() in window().
  char *readSock = malloc(9 * sizeof(char));
  strcpy(readSock, responseHeader);
  int i = 0;
  while (i <= 9) {
    //printf("%d not heap: %d \n", i, responseHeader[i]);
    // Null terminate the char[].
    if (i == 9) {
      readSock[i] = '\0';
    }
    else if (responseHeader[i] == 0) {
      // The null terminators are changed to '1'.
      readSock[i] = '1';
    }
    else {
      readSock[i] = responseHeader[i];
    }
    i++;
  }

  // If the server returns 1, connection was successfull and send heap pointer to 'window()'.
  if (responseHeader[8] == 1) {
    window(readSock);
  }
  else {
    // Otherwise print an error message.
    printf("X11 server connection failed, check read(), write(), connect()\n.");
  }
  close(sock);
  return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////




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
