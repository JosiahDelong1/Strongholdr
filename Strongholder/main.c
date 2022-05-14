#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include "stronghold.h"
#include "fileio.h"


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