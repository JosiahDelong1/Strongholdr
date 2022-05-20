#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stronghold.h"
#include "fileio.h"

#if defined(_WIN64) || defined(_WIN32)
#include <malloc.h>

#elif(__unix__)
#include <unistd.h>
#endif

FILE* openFile(const char* fileName, const char* fileMode)
{
    FILE* fPtr;

    if((fPtr = fopen(fileName, fileMode)) != NULL)
    {
        printf("File %s opened succesfully :D\n", fileName);
        return fPtr;
    }
    else
    {
        printf("File %s could not be opened :(\n", fileName);
        return NULL;
    }
}


