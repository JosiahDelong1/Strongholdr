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

	Copyright (C) 2022  Josiah DeLong

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

//Initialized the new stronghold, mallocs appropriate size
void initializeStronghold(Stronghold* sPtr)
{
	//Each stronghold only starts with 1 floor
	sPtr->fPtr = (Floor**)malloc(sizeof(Floor*)); 
	sPtr->fPtr[0] = (Floor*)malloc(sizeof(Floor));

	sPtr->totalSize = 0;
	sPtr->EndTotal = 0;

	sPtr->workersNeeded = 0;
	sPtr->numBedsNeeded = 0;
	sPtr->totalBeds = 0;

	sPtr->heighestHeight = 0;
	sPtr->lowestDepth = 0;

	initializeFloorOne(sPtr->fPtr[0]);

	//Not sure if I need to keep track of number of floors,
	//but it may come in handy later
	sPtr->numFloors = 1;
}

void initializeFloorOne(Floor* fPtr)
{
	fPtr->numRooms = 0;
	fPtr->ssTotal = 0;
	fPtr->roomTotal = 0;
	fPtr->level = 0;
	fPtr->floorCost = 0;
	fPtr->layerCost = STARTER_LAYER;
	fPtr->rPtr = NULL;
}

void displayStronghold(Stronghold* sPtr)
{
	printf("Number of Floors: %d\nHighest floor: %d\nLowest Depth: %d\n\n\n",sPtr->numFloors, sPtr->heighestHeight, sPtr->lowestDepth);

	for (int i = 0; i < sPtr->numFloors; i++)
	{
		printf("\n%d. ", i + 1);
		displayFloor(sPtr->fPtr[i]);
	}
}

void displayFloor(Floor* fPtr)
{
	if (fPtr->level < 0)
		printf("Basement #%d\n", abs(fPtr->level));
	else if (fPtr->level == 0)
		printf("Ground Floor (%d)\n", abs(fPtr->level)+1);
	else
		printf("Floor #%d\n", (fPtr->level)+1);

	printf("\n\tNumber of Rooms: %d\n\tTotal Cost in Gold: %d\n\tStronghold Space Total: %.2f", fPtr->roomTotal, fPtr->floorCost, fPtr->ssTotal);
	if(fPtr->level < 0)
		printf("\n\tLevel: %d\n\tLayer: %d\n\tExtra Floor Costs (Per Room): %d\n\n", fPtr->level, abs(fPtr->level) + 2,fPtr->layerCost);
	else
		printf("\n\tLevel: %d\n\tLayer: %d\n\tExtra Floor Costs (Per Room): %d\n\n", fPtr->level + 1, fPtr->level +2, fPtr->layerCost);
	
	//The room pointer array always initialiizes to NULL
	if (fPtr->rPtr != NULL)
	{
		for (int i = 0; i < fPtr->numRooms; i++)
		{
			displayRoom(fPtr->rPtr[i], fPtr->layerCost);
		}
	}
}

void displayRoom(Room* rPtr, unsigned short layerCost)
{	//Name: Type: Cost: Stronghold Spaces: Copies:
	printf("\n\t\tRoom: %s\n\t\tType: %s\n\t\tRoom Cost in Gold: %d\n\t\tStronghold Spaces: %.2f\n\t\tQuantity: %d\n\t\tTotal Cost in Gold: %d\n\t\tStronghold Space Total: %.2f\n\n", rPtr->sName, rPtr->sType, rPtr->price, rPtr->ssSize, rPtr->numRooms, (rPtr->price * rPtr->numRooms) + (layerCost * rPtr->numRooms), rPtr->ssSize * rPtr->numRooms);
}


//Calls all the necessary functions to add the wanted room to a floor
void addRoomToFloor(Stronghold* sPtr, FILE* fPtr)
{
	//Adding room steps
	Floor* floorPtr;
	if (floorPtr = selectFloor(sPtr))
	{
		getRoomInfo(fPtr);

		if(selectRoomAndType(floorPtr, fPtr) == -1)
			return;

		sortRooms(floorPtr);
		displayFloor(floorPtr);
	}
}


