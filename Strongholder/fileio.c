#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stronghold.h"
#include "fileio.h"

#if defined(_WIN64) || defined(_WIN32)
#include <malloc.h>

#elif(__unix__)
#include <unistd.h>
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


/*
    read line from file
    split line to show space name
    user selects a rooms to add
    use number selection to get line again
    split that line to prompt for space type and size
    add room using that selection
*/
void getRoomInfo(FILE* fPtr)
{
    char* rString;
    const char sep[2] = ",";

    size_t bSize = MAX_CHAR_LENGTH;

    rString = (char*)malloc(sizeof(char) * MAX_CHAR_LENGTH);

    if(rString == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    int i = 1;
    printf("\n");

    //while((strLen = getline(&rString, &bSize, fPtr)) != -1)
    while(fgets(rString, MAX_CHAR_LENGTH, fPtr))
    {
        char* token = strtok(rString, sep);
        printf("%d. %s\n", i, token);
        i++;
    }
    
    free(rString);
}

void selectRoomAndType(Floor* floor, FILE* fPtr)
{
    fseek(fPtr, 0, SEEK_SET);
    char* rString = (char*)malloc(sizeof(char) * MAX_CHAR_LENGTH);
    char** splits = (char**)malloc(sizeof(char*) * MAX_ARRAY_LENGTH);
	const char sep[2] = ",";
	
    int count;
	int k = 0;

    //Some minor input validation, don't want the user inputing 
    printf("Please Select a Room:\n");
    int iErr = scanf("%d", &count);
    count--;
    while(count < 0 || count > 33)
    {
        printf("Please enter a valid room number (1 - 32)\n");
        iErr = scanf("%d", &count);
        count--;
    }

    if (count == -1)
    {
        printf("okay\n");
        return;
    }
	while (fgets(rString, MAX_CHAR_LENGTH, fPtr) && k < count)
	{
		k++;
	}

	char* token = strtok(rString, sep);
	k = 0;
	while(token != NULL)
	{
		splits[k] = (char*)malloc(sizeof(char) * strlen(token));
		strcpy(splits[k], token);
		k++;
		token = strtok(NULL, sep);
	}

    k = 0;
    //If an entry in the .csv has "N/A" and only 1 type, just skip the for loop
    printf("\n\n%s:\n", splits[0]);
    if (strcmp(splits[1], "N/A") == 0)
    {
        printf("\n\t--%s GP\n\t--Stronghold Spaces: %s\n\n", splits[2], splits[3]);
    }

    else 
    {
        int k = 0;
        for (int i = 1; i < k; i++)
        {
            switch (i){
            case BASIC:
                k = BASIC;
                break;
            case FANCY:
               k = 2;
                break;
            case LUXURY:
                k = 3;
                break;
            }
            if (i == BASIC || i == FANCY || i == LUXURY)
            {
                printf("\t%d.%s:\n\t\t--%s GP\n\t\t--Stronghold Spaces: %s\n\n",k ,splits[i], splits[i + 1], splits[i + 2]);
            }
            k++;
        }
    }
    
    iErr = scanf("%d", &count);
    //We use the previous use of k to keep track of how many options there are
    while (count < k || count > k)
    {
        printf("Please select a valid option:\n");
        iErr = scanf("%d", &count);
    }
    //Basically just use the reverse of the above case switch to properly grab the data we need
    //for making a new Room
    switch (count)
    {
    case 1:
        count = BASIC;
        break;
    case 2: 
        count = FANCY;
        break;
    case 3:
        count = LUXURY;
    }

    if (roomExists(floor, splits, count))
    {
        Room* r = getRoom(floor, splits, count);
        r->numRooms++;
    }
    else
    {
        addRoom(floor, splits, count);
    }
   


    return;
}