#ifndef UTILITY_H
#define UTILITY_H

char *slice(char charInput[], int start, int end);

int charLength(char charInput[]);

int intLength(int intInput);

struct threeInt charAppend(char firstChar[], int firstLength, char appendChar[], int appendLength, int space);

#endif
