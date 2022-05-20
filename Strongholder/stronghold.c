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
{	//Name: Type: Cost: Stronghold Spaces: Copies:
	printf("\t\t%s\n\t\t%s\n\t\t%d\n\t\t%d\n\t\t%d\n", rPtr->sName, rPtr->sType, rPtr->price, rPtr->ssSize, rPtr->numRooms);
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

void addRoom(Floor* fPtr, char** splits, int rSelection)
{
	fPtr->numRooms = 1;

	//Then we need to reallocate the space we had previously set for all the floors
	if(fPtr->rPtr == NULL)
		fPtr->rPtr = (Floor**)malloc(sizeof(Floor*) * fPtr->numRooms);
	else
		fPtr->rPtr = (Floor**)realloc(fPtr->rPtr,sizeof(Floor*) * fPtr->numRooms);
	//Make freeing function

	//Create a new Floor pointer for the sake of convenience
	Room* r = fPtr->rPtr[fPtr->numRooms - 1] = (Floor*)malloc(sizeof(Floor));

	r->nameSize = atoi(strlen(splits[0]));
	strcpy(r->sName, splits[0]);
	r->typeSize = atoi(strlen(splits[rSelection]));
	strcpy(r->sType,splits[rSelection]);
	r->price = atoi(splits[rSelection + 1]);
	r->ssSize = atoi(splits[rSelection + 2]);
	r->numRooms = 1;
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
}


bool roomExists(Floor* fPtr, char** splits, int rSelection)
{
	for(int i = 0;i < fPtr->numRooms; i++)
	{
		if(strcmp(fPtr->rPtr[i]->sName, splits[0]) == 0 && strcmp(fPtr->rPtr[i]->sType, splits[rSelection]) == 0)
			return true;
	}
	return false;
}

Room* getRoom(Floor* fPtr, char** splits, int rSelection)
{
	for(int i = 0;i < fPtr->numRooms; i++)
	{
		if(strcmp(fPtr->rPtr[i]->sName, splits[0]) == 0 && strcmp(fPtr->rPtr[i]->sType, splits[rSelection]) == 0)
			return fPtr->rPtr[i];
	}
	return NULL;
}

/*
*	for malloc'ing rooms later
	Room** rPtr = (Room**)malloc(sizeof(Room*));
*/