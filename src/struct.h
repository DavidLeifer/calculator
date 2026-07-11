#ifndef STRUCT_H
#define STRUCT_H

// int and char structure.
struct threeInt {
  int one;
  int two;
  int three;
  char *string;
};

struct xAuthorityRequest {
    short int byteOrder;        // 'l' = Little Endian, 'B' = Big Endian
    short int padOne;           // Unused padding byte
    short int majorVersion;     // X11 Major (typically 11)
    short int minorVersion;     // X11 Minor (typically 0)
    short int authNameLen;      // Length of "MIT-MAGIC-COOKIE-1" (18)
    short int authDataLen;      // Length of Cookie (16)
    short int padTwo;           // Unused padding bytes
    char      authName[20];     // 18 bytes padded to 4-byte boundary
    short int authData[16];     // 16 bytes of raw cookie data
};                                // The directions said not to null-terminate the char.

#endif