//Will be useful for setting the extra layer cost needed for adding rooms
//Will need height and depth in order to increment where needed
//REMEMBER: To add freeing function when removing Rooms and Floors
void addFloor(Stronghold* sPtr, bool floorType)
{
	//First we need to set the number of floors to +1
	sPtr->numFloors++;

	//Then we need to reallocate the space we had previously set for all the floors
	sPtr->fPtr = (Floor**)realloc(sPtr->fPtr, sizeof(Floor*) * sPtr->numFloors);
	//Make freeing function

	//Create a new Floor pointer for the sake of convenience
	Floor* fPtr = sPtr->fPtr[sPtr->numFloors - 1] = (Floor*)malloc(sizeof(Floor));

	fPtr->numRooms = 0;
	fPtr->ssTotal = 0;
	fPtr->roomTotal = 0;
	fPtr->floorCost = 0;
	//Here we are checking if we need to increment/decrement height/depth
	//using a typedef'd bool, then we set the level
	if (floorType == NewFloor)
	{
		sPtr->heighestHeight++;
		fPtr->level = sPtr->heighestHeight;
	}
	else
	{
		sPtr->lowestDepth--;
		fPtr->level = sPtr->lowestDepth;
	}

	fPtr->layerCost = getLayerCost(fPtr);

	fPtr->rPtr = NULL;
}

void sortFloors(Stronghold* sPtr)
{
	Floor* tempPtr = sPtr->fPtr[0];
	for(int i = 0; i < sPtr->numFloors - 1; i++)
	{
		tempPtr = sPtr->fPtr[0];
		for(int j = 0; j < sPtr->numFloors -i -1; j++)
		{
			if(sPtr->fPtr[j]->level > sPtr->fPtr[j+1]->level)
			{
				tempPtr = sPtr->fPtr[j];
				sPtr->fPtr[j] = sPtr->fPtr[j+1];
				sPtr->fPtr[j+1] = tempPtr; 
			}
		}
	}

	//set each floors position to grab later
	for (int i = 0; i < sPtr->numFloors; i++)
		sPtr->fPtr[i]->pos = i;
}

void sortRooms(Floor* fPtr)
{
	Room* tempPtr = fPtr->rPtr[0];
	for (int i = 0; i < fPtr->numRooms - 1; i++)
	{
		for (int j = 0; j < fPtr->numRooms - i - 1; j++)
		{
			if (strcmp(sToLower(fPtr->rPtr[j]->sName), sToLower(fPtr->rPtr[j + 1]->sName)) > 0)
			{
				tempPtr = fPtr->rPtr[j];
				fPtr->rPtr[j] = fPtr->rPtr[j + 1];
				fPtr->rPtr[j + 1] = tempPtr;
				printf("Swap:\n");
				printf("%s\n", fPtr->rPtr[j]->sName);
				printf("%s\n", fPtr->rPtr[j + 1]->sName);
			}
		}
	}
}

void getFloorStats(Floor* fPtr)
{
	fPtr->ssTotal = 0;
	fPtr->roomTotal = 0;
	fPtr->floorCost = 0;

	for (int i = 0; i < fPtr->numRooms; i++)
	{
		fPtr->ssTotal += (fPtr->rPtr[i]->ssSize * fPtr->rPtr[i]->numRooms);
		fPtr->roomTotal += fPtr->rPtr[i]->numRooms;
		fPtr->floorCost += (fPtr->rPtr[i]->price * fPtr->rPtr[i]->numRooms) + (getLayerCost(fPtr) * fPtr->rPtr[i]->numRooms);
	}
}

unsigned short getLayerCost(Floor* fPtr)
{
	int layer;

	//I can check the layer cost by getting the absolute
	//of the layer cost and adding 2 if it was negative initially,
	//or adding only 1 if it is positive
	if (fPtr->level < 0)
		layer = abs(fPtr->level) + 2;
	else
		layer = fPtr->level + 1;

	/*Layer is different from level, so this may be a tad confusing :(
	Basement 1 through Floor 2 are first 3 Layers, not really gone into more depth in the book
	Basement 2 and Floor 3 is Layer 4
	Basement 3 and Floor 4 is Layer 5
	 etc...
	The way layers are set up in the book, I need to add 2 to floors, 3 to basements
	*/

	//Had an error, was checking for <= 4, rather than 3
	if (layer <= 3)
		return STARTER_LAYER;

	switch (layer)
	{
	case 4:
		return LAYER_4;

	case 5: 
		return LAYER_5;
	case 6:
		return LAYER_6;

	default:
			return ((layer - 7) * LAYER_BEYOND) + LAYER_7;
	}
}

