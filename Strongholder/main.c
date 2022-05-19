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
	int i = 1;
	printf("Counting splits in string...\n\n");
	fgets(rString, MAX_CHAR_LENGTH, fPtr);
	char* token = strtok(rString, sep);
	while (token != NULL)
	{
		printf("%d: %s", i, token);
		i++;
		token = strtok(NULL, sep);
		printf("\nPress Enter to Continue");
		while (getchar() != '\n');
	}
}

 int main()
{
	 
	 /*testStrTok(fPtr);*/
	Stronghold nStrong;
	 testStrongholdAddFloor(&nStrong);
	 showStrongholdData(&nStrong);
	 FILE* fPtr = openFile("spaces.csv", "r");
	printf("\nGetting room info...\n");
	Room* r = getRoomInfo(fPtr);
	
	return 0;
}