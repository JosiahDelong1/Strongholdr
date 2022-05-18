#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "stronghold.h"
#include "fileio.h"

#if defined(_WIN64) || defined(_WIN32)
#include <malloc.h>
#endif




FILE* openFile(const char* fileName, const char* fileMode)
{
    FILE* fPtr;

    if((fPtr = fopen(fileName, fileMode)) != NULL)
    {
        printf("File %s opened succesfully :D\n", fileName);
        return fPtr;
    }
    else
    {
        printf("File %s could not be opened :(\n", fileName);
        return NULL;
    }
}

//getRoom will call getRoomInfo
//getRoomInfo will display all rooms in file
Room* getRoom(FILE* fPtr)
{
    Room* nRoom;
    getRoomInfo(fPtr);


    return nRoom;
}

/*
    read line from file
    split line to show space name
    user selects a rooms to add
    use number selection to get line again
    split that line to prompt for space type and size
    add room using that selection
*/
Room* getRoomInfo(FILE* fPtr)
{
    char* rString;
    char* sep;
    *sep = ',';

    size_t bSize = MAX_CHAR_LENGTH;
    size_t strLen;
    unsigned long* stringSize;
    
    rString = (char*)malloc(sizeof(char) * MAX_CHAR_LENGTH);

    if(rString == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    int i = 1;
    printf("\n");
    while((strLen = getline(&rString, &bSize, fPtr)) != -1)
    {
        char* token = strtok(rString, sep);
        printf("%d. %s\n", i, token);
        i++;
    }
    sleep(1);
    free(rString);
    return NULL;
}