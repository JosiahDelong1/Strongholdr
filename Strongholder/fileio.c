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

Room* selectRoomAndType(FILE* fPtr)
{
    fseek(fPtr, 0, SEEK_SET);
    char* rString = (char*)malloc(sizeof(char) * MAX_CHAR_LENGTH);
    char** splits = (char**)malloc(sizeof(char*) * MAX_ARRAY_LENGTH);
	const char sep[2] = ",";
	
	int* count;
	int k = 0;

    printf("Please Select a Room:\n");
    scanf("%d", count);
    while(*count < 0 || *count > 33)
    {
        printf("Please enter a valid room number (1 - 32)\n");
        scanf("%d", count);
    }

	while (fgets(rString, MAX_CHAR_LENGTH, fPtr) && k < count)
	{
		k++;
	}

	char* token = strtok(rString, sep);
	k = 0;
	while(token != NULL)
	{
		printf("%d: %s\n", k, token);
		splits[k] = (char*)malloc(sizeof(char) * strlen(token));
		strcpy(splits[k], token);
		k++;
		token = strtok(NULL, sep);
	}
}