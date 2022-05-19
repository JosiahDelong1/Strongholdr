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

void testStrTok(FILE* fPtr)
{
	char* rString = (char*)malloc(sizeof(char) * MAX_CHAR_LENGTH);
	const char sep[2] = ",";

	int i = 0;
	int j = 0;


	char** splits = (char**)malloc(sizeof(char*) * 10);

	printf("Counting splits in string...\n\n");
	fgets(rString, MAX_CHAR_LENGTH, fPtr);

	char* token = strtok(rString, sep);
	while(token != NULL)
	{
		printf("%d: %s\n", i, token);
		splits[i] = (char*)malloc(sizeof(char) * strlen(token) + 1);
		strcpy(splits[i], token);
		i++;
		token = strtok(NULL, sep);
	}

	for (int i = 1; i < 10; i++)
	{
		if (splits[i] == NULL)
		{
			break;
		}
		else if (i==1 || i == 4 || i ==7 && splits[i] != NULL)
		{
			printf("%s:\n\t--%s GP, Stronghold Spaces: %s\n", splits[i], splits[i + 1], splits[i + 2]);
		}
	}
	printf("NO ATOI: %s GP, Stronghold Space: %s", splits[5], splits[6]);

	printf("%d GP, Stronghold Space: %d", atoi(splits[5]), atoi(splits[6]));
	


}

 int main()
{
	 
	 
	//Stronghold nStrong;

	//testStrongholdAddFloor(&nStrong);
	//showStrongholdData(&nStrong);

	FILE* fPtr = openFile("spaces.csv", "r");
	//printf("\nGetting room info...\n");

	//Room* r = getRoomInfo(fPtr);

	//fseek(fPtr, 0, SEEK_SET);
	//int i = atoi("Hello");
	//if (i != NULL)
	//	printf(i);
	//else
	//printf("i = NULL");
	testStrTok(fPtr);

	return 0;
}