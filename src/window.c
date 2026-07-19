#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "struct.h"
#include "utility.h"

// 0) Extract MIT-MAGIC-COOKIE-1 from ~/.Xauthority
// Input is the user's path 'userPath'. Returns the path to ".Xauthority".
char* xAuthority(char *userPath) {
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
//    in 'window()' through 'serverConnect()'.
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

char* getParentWindowID(char *readSock, int screenOffset) {
  // printf("screen offset = %d\n", screenOffset);
  // Return the heap pointer 'parentWindowID'.
  // 'free()' in 'window()'.
  char *parentWindowID = malloc(5 * sizeof(char));
  strcpy(parentWindowID, "a");
  parentWindowID[0] = readSock[screenOffset];
  parentWindowID[1] = readSock[screenOffset + 1];
  parentWindowID[2] = readSock[screenOffset + 2];
  parentWindowID[3] = readSock[screenOffset + 3];
  parentWindowID[4] = '\0';
  return parentWindowID;
}

char* getWindowID(char *readSock) {
  // Return the heap pointer 'windowID'.
  // 'free()' in 'window()'.
  char *windowID = malloc(5 * sizeof(char));
  strcpy(windowID, "a");
  // 'windowID' is 12-15 from the 'read()' char[] but
  // the first 'read()' length is '8'.
  windowID[0] = readSock[4];
  windowID[1] = readSock[5];
  windowID[2] = readSock[6];
  windowID[3] = readSock[7];
  windowID[4] = '\0';
  return windowID;
}

char* getVisualID(char *readSock, int screenOffset) {
  int visualScreenOffset = screenOffset + 32;
  // printf("screen offset = %d\n", visualScreenOffset);
  // Return the heap pointer 'visualID'.
  // 'free()' in 'window()'.
  char *visualID = malloc(5 * sizeof(char));
  strcpy(visualID, "a");
  visualID[0] = readSock[visualScreenOffset];
  visualID[1] = readSock[visualScreenOffset + 1];
  visualID[2] = readSock[visualScreenOffset + 2];
  visualID[3] = readSock[visualScreenOffset + 3];
  visualID[4] = '\0';
  free(readSock);
  return visualID;
}
