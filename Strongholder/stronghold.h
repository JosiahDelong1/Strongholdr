#ifndef STRONGHOLD_H
#define STRONGHOLD_H
#include <stdbool.h>

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

//The longest length of any String for a given Stronghold Space
//21 is account for /0
#define MAX_SS_LENGTH 21


//These will be used to modify room prices on different floor
#define STARTER_LAYER 0
#define LAYER_4 400
#define LAYER_5 1000
#define LAYER_6 2000
#define LAYER_7 3000

//Layers beyond 7 will use following forumla,
// as every layer beyond 7 gets progressively more expensive by 1500 (Basements get a little weird, as a second basement is a cost of LAYER_4
// With respect to n being current layer:
// Floor Room Cost = ((n - 7) * LAYER_BEYOND) + LAYER_7
// Basement Room Cost = ((-n - 5) * LAYER_BEYOND) + LAYER_7   (No longer have to use this, but I still like looking at it)

#define LAYER_BEYOND 1500

//Need a define for easier readability when initializing new floors
#define NewFloor true
#define NewBasement false

#define workSplitter true
#define roomSplitter false

typedef struct
{
	short cost;
	short wNumWorkers;
	unsigned char* wName;

	//FILL OUT LATER
}Workforce;

typedef struct
{
	float ssSize;
	unsigned short price;
	
	//need to keep track of the char sizes for serialization
	unsigned short nameSize;
	unsigned short typeSize;

	//Will keep track of duplicate rooms on the floor, keeps from duplicate structs taking up space
	unsigned short numRooms;

	Workforce** workers;

	unsigned char* sName;
	unsigned char* sType;

}Room;

typedef struct
{
	//numRooms is strictly the number of Room Pointers, roomTotal will be the actual number of rooms
	unsigned short numRooms;
	unsigned short roomTotal;
	float ssTotal;
	//Shows level of this floor, floors can be above and below ground, 
	//0 is ground floor, above are floors, below are basements
	short level;

	//Each floor above 2 and basement below -1 have additional room costs
	unsigned short layerCost;
	unsigned int floorCost;

	//Needed array position for deleting floors
	unsigned short pos;

	//Each floor will have an array of rooms that can vary
	//Future implementation of hashmap rather than array for faster adding and removing of rooms would be cool
	Room** rPtr;
}Floor;

//Struct used to keep track of everything within the stronghold
typedef struct
{
	float totalSize;
	unsigned int EndTotal;

	unsigned short numFloors;
	unsigned short numBedsNeeded;

	unsigned short totalBeds;
	unsigned short workersNeeded;

	//lowestDept needs to be signed so we can track
	//depth properly, almost didn't do that :p
	short heighestHeight;
	short lowestDepth;

	//Each Stronghold has an array of floors, both above and below ground
	Floor** fPtr; 

	//Each Stronghold also needs a workforce to maintain it
	//Workforce wforce;

	//Each Stronghold has options for outfiting a personal militia
	//Militia mSettings;
}Stronghold;

typedef struct
{
	short dummy;
	//FILL OUT LATER
}Militia;




/* FUNCTIONS */

//Need to add a Bool to check if for splitting room choices or workers
//to avoid printing lines for one or the other
char** splitSelection(FILE*, int*, int*, bool);

void initializeStronghold(Stronghold*);

void initializeFloorOne(Floor*);

void displayStronghold(Stronghold*);

void displayFloor(Floor*);

void displayRoom(Room*, unsigned short layerCost);

void addFloor(Stronghold* sPtr, bool floorType);

void sortFloors(Stronghold*);

void sortRooms(Floor*);

//Will scrape through all rooms and floors to grab 
//the different totals needed for end calculation
void setTotals(Stronghold*);

short getStrongholdSize(Stronghold*);

//Gets the Floor's size, cost, and number of rooms
void getFloorStats(Floor* fPtr);

//Will calculate end totals with all modifiers
void getStrongholdEndTotal(Stronghold*);

//Adds a selected room to a floor
void addRoomToFloor(Stronghold* sPtr, FILE* fPtr);

Floor* selectFloor(Stronghold*);

//Adds the actual room to floor pointer
void addRoom(Floor*, char**, int rSelection);

//Will return the chosen room information for adding to floor
//getRoom will call getRoomInfo, which will query the user
//from there, we get their inputs and return the room
//We will need to get 2 inputs from user, which room, and the style
//of the room (Basic, Fancy, Luxury, etc)
//Will read from provided file to give a list of rooms available
void getRoomInfo(FILE*);

int selectRoomAndType(Floor*, FILE*);

bool roomExists(Floor*, char**, int rSelection);

Room* getRoom(Floor*, char**, int rSelection);

unsigned short getLayerCost(Floor*);

//Will remove either 1, a custom amount, or all of 1 type of room
void removeRoom(Floor*);

//Will free all the allocated space from a room given to it
void freeRoom(Room** rPtr, int selection);

bool hasRooms(Floor*);

//Will remove an entire floor, including all rooms, will have to put extreme warning
void removeFloor(Stronghold*);

void freeFloors(Stronghold*);

void freeRooms(Floor*);

void setMilitary(Stronghold*);

void getWorkforce(Stronghold*);

#endif // !STRONGHOLD_H