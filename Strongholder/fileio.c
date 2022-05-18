#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "stronghold.h"
#include "fileio.h"

#if defined(_WIN64) || defined(_WIN32)
#include <malloc.h>
#endif


/*
    read line from file
    split line to show space name
    user selects a rooms to add
    use number selection to get line again
    split that line to prompt for space type and size
    add room using that selection
*/