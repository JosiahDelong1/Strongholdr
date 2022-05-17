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
	printf("Highest height: %d\nHighest height + 1: %d\n", nStrong->heighestHeight, nStrong->heighestHeight + 1);
	addFloor(nStrong, NewFloor);
	addFloor(nStrong, NewBasement);
	addFloor(nStrong, NewBasement);
	/*// addFloor(&nStrong, NewBasement);
	
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewFloor);
	
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewBasement);
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewBasement);
	addFloor(&nStrong, NewFloor);
	addFloor(&nStrong, NewFloor);
	sortFloors(&nStrong);
	displayStronghold(&nStrong);
	*/

}

void showStrongholdData(Stronghold* nStrong)
{
	printf("Floor size: %ld\nFloor Pointer Size: %ld\n", sizeof(Floor), sizeof(Floor*));
	printf("Floors: %d\nSpace allocated: %ld\n", nStrong->numFloors, sizeof(Floor) * nStrong->numFloors);
	printf("size of Int: %ld", sizeof(int));
}

 int main()
{
	Stronghold nStrong;
	testStrongholdAddFloor(&nStrong);
	showStrongholdData(&nStrong);

	return 0;
}