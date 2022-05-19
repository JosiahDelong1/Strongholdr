#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stronghold.h"
#include "fileio.h"

#if defined(_WIN64) || defined(_WIN32)
#include <malloc.h>
#endif

void testStrongholdAddFloor(Stronghold* nStrong)
{
	initializeStronghold(nStrong);
	
	addFloor(nStrong, NewFloor);
	addFloor(nStrong, NewBasement);
	addFloor(nStrong, NewBasement);
	addFloor(nStrong, NewFloor);
	/*// addFloor(&nStrong, NewBasement);
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewBasement);
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewBasement);
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewFloor);
	*/

	sortFloors(nStrong);
	displayStronghold(nStrong);
	

}

void showStrongholdData(Stronghold* nStrong)
{
	printf("Floor size: %ld\nFloor Pointer Size: %ld\n", sizeof(Floor), sizeof(Floor*));
	printf("Floors: %d\nSpace allocated: %ld\n", nStrong->numFloors, sizeof(Floor) * nStrong->numFloors);
	printf("size of Int: %ld\n", sizeof(int));
}

//REMEMBER: To free all this memory in the real Room adding function
void testStrTok(FILE* fPtr)
{
	char* rString = (char*)malloc(sizeof(char) * MAX_CHAR_LENGTH);
	const char sep[2] = ",";

	int k = 0;
	char** splits = (char**)malloc(sizeof(char*) * MAX_ARRAY_LENGTH);

	printf("Counting splits in string...\n\n");
	fgets(rString, MAX_CHAR_LENGTH, fPtr);

	char* token = strtok(rString, sep);
	int count = 4;

	while(token != NULL)
	{
		printf("%d: %s\n", k, token);
		splits[k] = (char*)malloc(sizeof(char) * strlen(token));
		strcpy(splits[k], token);
		k++;
		token = strtok(NULL, sep);
	}
	
	//C has problems, Need to set the rest of the strings to NULL,
	// :| or just use k in the next for loop idiot
	for (int i = 1; i < k; i++)
	{
		if (splits[i] == NULL)
		{
			break;
		}
		else if (i == BASIC || i == FANCY || i == LUXURY)
		{
			printf("%s:\n\t--%s GP, Stronghold Spaces: %s\n\n", splits[i], splits[i + 1], splits[i + 2]);
		}
	
	}

	//Use atoi(char*) to convert string to int
	//use while(fgets(rString, MAX_CHAR_LENGTH, fPtr && count < RoomSelected - 1)) to get ID to wanted room
	//Then use above to get split data, then create room with above data
	//If adding duplicate rooms, use binary search to see if room with same name and type exists, 
	//If it does, increment the counter for that room
	
	/* USE THE FOLLOWING FOR SEARCHING SPECIFIC ROOM TYPE
	char* rString = (char*)malloc(sizeof(char) * MAX_CHAR_LENGTH);
	const char sep[2] = ",";
	
	int count = 5;
	int k = 0;

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
	*/
}

 int main()
{
	 
	 
	Stronghold nStrong;

	//testStrongholdAddFloor(&nStrong);
	//showStrongholdData(&nStrong);

	FILE* fPtr = openFile("spaces.csv", "r");
	//printf("\nGetting room info...\n");

	//Room* r = getRoomInfo(fPtr);

	//fseek(fPtr, 0, SEEK_SET);
	/*int i = atoi("Hello");
	if (i != NULL)
		printf(i);
	else
	printf("i = NULL");*/
	testStrTok(fPtr);

	return 0;
}