#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "embroidery.h"

char binaryReadByte(EmbFile* file)
{
    return (char)embFile_getc(file);
}

int binaryReadBytes(EmbFile* file, unsigned char* destination, int count)
{
    return (int) embFile_read((char*) destination, 1, count, file);
}

short binaryReadInt16(EmbFile* file)
{
    int x = embFile_getc(file);
    x = x | embFile_getc(file) << 8;
    return (short)x;
}

int binaryReadInt32(EmbFile* file)
{
    int x = embFile_getc(file);
    x = x | embFile_getc(file) << 8;
    x = x | embFile_getc(file) << 16;
    x = x | embFile_getc(file) << 24;
    return x;
}

unsigned char binaryReadUInt8(EmbFile* file)
{
    return (unsigned char)embFile_getc(file);
}

unsigned short binaryReadUInt16(EmbFile* file)
{
    return (unsigned short)(embFile_getc(file) | embFile_getc(file) << 8);
}

unsigned int binaryReadUInt32(EmbFile* file)
{
    unsigned int x = embFile_getc(file);
    x = x | embFile_getc(file) << 8;
    x = x | embFile_getc(file) << 16;
    x = x | embFile_getc(file) << 24;
    return x;
}

/* Big endian version */
short binaryReadInt16BE(EmbFile* file)
{
    short returnValue = (short)(embFile_getc(file) << 8);
    returnValue |= embFile_getc(file);
    return returnValue;
}

/* Big endian version */
unsigned short binaryReadUInt16BE(EmbFile* file)
{
    unsigned short returnValue = (unsigned short)(embFile_getc(file) << 8);
    returnValue |= embFile_getc(file);
    return returnValue;
}

/* Big endian version */
int binaryReadInt32BE(EmbFile* file)
{
    int returnValue = embFile_getc(file) << 24;
    returnValue |= embFile_getc(file) << 16;
    returnValue |= embFile_getc(file) << 8;
    returnValue |= embFile_getc(file);
    return (returnValue);
}

/* Big endian version */
unsigned int binaryReadUInt32BE(EmbFile* file)
{
    unsigned int returnValue = embFile_getc(file) << 24;
    returnValue |= embFile_getc(file) << 16;
    returnValue |= embFile_getc(file) << 8;
    returnValue |= embFile_getc(file);
    return returnValue;
}

void binaryReadString(EmbFile* file, char* buffer, int maxLength)
{
    int i = 0;
    while(i < maxLength)
    {
        buffer[i] = (char)embFile_getc(file);
        if(buffer[i] == '\0') break;
        i++;
    }
}

void binaryReadUnicodeString(EmbFile* file, char *buffer, const int stringLength)
{
    int i = 0;
    for(i = 0; i < stringLength * 2; i++)
    {
        char input = (char)embFile_getc(file);
        if(input != 0)
        {
            buffer[i] = input;
        }
    }
}

float binaryReadFloat(EmbFile* file)
{
    union
    {
        float f32;
        unsigned int u32;
    } float_int_u;
    float_int_u.u32 = embFile_getc(file);
    float_int_u.u32 |= embFile_getc(file) << 8;
    float_int_u.u32 |= embFile_getc(file) << 16;
    float_int_u.u32 |= embFile_getc(file) << 24;
    return float_int_u.f32;
}

void binaryWriteByte(EmbFile* file, unsigned char data)
{
    embFile_putc(data, file);
}

void binaryWriteBytes(EmbFile* file, const char* data, int size)
{
    embFile_write((char*)data, 1, size, file);
}

void binaryWriteShort(EmbFile* file, short data)
{
    embFile_putc(data & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
}

void binaryWriteShortBE(EmbFile* file, short data)
{
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc(data & 0xFF, file);
}

void binaryWriteUShort(EmbFile* file, unsigned short data)
{
    embFile_putc(data & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
}

void binaryWriteUShortBE(EmbFile* file, unsigned short data)
{
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc(data & 0xFF, file);
}

void binaryWriteInt(EmbFile* file, int data)
{
    embFile_putc(data & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc((data >> 16) & 0xFF, file);
    embFile_putc((data >> 24) & 0xFF, file);
}

void binaryWriteIntBE(EmbFile* file, int data)
{
    embFile_putc((data >> 24) & 0xFF, file);
    embFile_putc((data >> 16) & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc(data & 0xFF, file);
}

void binaryWriteUInt(EmbFile* file, unsigned int data)
{
    embFile_putc(data & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc((data >> 16) & 0xFF, file);
    embFile_putc((data >> 24) & 0xFF, file);
}

void binaryWriteUIntBE(EmbFile* file, unsigned int data)
{
    embFile_putc((data >> 24) & 0xFF, file);
    embFile_putc((data >> 16) & 0xFF, file);
    embFile_putc((data >> 8) & 0xFF, file);
    embFile_putc(data & 0xFF, file);
}

void binaryWriteFloat(EmbFile* file, float data)
{
    union
    {
        float f32;
        unsigned int u32;
    } float_int_u;
    float_int_u.f32 = data;

    embFile_putc(float_int_u.u32 & 0xFF, file);
    embFile_putc((float_int_u.u32 >> 8) & 0xFF, file);
    embFile_putc((float_int_u.u32 >> 16) & 0xFF, file);
    embFile_putc((float_int_u.u32 >> 24) & 0xFF, file);
}

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
