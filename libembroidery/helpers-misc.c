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

/* Removes all characters from the right end of the string that match (junk), moving left until a mismatch occurs. */
char* rTrim(char* const string, char junk)
{
    char* original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}

/* Removes all characters from the left end of the string that match (junk), moving right until a mismatch occurs. */
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

/* Optimizes the number for output to a text file and returns it as a string. */
char* emb_optOut(double num, char* str)
{
    /* Convert the number to a string */
    sprintf(str, "%.10f", num);
    /* Remove trailing zeroes */
    rTrim(str, '0');
    /* Remove the decimal point if it happens to be an integer */
    rTrim(str, '.');
    return str;
}

/* Duplicates the string (src) and returns it. It is created on the heap. The caller is responsible for freeing the allocated memory. */
char* emb_strdup(const char* src)
{
    char* dest = (char*)malloc(strlen(src) + 1);
    /* TODO: malloc fail error */
    if(dest) strcpy(dest, src);
    return dest;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
