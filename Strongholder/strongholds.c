#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include "stronghold.h"
#include "fileio.h"


//Initialized the new stronghold, mallocs appropriate size
void initializeStronghold(Stronghold* sPtr)
{
	//Each stronghold only starts with 1 floor
	sPtr->fPtr = (Floor*)malloc(sizeof(Floor*)); 
	initializeFloorOne(sPtr->fPtr[0]);

	//Not sure if I need to keep track of number of floors,
	//but it may come in handy later
	sPtr->numFloors = 1;
}

void initializeFloorOne(Floor* fPtr)
{
	fPtr->level = 0;
	fPtr->layerCost = STARTER_LAYER;
	fPtr->rPtr = NULL;
}


//Will be useful for setting the extra layer cost needed for adding rooms
//Will need height and depth in order to increment where needed
void addFloor(Stronghold* sPtr, bool floorType, int* highestHeight, int* lowestDepth)
{
	//First we need to set the number of floors to +1
	sPtr->numFloors = sPtr->numFloors + 1;

	//Then we need to reallocate the space we had previously set for all the floors
	sPtr->fPtr = (Floor*)realloc(sPtr->fPtr, sizeof(Floor*) * sPtr->numFloors);
	
	//Create a new Floor pointer for the sake of convenience
	Floor* fPtr = sPtr->fPtr[sPtr->numFloors - 1];

	fPtr->numRooms = 0;
	fPtr->ssTotal = 0;
	
	//Here we are checking if we need to increment
	//or decrement the height and depth, then we set the level
	if (floorType == NewFloor)
	{
		*highestHeight++;
		fPtr->level = *highestHeight;
	}
	else
	{
		*lowestDepth++;
		fPtr->level = *lowestDepth;
	}

	fPtr->layerCost = getLayerCost(fPtr);

	fPtr->rPtr = NULL;

}



unsigned short getLayerCost(Floor* fPtr)
{
	int level;

	//I can check the layer cost by getting the absolute
	//of the layer cost and adding 1 if it was negative initially
	if (fPtr->level < 0)
	{
		level = abs(fPtr->level) + 1;
	}
	level = fPtr->level;
	
	//The layer will also always be 1 more than the level
	int layer = level++;

	if (level < 4)
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
	Room** rPtr = (Room*)malloc(sizeof(Room*));
*/