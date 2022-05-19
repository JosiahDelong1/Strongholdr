#ifndef FILEIO_H
#define FILEIO_H
#include <stdio.h>
#include <string.h>
#include "stronghold.h"

#define MAX_CHAR_LENGTH 65
#define MAX_ARRAY_LENGTH 10

//Numbers needed for positions of Stronghold Types in split string
#define BASIC 1
#define FANCY 4
#define LUXURY 7
typedef unsigned char BYTE;

//Eventual FILEIO to implement

//Will open and return a Stronghold struct
Stronghold* loadStronghold(FILE*);

int writeStronghold(FILE* filePtr, Stronghold* sPtr);

//Specifically for reading and writinge stronghold files
FILE* openFile(const char* fileName, const char* fileMode);

//Will return the chosen room information for adding to floor
//getRoom will call getRoomInfo, which will query the user
//from there, we get their inputs and return the room
//We will need to get 2 inputs from user, which room, and the style
//of the room (Basic, Fancy, Luxury, etc)
Room* getRoom(FILE*);

//Will read from provided file to give a list of rooms available
Room* getRoomInfo(FILE*);

#endif // !FILEIO_H
