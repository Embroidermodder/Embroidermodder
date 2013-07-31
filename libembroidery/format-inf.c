#include "format-inf.h"
#include <stdio.h>
#include <stdlib.h>
#include "helpers-binary.h"
#include <string.h>

int readInf(EmbPattern* pattern, const char* fileName)
{
    int numberOfColors;
    int i;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }

    binaryReadUInt32BE(file);
    binaryReadUInt32BE(file);
    binaryReadUInt32BE(file);
    numberOfColors = binaryReadUInt32BE(file);
    embThread_free(pattern->threadList);
    for(i = 0; i < numberOfColors; i++)
    {
        char colorType[50];
        char colorDescription[50];
        EmbThread t;
        int pos;
        binaryReadUInt16(file);
        binaryReadUInt16(file);
        t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        t.color.b = binaryReadByte(file);
        embPattern_addThread(pattern, t);
        binaryReadUInt16(file);
        binaryReadString(file, colorType, 50);
        binaryReadString(file, colorDescription, 50);
    }
    fclose(file);
    return 1;
}

int writeInf(EmbPattern* pattern, const char* fileName)
{
    EmbThreadList* pointer;
    int i = 1, bytesRemaining;
    FILE* file = fopen(fileName, "wb");
    if(file == 0)
    {
        return 0;
    }
    binaryWriteUIntBE(file, 0x01);
    binaryWriteUIntBE(file, 0x08);
    /* write place holder offset */
    binaryWriteUIntBE(file, 0x00);
    binaryWriteUIntBE(file, embThread_count(pattern->threadList));

    pointer = pattern->threadList;
    while(pointer)
    {
        char buffer[50];
        EmbColor c;
        c = pointer->thread.color;
        sprintf(buffer, "RGB(%d,%d,%d)", c.r, c.g, c.b);
        binaryWriteUShortBE(file, 14 + strlen(buffer)); /* record length */
        binaryWriteUShortBE(file, i); /* record number */
        binaryWriteByte(file, c.r);
        binaryWriteByte(file, c.g);
        binaryWriteByte(file, c.b);
        binaryWriteUShortBE(file, i); /* needle number */
        binaryWriteBytes(file, "RGB\0", 4);
        fprintf(file, buffer);
        binaryWriteByte(file, 0);
        pointer = pointer->next;
        i++;
    }
    fseek(file, -8, SEEK_END);
    bytesRemaining = ftell(file);
    fseek(file, 8, SEEK_SET);
    binaryWriteUIntBE(file, bytesRemaining);
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
