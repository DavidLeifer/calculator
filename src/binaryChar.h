#ifndef BINARYCHAR_H
#define BINARYCHAR_H

// int and char structure.
/*
struct threeInt {
  int one;
  int two;
  int three;
  char *string;
};
*/
// Define functions in header and binaryFunctions.c.
// W3 schools wasn't specific it said 'gaurd'.

extern const char version[];

extern const int maxBinaryLength;

//extern const char features[];

struct threeInt decimal2intBinary(int decimal, int binaryLength);

char* intBinary2Char(struct threeInt intZerosBinary);

struct threeInt intBinaryCheck(struct threeInt intBinaryOne, struct threeInt intBinaryTwo);

int binary2Decimal(char charBinaryAdd[], int binaryAddOne);

int charBinary2Int(char binaryAddString[], int intBinaryGreatOne);

#endif
