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