void addRoom(Floor* fPtr, char** splits, int rSelection)
{
	fPtr->numRooms++;

	//Then we need to reallocate the space we had previously set for all the floors
	if(fPtr->rPtr == NULL)
		fPtr->rPtr = (Room**)malloc(sizeof(Room*) * fPtr->numRooms);
	else
		fPtr->rPtr = (Room**)realloc(fPtr->rPtr,sizeof(Room*) * fPtr->numRooms);
	//Make freeing function

	//Create a new Floor pointer for the sake of convenience
	Room* r = fPtr->rPtr[fPtr->numRooms - 1] = (Room*)malloc(sizeof(Room));
	printf("%s", splits[rSelection]);

	r->nameSize = strlen(splits[0]);
	r->sName = (char*)malloc((sizeof(char) * r->nameSize) + 1);
	strcpy(r->sName, splits[0]);

	r->typeSize = strlen(splits[rSelection]);
	r->sType = (char*)malloc((sizeof(char) * r->typeSize) + 1);
	strcpy(r->sType,splits[rSelection]);

	r->price = atoi(splits[rSelection + 1]);
	r->ssSize = atof(splits[rSelection + 2]);
	r->numRooms = 1;

	getFloorStats(fPtr);
}

/*
    read line from file
    split line to show space name
    user selects a rooms to add
    use number selection to get line again
    split that line to prompt for space type and size
    add room using that selection
*/

