#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format-ofm.h"
#include "helpers-binary.h"

static char *ReadLibrary(FILE *file)
{
    int stringLength = 0;
    char *libraryName = 0;
    /* FF FE FF */
    unsigned char leadin[3];
    binaryReadBytes(file, leadin, 3);
    stringLength = binaryReadByte(file);
    libraryName = (char *)malloc(sizeof(char) * stringLength * 2);
    binaryReadBytes(file, (unsigned char *)libraryName, stringLength * 2);
    return libraryName;
}

static int ReadClass(FILE *file)
{
    int len;
    char *s;
    binaryReadInt16(file);
    len = binaryReadInt16(file);

    s = (char *)malloc(sizeof(char) * len + 1);
    binaryReadBytes(file, (unsigned char *)s, len);
    s[len] = '\0';
    if(strcmp(s, "CExpStitch") == 0)
            return 0x809C;
    if(strcmp(s, "CColorChange") == 0)
            return 0xFFFF;
    return 0;
}

static void ReadBlockHeader(FILE *file)
{
    int val1, val2, val3, val4, val5, val6, val7, val8, val9, val10;
    unsigned char len;
    char *s;
    unsigned short short1;
    short unknown1 = binaryReadInt16(file);
    short unknown2 = (short)binaryReadInt32(file);
    int unknown3 = binaryReadInt32(file);
    /* int v = binaryReadBytes(3); TODO: review */
    binaryReadInt16(file);
    binaryReadByte(file);
    len = binaryReadByte(file);
    s = (char *)malloc(2 * len);
    binaryReadBytes(file, (unsigned char *)s, 2 * len);
    val1 = binaryReadInt32(file);   /*  0 */
    val2 = binaryReadInt32(file);   /*  0 */
    val3 = binaryReadInt32(file);   /*  0 */
    val4 = binaryReadInt32(file);   /*  0 */
    val5 = binaryReadInt32(file);   /*  1 */
    val6 = binaryReadInt32(file);   /*  1 */
    val7 = binaryReadInt32(file);   /*  1 */
    val8 = binaryReadInt32(file);   /*  0 */
    val9 = binaryReadInt32(file);   /* 64 */
    val10 = binaryReadInt32(file);  /* 64 */
    short1 = binaryReadInt16(file); /*  0 */
}

static void ReadColorChange(FILE *file, EmbPattern *pattern)
{
    ReadBlockHeader(file);
    embPattern_addStitchRel(pattern, 0.0, 0.0, STOP, 1);
}

static void ReadThreads(FILE *file, EmbPattern *p)
{
    int i, numberOfColors, stringLen, numberOfLibraries;
    char *primaryLibraryName, *expandedString;
    /* FF FE FF 00 */
    binaryReadInt32(file);

    numberOfColors = binaryReadInt16(file);

    binaryReadInt16(file);
    binaryReadInt16(file);
    stringLen = binaryReadInt16(file);
    expandedString = (char *)malloc(stringLen);
    binaryReadBytes(file, (unsigned char *)expandedString, stringLen);
    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread thread;
        char colorNumberText[10];
        int threadLibrary = 0, colorNameLength, colorNumber;
        char *colorName;
        int r = binaryReadByte(file);
        int g = binaryReadByte(file);
        int b = binaryReadByte(file);
        binaryReadByte(file);
        threadLibrary = binaryReadInt16(file);
        binaryReadInt16(file);
        colorNumber = binaryReadInt32(file);
        binaryReadByte(file);
        binaryReadInt16(file);
        colorNameLength = binaryReadByte(file);
        colorName = (char *)malloc(colorNameLength * 2);
        binaryReadBytes(file, (unsigned char *)colorName, colorNameLength*2);
        binaryReadInt16(file);
     /* itoa(colorNumber, colorNumberText, 10); TODO: never use itoa, it's non-standard, use sprintf: http://stackoverflow.com/questions/5242524/converting-int-to-string-in-c */
        thread.catalogNumber = colorNumberText;
        thread.description = colorName;
        thread.color.r = (unsigned char)r;
        thread.color.g = (unsigned char)g;
        thread.color.b = (unsigned char)b;
        embPattern_addThread(p, thread);
    }
    binaryReadInt16(file);
    primaryLibraryName = ReadLibrary(file);
    numberOfLibraries = binaryReadInt16(file);
    for(i = 0; i < numberOfLibraries; i++)
    {
        /*libraries.Add( TODO: review */
        char *libName = ReadLibrary(file);
        free(libName);
    }
}

static double Decode(unsigned char b1, unsigned char b2)
{
    double val = (double)(short)(b1 << 8 | b2);
    return val;
}

static void ReadExpanded(FILE *file, EmbPattern *p)
{
    int i, numberOfStitches = 0;
    ReadBlockHeader(file);
    numberOfStitches = binaryReadInt32(file);

    for(i = 0; i < numberOfStitches; i++)
    {
        unsigned char stitch[5];
        binaryReadBytes(file, stitch, 5);
        if(stitch[0] == 0)
        {
            embPattern_addStitchAbs(p, Decode(stitch[1], stitch[2]) / 10.0, Decode(stitch[3], stitch[4]) / 10.0, i == 0 ? JUMP : NORMAL, 1);
        }
    }
}

int readOfm(EmbPattern* pattern, const char* fileName)
{
    int unknownCount = 0;
    int key = 0, classNameLength;
    char *s;
    FILE* file = fopen(fileName, "rb");
    if(!file)
    {
        return 0;
    }
    fseek(file, 0x1C6, SEEK_SET);
    ReadThreads(file, pattern);
    fseek(file, 0x110, SEEK_CUR);
    binaryReadInt32(file);
    classNameLength = binaryReadInt16(file);
    s = (char *)malloc(sizeof(char) * classNameLength);
    binaryReadBytes(file, (unsigned char *)s, classNameLength);
    unknownCount = binaryReadInt16(file); /* unknown count */

    binaryReadInt16(file);
    key = ReadClass(file);
    while(1 == 1)
    {
        if(key == 0xFEFF)
        {
            break;
        }
        if(key == 0x809C)
        {
            ReadExpanded(file, pattern);
        }
        else
        {
            ReadColorChange(file, pattern);
        }
        key = binaryReadUInt16(file);
        if(key == 0xFFFF) ReadClass(file);
    }
    embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
    return 1;
}

int writeOfm(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeOfm */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
