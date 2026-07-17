#ifndef WINDOW_H
#define WINDOW_H

char* xAuthority(char userPath[]);

char* openAuthority(char charAuthorityPath[]);

int getScreenOffset(char readSock[]);

char* getParentWindowID(char readSock[], int screenOffset);

char* getWindowID(char readSock[]);

char* getVisualID(char readSock[], int screenOffset);

#endif
