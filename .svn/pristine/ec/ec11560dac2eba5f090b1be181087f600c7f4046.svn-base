#include "format-rgb.h"
#include <stdio.h>
#include <stdlib.h>
#include "helpers-binary.h"

int readRgb(EmbPattern* pattern, const char* fileName)
{
    int i, numberOfColors;
    EmbThreadList *colors;

    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    fseek(file, 0x00, SEEK_END);
    numberOfColors = ftell(file) / 4;

    colors = pattern->threadList;
    while(colors)
    {
        EmbThreadList* next = colors->next;
        free(colors);
        colors = next;
    }
    pattern->threadList = NULL;

    fseek(file, 0x00, SEEK_SET);
    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread t;
        t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        t.color.b = binaryReadByte(file);
        binaryReadByte(file);
        embPattern_addThread(pattern, t);
    }
    fclose(file);
    return 1;
}

int writeRgb(EmbPattern* pattern, const char* fileName)
{
    EmbThreadList* colors = pattern->threadList;
    EmbColor c;
    FILE* file = fopen(fileName, "wb");

    if(file == 0)
    {
        return 0;
    }

    while(colors)
    {
        c = colors->thread.color;
        binaryWriteByte(file, c.r);
        binaryWriteByte(file, c.g);
        binaryWriteByte(file, c.b);
        binaryWriteByte(file, 0);
        colors = colors->next;
    }
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
