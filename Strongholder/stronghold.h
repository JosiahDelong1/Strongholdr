#ifndef STRONGHOLD_H
#define STRONGHOLD_H
#include <stdbool.h>


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

typedef struct
{
	unsigned short price;
	unsigned short ssSize;

	//need to keep track of the char sizes for serialization
	unsigned short nameSize;
	unsigned short typeSize;

	unsigned char* sName;
	unsigned char* sType;

	//Will keep track of duplicate rooms on the floor, keeps from duplicate structs taking up space
	unsigned short numRooms;

}Room;

typedef struct
{
	unsigned short numRooms;
	unsigned short ssTotal;

	//Shows level of this floor, floors can be above and below ground, 
	//0 is ground floor, above are floors, below are basements
	short level;

	//Each floor above 2 and basement below -1 have additional room costs
	unsigned short layerCost;

	//Each floor will have an array of rooms that can vary
	//Future implementation of hashmap rather than array for faster adding and removing of rooms would be cool
	Room** rPtr;
}Floor;

//Struct used to keep track of everything within the stronghold
typedef struct
{
	unsigned int totalSSPrice;
	unsigned short totalSize;
	unsigned short numFloors;
	unsigned int EndTotal;

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

typedef struct
{
	short dummy;
	//FILL OUT LATER
}Workforce;


/* FUNCTIONS */

void initializeStronghold(Stronghold*);

void initializeFloorOne(Floor*);

void displayStronghold(Stronghold*);

void displayFloor(Floor*);

void displayRoom(Room*);

void addFloor(Stronghold* sPtr, bool floorType);

void sortFloors(Stronghold*);
//Will scrape through all rooms and floors to grab 
//the different totals needed for end calculation
void setTotals(Stronghold*);

short getStrongholdSize(Stronghold*);

short getFloorSize(Floor*);

//Will calculate end totals with all modifiers
void getStrongholdEndTotal(Stronghold*);

void addRoom(Floor*, char**, int rSelection);

//Will return the chosen room information for adding to floor
//getRoom will call getRoomInfo, which will query the user
//from there, we get their inputs and return the room
//We will need to get 2 inputs from user, which room, and the style
//of the room (Basic, Fancy, Luxury, etc)
//Will read from provided file to give a list of rooms available
void getRoomInfo(FILE*);

void selectRoomAndType(Floor*, FILE*);

bool roomExists(Floor*, char**, int rSelection);

Room* getRoom(Floor*, char**, int rSelection);

unsigned short getLayerCost(Floor*);

void removeRoom(Floor*);

//Will remove an entire floor, including all rooms, will have to put extreme warning
void removeFloor(Stronghold*);

void freeFloors(Stronghold*);

void freeRooms(Floor*);

void setMilitary(Stronghold*);

void setWorkforce(Stronghold*);

#endif // !STRONGHOLD_H