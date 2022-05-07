#ifndef STRONGHOLD_H
#include <stdio.h>

#define MAX_SS_NAME 21
#define MAX_SS_TYPE 7

//These will be used to modify room prices on different floor
#define LAYER_4 400
#define LAYER_5 1000
#define LAYER_6 2000
#define LAYER_7 3000

//Layers beyond 7 will use following forumla,
// as every layer beyond 7 gets progressively more expensive by 1500 
// With respect to n being current layer:
// Room Cost = ((n - 7) * LAYER_BEYOND) + LAYER_7

#define LAYER_BEYOND 1500

//Struct used to keep track of everything within the stronghold
typedef struct 
{
	unsigned int totalSSPrice;
	unsigned short totalSize;
	unsigned int EndTotal;

	unsigned short numBedsNeeded;
	unsigned short totalBeds;
	unsigned short workersNeeded;

	//Each Stronghold has an array of floors, both above and below ground
	Floor** fPtr; 

	//Each Stronghold also needs a workforce to maintain it
	Workforce wforce;

	//Each Stronghold has options for outfiting a personal militia
	Militia mSettings;
}Stronghold;

typedef struct
{
	unsigned short numRooms;
	unsigned short ssTotal;

	//Shows level of this floor, floors can be above and below ground, 0 is ground floor
	short level; 
	unsigned short layerCost;

	//Each floor will have an array of rooms that can vary
	//Future implementation of hashmap rather than array for faster adding and removing of rooms would be cool
	Room** rPtr;
}Floor;


typedef struct
{
	unsigned short price;
	unsigned short ssSize;
	unsigned char* sName;
	unsigned char* sType;

	//Will keep track of duplicate rooms on the floor, keeps from duplicate structs taking up space
	unsigned short numRooms; 

}Room;

typedef struct
{
	short dummy;
	//FILL OUT LATER
}Militia;

typedef struct
{
	short dummy;
	//FILL OUT LATER
}Workforce;


/* FUNCTIONS */

void initializeStronghold(Stronghold*);

//Will scrape through all rooms and floors to grab 
//the different totals needed for end calculation
void setTotals(Stronghold*);

short getStrongholdSize(Stronghold*);

short getFloorSize(Floor*);

//Will calculate end totals with all modifiers
void getStrongholdEndTotal(Stronghold*);

void addFloor(Stronghold*);

void addRoom(Floor*);

unsigned short getLayerCost(Floor*);

void removeRoom(Floor*);

//Will remove an entire floor, including all rooms, will have to put extreme warning
void removeFloor(Stronghold*);

void setMilitary(Stronghold*);

void setWorkforce(Stronghold*);

#endif // !STRONGHOLD_H

