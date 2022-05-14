#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "fileio.h"
#include "stronghold.h"


Stronghold* testStrongholdAddFloor()
{
	Stronghold nStrong;
	initializeStronghold(&nStrong);
	addFloor(&nStrong, NewBasement);
	addFloor(&nStrong, NewFloor);

	displayStronghold(&nStrong);
}


 int main()
{

	testStrongholdAddFloor();

	return 0;
}