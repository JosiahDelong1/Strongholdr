#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stronghold.h"
#include "fileio.h"

#if defined(_WIN64) || defined(_WIN32)
#include <malloc.h>
#endif

//Initialized the new stronghold, mallocs appropriate size
void initializeStronghold(Stronghold* sPtr)
{
	//Each stronghold only starts with 1 floor
	sPtr->fPtr = (Floor**)malloc(sizeof(Floor*)); 
	sPtr->fPtr[0] = (Floor*)malloc(sizeof(Floor));

	sPtr->totalSSPrice = 0;
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
	fPtr->level = 0;
	fPtr->layerCost = STARTER_LAYER;
	fPtr->rPtr = NULL;
}

void displayStronghold(Stronghold* sPtr)
{
	printf("Number of Floors: %d\nHighest floor: %d\nLowest Depth: %d\n\n\n",sPtr->numFloors, sPtr->heighestHeight, sPtr->lowestDepth);

	for (int i = 0; i < sPtr->numFloors; i++)
	{
		displayFloor(sPtr->fPtr[i]);
	}
}

void displayFloor(Floor* fPtr)
{
	if (fPtr->level < 0)
		printf("Basement #%d\n", abs(fPtr->level));
	else
		printf("Floor #%d\n", fPtr->level);

	printf("\nNumber of Rooms: %d\nStronghold Space Total: %d", fPtr->numRooms, fPtr->ssTotal);
	if(fPtr->level < 0)
		printf("\nLevel: %d\nLayer: %d\nExtra Floor Costs: %d\n\n", fPtr->level, abs(fPtr->level) + 2,fPtr->layerCost);
	else
		printf("\nLevel: %d\nLayer: %d\nExtra Floor Costs: %d\n\n", fPtr->level + 1, fPtr->level +2, fPtr->layerCost);
	
	//The room pointer array always initialiizes to NULL
	if (fPtr->rPtr != NULL)
	{
		for (int i = 0; i < fPtr->numRooms; i++)
		{
			displayRoom(fPtr->rPtr[i]);
		}
	}
}

void displayRoom(Room* rPtr)
{
	printf("No rooms");
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
}

unsigned short getLayerCost(Floor* fPtr)
{
	//I can check the layer cost by getting the absolute
	//of the layer cost and adding 2 if it was negative initially
	int layer = abs(fPtr->level) + 2;
	
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



/*
*	for malloc'ing rooms later
	Room** rPtr = (Room**)malloc(sizeof(Room*));
*/