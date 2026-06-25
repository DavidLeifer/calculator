#ifndef USERINPUT_H
#define USERINPUT_H

char* charUserInput();

int test(int small, int large, int mid, int longPrint);

struct threeInt inputFormat(char input[]);

char* inputArithmatic(struct threeInt inputFormatted);

struct threeInt dateTime(int logCheck);

int fileLogCheck();

char* outputArithmatic(char userInput[]);

struct threeInt manualInput(struct threeInt userInputTime, char strUserInput[], char answer[]);

void fileLog(char userInput[], int fileCheck);

#endif
