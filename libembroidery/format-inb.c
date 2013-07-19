#include <stdio.h>
#include "format-inb.h"
#include "helpers-binary.h"

int readInb(EmbPattern* pattern, const char* fileName)
{
    unsigned char fileDescription[8];
    unsigned char nullVal;
    int stitchCount;
    short width;
    short height;
    short colorCount;
    short unknown3;
    short unknown2;
    short imageWidth;
    short imageHeight;
    unsigned char bytesUnknown[300];
    short nullbyte;
    short left;
    short right;
    short top;
    short bottom;
    int x = 0;
    int y = 0;
    int i;

    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);
    binaryReadBytes(file, fileDescription, 8);
    nullVal = binaryReadByte(file);
    binaryReadInt16(file);
    stitchCount = binaryReadInt32(file);
    width = binaryReadInt16(file);
    height = binaryReadInt16(file);
    colorCount = binaryReadInt16(file);
    unknown3 = binaryReadInt16(file);
    unknown2 = binaryReadInt16(file);
    imageWidth = binaryReadInt16(file);
    imageHeight = binaryReadInt16(file);
    binaryReadBytes(file, bytesUnknown, 300);
    nullbyte = binaryReadInt16(file);
    left = binaryReadInt16(file);
    right = binaryReadInt16(file);
    top = binaryReadInt16(file);
    bottom = binaryReadInt16(file);
    fseek(file, 0x2000, SEEK_SET);

    for(i = 0; i < stitchCount; i++)
    {
        unsigned char type;
        int stitch = NORMAL;
        x = binaryReadByte(file);
        y = binaryReadByte(file);
        type = binaryReadByte(file);
        if((type & 0x40) > 0)
            x = -x;
        if((type & 0x10) > 0)
            y = -y;
        if((type & 1) > 0)
            stitch = STOP;
        if((type & 2) > 0)
            stitch = TRIM;
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, stitch, 1);
    }
    fclose(file);
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    embPattern_flipVertical(pattern);
    return 1;
}

int writeInb(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeInb */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
