#include <stdio.h>
#include <string.h>
#include "stronghold.h"
#include "fileio.h"



void initializeStronghold(Stronghold* sPtr)
{
	sPtr->fPtr = (Floor*)malloc(sizeof(Floor));
	sPtr->fPtr[0]->numRooms = 0;
	sPtr->fPtr[0]->level = 0;
	sPtr->fPtr[0]->layerCost = 0;
}