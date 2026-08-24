#ifndef WINDOW_H
#define WINDOW_H

char* xAuthority(char userPath[]);

char* openAuthority(char charAuthorityPath[]);

int getScreenOffset(char readSock[]);

struct fourInt getWindowID(char readSock[]);




struct fourInt getGCID(char *readSock, struct fourInt windowID);




struct fourInt getParentWindowID(char readSock[], int screenOffset);

struct fourInt getVisualID(char readSock[], int screenOffset);

int serverConnect(char userPath[]);

#endif