//From what I can tell, this function gets all the room options
//and prints them out for the user to see?
//Whoops, guess I should've left comments 9 months ago when I was working on this lol
void getRoomInfo(FILE* fPtr)
{
	//Make sure to set the file pointer back to the start ;)
	fseek(fPtr, 0, SEEK_SET);
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


Floor* selectFloor(Stronghold* sPtr)
{
	int count;
	for (int i = 0; i < sPtr->numFloors; i++)
	{
		printf("\n%d. ", i + 1);
		displayFloor(sPtr->fPtr[i]);
	}
	printf("Please select a floor:\n(-1 to exit)\n");
	int iErr = scanf("%d", &count);

	
	while (count < 1 || count > sPtr->numFloors)
	{
		if (count == -1)
			return NULL;
		printf("Please enter a valid selection (1 - %d):\n(-1 to exit)\n", sPtr->numFloors);
		int iErr = scanf("%d", &count);
	}
	return sPtr->fPtr[count - 1];


}

//Returns an int so we can check if User wanted to exit or not
int selectRoomAndType(Floor* floor, FILE* fPtr)
{
	int* count = (int*)malloc(sizeof(int));
	int* k = (int*)malloc(sizeof(int));

	char** splits = splitSelection(fPtr, count, k, roomSplitter);
	if (splits == NULL)
	{
		free(count);
		free(k);
		free(splits);
		printf("No Room added :(");
		return -1;
	}


	int j = 0;
	//If an entry in the .csv has "N/A" and only 1 type, just skip the for loop
    printf("\n\n%s:\n", splits[0]);
    if (strcmp(splits[1], "N/A") == 0)
    {
        printf("\n\t--%s GP\n\t--Stronghold Spaces: %s\n\n", splits[2], splits[3]);
    }
    else 
    {
        for (int i = 1; i < *k; i++)
        {
            switch (i){
            case BASIC:
                j = BASIC;
                break;
            case FANCY:
               j = 2;
                break;
            case LUXURY:
                j = 3;
                break;
            }
            if (i == BASIC || i == FANCY || i == LUXURY)
            {
                printf("\t%d.%s:\n\t\t--%s GP\n\t\t--Stronghold Spaces: %s\n\n",j ,splits[i], splits[i + 1], splits[i + 2]);
            }
        }
    }
    
	if (j != 0)
	{
		printf("Please select a  option:\n");
		int iErr = scanf("%d", count);
		//We use the previous use of k to keep track of how many options there are
		while (*count < 1 || *count > j)
		{
			printf("Please select a valid option:\n");
			iErr = scanf("%d", count);
		}
		//Basically just use the reverse of the above case switch to properly grab the data we need
		//for making a new Room
		switch (*count)
		{
		case 1:
			*count = BASIC;
			break;
		case 2:
			*count = FANCY;
			break;
		case 3:
			*count = LUXURY;
		}
	}
	else
	{
		*count = 1;
	}
    if (roomExists(floor, splits, *count))
    {
        Room* r = getRoom(floor, splits, *count);
        r->numRooms++;
		getFloorStats(floor);
    }
    else
    {
        addRoom(floor, splits, *count);
    }

	free(count);
	count = NULL;
	free(k);
	k = NULL;
	free(splits);
	splits = NULL;

	return 1;
}

char** splitSelection(FILE* fPtr, int* count, int* k, bool splitter)
{
	fseek(fPtr, 0, SEEK_SET);
	char* rString = (char*)malloc(sizeof(char) * MAX_CHAR_LENGTH);
	char** splits = (char**)malloc(sizeof(char*) * MAX_ARRAY_LENGTH);
	const char sep[2] = ",";

	*k = 0;
	int iErr = 0;

	if (rString == NULL || splits == NULL) {
		// Handle memory allocation error
		return 1;
	}

	//Some minor input validation, don't want the user inputing
	//Also need to check if this is being used for 
	if (splitter == roomSplitter)
	{
		printf("Please Select a Room (1 - 32)\nEnter -1 to exit:\n");
		iErr = scanf("%d", count);
		(*count)--;
	}

	//If my selection for line when grabbing workers if correct, this 
	//loop won't even be called
	while ((*count < 0 || *count > 33) || (*count + 1) == -1)
	{
		if (((*count) + 1) == -1 && splitter == roomSplitter)
		{
			printf("Exiting...\n");
			return NULL;
		}
		printf("Please enter a valid room number (1 - 32)\nEnter -1 to exit:\n");
		iErr = scanf("%d", count);

		(*count)--;
	}

	//Grab the line of the room the user wants
	while (fgets(rString, MAX_CHAR_LENGTH, fPtr) && *k < *count)
	{
		(*k)++;
	}

	//Split the line into usable strings
	//Keep k to track the number of splits we have later
	char* token = strtok(rString, sep);
	*k = 0;
	while (token != NULL)
	{
		splits[*k] = (char*)malloc(sizeof(char) * (strlen(token) + 1));

		strcpy(splits[*k], token);
		(*k)++;
		token = strtok(NULL, sep);
	}

	free(rString);

	return splits;
}

bool roomExists(Floor* fPtr, char** splits, int rSelection)
{
	for (int i = 0; i < fPtr->numRooms; i++)
	{
		if (strcmp(fPtr->rPtr[i]->sName, splits[0]) == 0 && strcmp(fPtr->rPtr[i]->sType, splits[rSelection]) == 0)
			return true;
	}
	return false;
}

Room* getRoom(Floor* fPtr, char** splits, int rSelection)
{
	for (int i = 0; i < fPtr->numRooms; i++)
	{
		if (strcmp(fPtr->rPtr[i]->sName, splits[0]) == 0 && strcmp(fPtr->rPtr[i]->sType, splits[rSelection]) == 0)
			return fPtr->rPtr[i];
	}
	return NULL;
}

void removeFloor(Stronghold* sPtr)
{
	printf("\n\nREMOVE FLOOR:\n\n");

	//I need to find a way to set the ACTUAL pointer within sPtr to NULL
	//Right now, I can only check if fPtr below is NULL, which is a different pointer
	//*EDIT*: I have added a position short that is automatically set everytime my floors
	//		  are sorted.  Now each floor essentially has their position stored so I can grab
	//		  it at any time/
	Floor* fPtr = selectFloor(sPtr);
	int pos = fPtr->pos;

	//first we check if the pointer is null, then check if it has any rooms,
	//other wise we just set the room pointer to null and the actual floor to null/
	if (fPtr != NULL)
	{
		if (fPtr->rPtr != NULL)
		{
			for (int i = 0; i < fPtr->numRooms; i++)
			{
				freeRoom(fPtr->rPtr, &i);

				//function above does this below
				/*free(fPtr->rPtr[i]->sName);
				fPtr->rPtr[i]->sName = NULL;
				free(fPtr->rPtr[i]->sType);
				fPtr->rPtr[i]->sType = NULL;
				free(fPtr->rPtr[i]);
				fPtr->rPtr[i] = NULL;*/
			}

			free(fPtr->rPtr);
			fPtr->rPtr = NULL;
			free(fPtr);
			fPtr = NULL;

			sPtr->fPtr[pos] = NULL;
		}


		for (int j = pos; j < sPtr->numFloors - 1; j++)
		{
			sPtr->fPtr[j] = sPtr->fPtr[j + 1];
			if (sPtr->fPtr[j]->level < 0)
				sPtr->fPtr[j]->level++;
			else
				sPtr->fPtr[j]->level--;
		}

		//Ex: floors 1 2 3 are 0 1 2 in arrays,  numfloors is reduced from 3 to 2
		sPtr->numFloors--;
		//numfloors now equals 2, floor 3 is removed
		sPtr->fPtr[sPtr->numFloors] = NULL;

		//Realloc 2 floors
		sPtr->fPtr = (Floor**)realloc(sPtr->fPtr, sizeof(Floor*) * sPtr->numFloors);

		displayStronghold(sPtr);
	}
	else
	{
		printf("No such floor to remove\nReturning...\n");
		return;
	}

}

void removeRoom(Floor* fPtr)
{
	printf("\n\n");
	
	if (hasRooms(fPtr))
	{
		if (fPtr->level < 0)
			printf("Basement #%d", abs(fPtr->level));
		else if (fPtr->level == 0)
			printf("Ground Floor (%d)", abs(fPtr->level) + 1);
		else
			printf("Floor #%d", (fPtr->level) + 1);

		printf(" has no rooms.\nReturning...");
		return;
	}

	for (int i = 0; i < fPtr->numRooms; i++)
	{
		printf("%d. %s : %s\n\n", i + 1, fPtr->rPtr[i]->sName, fPtr->rPtr[i]->sType);
	}

	printf("\nPlease select a room to remove:\n(-1 to exit)\n");

	int selection;

	int iErr = scanf("%d", &selection);

	if (selection == -1)
		return;
	else while (selection > fPtr->numRooms || selection < 0)
	{
		printf("\n\nPlease select a number between 1 and %d", fPtr->numRooms);
		iErr = scanf("%d", selection);
	}

	//selection needs to be -1 as numRooms is the actual number of rooms, not the position in the pointer array
	(selection)--;

	int amount;

	displayRoom(fPtr->rPtr[selection], fPtr->layerCost);

	printf("\n\nYou have chosen %s:%s.\nPlease enter a removal amount (-1 to exit): ", fPtr->rPtr[selection]->sName, fPtr->rPtr[selection]->sType);
	iErr = scanf("%d", &amount);

	if (amount == fPtr->rPtr[selection]->numRooms)
	{
		freeRoom(fPtr->rPtr, selection);

		

		for (int j = selection; j < fPtr->numRooms - 1; j++)
		{
			fPtr->rPtr[j] = fPtr->rPtr[j + 1];
		}
		fPtr->numRooms--;

		fPtr->rPtr[fPtr->numRooms] = NULL;

		if (fPtr->numRooms == 0)
			fPtr->rPtr = NULL;
		else
		{	
			printf("%d", sizeof(Room*) * fPtr->numRooms);
			fPtr->rPtr = (Room**)realloc(fPtr->rPtr, sizeof(Room*) * fPtr->numRooms);
		}
	}
	else
	{
		fPtr->rPtr[selection]->numRooms = fPtr->rPtr[selection]->numRooms - amount;
		fPtr->roomTotal = fPtr->roomTotal - amount;
	}

}

void freeRoom(Room** rPtr, int selection)
{
	free(rPtr[selection]->sName);
		rPtr[selection]->sName = NULL;

		free(rPtr[selection]->sType);
		rPtr[selection]->sType = NULL;

		free(rPtr[selection]);
		rPtr[selection] = NULL;
}


//I think whats happening here is I accidentally correctly set up my if statement,
//since my pointer to pointer (fPtr->rPtr) is set to null on creation, for some reason it gives it the value 0
//so in the if statment here, since my non null rPtr's are not 0 it returns false, which is what I accidentally 
//set backwards. I meant to return true on NULL in the if statement but this works too I guess
bool hasRooms(Floor* fPtr)
{
	if (fPtr->rPtr)
		return false;
	return true;
}


/*
*	for malloc'ing rooms later
	Room** rPtr = (Room**)malloc(sizeof(Room*));
*/