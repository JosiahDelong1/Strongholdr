#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stronghold.h"
#include "fileio.h"

#if defined(_WIN64) || defined(_WIN32)
#include <malloc.h>
#endif

/*
	Name: Strongholder
	Purpose: A program to help DM's and players alike build and budget their homebrew strongholds and bases

	Copyright (C) 2022 Josiah DeLong

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
		if (token != NULL)
		{
			printf("%d: %s\n", k, token);
			splits[k] = (char*)malloc(sizeof(char) * strlen(token));
			strcpy(splits[k], token);
			k++;
		}
		token = strtok(NULL, sep);
	}
	
	//C has problems, Need to set the rest of the strings to NULL,
	// :| or just use k in the next for-loop idiot
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

void testAddRoom(Stronghold* sPtr, FILE* fPtr)
{
	addRoomToFloor(sPtr, fPtr);
	addRoomToFloor(sPtr, fPtr);

}

void testRemoveFloor(Stronghold* sPtr, FILE* fPtr)
{
	addRoomToFloor(sPtr, fPtr);
	addRoomToFloor(sPtr, fPtr);
	removeFloor(sPtr);
	displayStronghold(sPtr);
}

void testToLower()
{
	char str[] = "HELLO WORLD!";

	printf("%s", sToLower(&str));
}

void testRemoveRoom(Stronghold* sPtr, FILE* fPtr)
{
	addRoomToFloor(sPtr, fPtr);
	addRoomToFloor(sPtr, fPtr);
	addRoomToFloor(sPtr, fPtr);
	
	Floor* floor = selectFloor(sPtr);

	removeRoom(floor);
	getFloorStats(floor);
	displayStronghold(sPtr);
}

void testSpliiter()
{
	FILE* workPtr = openFile("staffWages.csv", "r");

	int* count = (int*)malloc(sizeof(int));
	*count = 1;
	int* k = (int*)malloc(sizeof(int));

	char** workline = splitSelection(workPtr, count, k, workSplitter);

	for (int i = 0; i < *k; i++)
	{
		printf("%s : %s", workline[i], workline[i+1]);
		i++;
	}
}

 int main()
{
	 
	 
	Stronghold nStrong;
	initializeStronghold(&nStrong);
	testStrongholdAddFloor(&nStrong);
	//showStrongholdData(&nStrong);

	FILE* fPtr = openFile("spaces.csv", "r");
	//printf("\nGetting room info...\n");

	//Room* r = getRoomInfo(fPtr);

	//fseek(fPtr, 0, SEEK_SET);

	// testStrTok(fPtr);
	//testAddRoom(&nStrong,fPtr);

	//displayStronghold(&nStrong);
	//fclose(fPtr);

	testRemoveFloor(&nStrong, fPtr);

	//testRemoveRoom(&nStrong, fPtr);
	 //testToLower();
	 //testSpliiter();
	return 0;
}