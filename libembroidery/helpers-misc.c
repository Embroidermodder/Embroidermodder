#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "helpers-misc.h"

int roundDouble(double src)
{
	if(src < 0.0) 
		return (int) ceil(src - 0.5);
    return (int)floor(src+0.5);
}

char* rTrim(char* const string, char junk)
{
    char* original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}

char* lTrim(char* const string, char junk)
{
    char* original = string;
    char* p = original;
    int trimmed = 0;
    do
    {
        if(*original != junk || trimmed)
        {
            trimmed = 1;
            *p++ = *original;
        }
    }
    while(*original++ != '\0');
    return string;
}

char* emb_strdup(const char* src)
{
    char* dest = (char*)malloc(strlen(src) + 1);
    if(dest) strcpy(dest, src);
    return dest;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
