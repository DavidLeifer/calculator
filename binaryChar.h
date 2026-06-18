#ifndef BINARYCHAR_H
#define BINARYCHAR_H

// Int structure.
struct threeInt {
  int one;
  int two;
  int three;
  char *string;
};

// Define functions in header and binaryFunctions.c.
// W3 schools wasn't specific it just said 'gaurd'.

extern const char version[];

struct threeInt decimal2intBinary(int decimal, int binaryLength);

char* intBinary2Char(struct threeInt intZerosBinary);

struct threeInt intBinaryCheck(struct threeInt intBinaryOne, struct threeInt intBinaryTwo);

struct threeInt binaryAddition(char binaryOne[], char binaryTwo[], struct threeInt intBinaryGreat);

int charBinary2Int(char binaryAddString[], int intBinaryGreatOne);

int binary2Decimal(int binary, int intBinaryGreatOne, int binaryAddOne);

#endif
