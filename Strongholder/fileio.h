#ifndef FILEIO_H
#include <stdio.h>

typedef unsigned char BYTE;

//Eventual FILEIO to implement
FILE* openStronghold(const char* fileName, const char* fileMode);
int readFile(FILE* filePtr, BYTE* data, int bytesTorRead);
int writeFile(FILE* filePtr, BYTE* data, int bytesToWrite);

#endif // !FILEIO_H
