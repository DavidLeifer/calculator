#ifndef INTERFACE_H
#define INTERFACE_H

//versionLength
//featuresLength
extern const char version[];

extern const char features[];

struct threeInt userOutput(char strUserInput[]);

void textUI();

int interface(int arc, char argv[]);

#endif
