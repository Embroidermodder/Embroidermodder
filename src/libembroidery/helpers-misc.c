#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "helpers-misc.h"
#include "emb-logging.h"

/*! Rounds a double (\a src) and returns it as an \c int. */
int roundDouble(double src)
{
    if(src < 0.0)
        return (int) ceil(src - 0.5);
    return (int)floor(src+0.5);
}

/*! Returns \c true if string (\a str) begins with substring (\a pre), otherwise returns \c false. */
char startsWith(const char* pre, const char* str)
{
    char result = 0;
    size_t lenpre;
    size_t lenstr;
    if(!pre) { embLog_error("helpers-misc.c startsWith(), pre argument is null\n"); return 0; }
    if(!str) { embLog_error("helpers-misc.c startsWith(), str argument is null\n"); return 0; }
    lenpre = strlen(pre);
    lenstr = strlen(str);
    if(lenstr < lenpre)
        return 0;
    result = (char)strncmp(pre, str, lenpre);
    if(result == 0)
        return 1;
    return 0;
}

/*! Removes all characters from the right end of the string (\a str) that match (\a junk), moving left until a mismatch occurs. */
char* rTrim(char* const str, char junk)
{
    char* original = str + strlen(str);
    while(*--original == junk);
    *(original + 1) = '\0';
    return str;
}

/*! Removes all characters from the left end of the string (\a str) that match (\a junk), moving right until a mismatch occurs. */
char* lTrim(char* const str, char junk)
{
    char* original = str;
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
    return str;
}

/* TODO: trimming function should handle any character, not just whitespace */
static char const WHITESPACE[] = " \t\n\r";

/* TODO: description */
static void get_trim_bounds(char const *s,
                            char const **firstWord,
                            char const **trailingSpace)
{
    char const* lastWord = 0;
    *firstWord = lastWord = s + strspn(s, WHITESPACE);
    do
    {
        *trailingSpace = lastWord + strcspn(lastWord, WHITESPACE);
        lastWord = *trailingSpace + strspn(*trailingSpace, WHITESPACE);
    }
    while (*lastWord != '\0');
}

/* TODO: description */
char* copy_trim(char const *s)
{
    char const *firstWord = 0, *trailingSpace = 0;
    char* result = 0;
    size_t newLength;

    get_trim_bounds(s, &firstWord, &trailingSpace);
    newLength = trailingSpace - firstWord;

    result = (char*)malloc(newLength + 1);
    memcpy(result, firstWord, newLength);
    result[newLength] = '\0';
    return result;
}

/* TODO: description */
void inplace_trim(char* s)
{
    char const *firstWord = 0, *trailingSpace = 0;
    size_t newLength;

    get_trim_bounds(s, &firstWord, &trailingSpace);
    newLength = trailingSpace - firstWord;

    memmove(s, firstWord, newLength);
    s[newLength] = '\0';
}

/*! Optimizes the number (\a num) for output to a text file and returns it as a string (\a str). */
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

/*! Duplicates the string (\a src) and returns it. It is created on the heap. The caller is responsible for freeing the allocated memory. */
char* emb_strdup(const char* src)
{
    char* dest = 0;
    if(!src) { embLog_error("helpers-misc.c emb_strdup(), src argument is null\n"); return 0; }
    dest = (char*)malloc(strlen(src) + 1);
    if(!dest) { embLog_error("helpers-misc.c emb_strdup(), cannot allocate memory\n"); }
    else { strcpy(dest, src); }
    return dest;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
