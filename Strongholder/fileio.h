#ifndef FILEIO_H
#define FILEIO_H
#include <stdio.h>
#include <string.h>
#include "stronghold.h"


typedef unsigned char BYTE;

//Eventual FILEIO to implement

//Will open and return a Stronghold struct
Stronghold* loadStronghold(FILE*);

int writeStronghold(FILE* filePtr, Stronghold* sPtr);

//Specifically for reading and writinge stronghold files
FILE* openFile(const char* fileName, const char* fileMode);

//Will read from provided file to give a list of rooms available
int getRoomInfo(FILE*);

//Will return the chosen room information for adding to floor
//getRoom will call getRoomInfo, which will query the user
//from their we get their inputs and return the room
//We will need to get 2 inputs from user, which room, and the style
//of the room (Basic, Fancy, Luxury, etc)
Room getRoom(FILE*);

#endif // !FILEIO_H
