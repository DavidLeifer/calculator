#ifndef USERINPUT_H
#define USERINPUT_H

int fileLog(char userInput[], char answer[]);

int test(int small, int large, int mid, int longPrint);

struct threeInt inputFormat(char input[]);

char* inputArithmatic(struct threeInt inputFormatted);

void textGUI();

int interface(int arc, char argv[]);

#endif
