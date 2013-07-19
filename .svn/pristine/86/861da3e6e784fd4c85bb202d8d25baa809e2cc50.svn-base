#include <stdio.h>
#include "format-fxy.h"
#include "helpers-binary.h"

int readFxy(EmbPattern* pattern, const char* fileName)
{
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        /*TODO: set messages here "Error opening FXY file for read:" */
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

    fseek(file, 0x100, SEEK_SET);

    while(1)
    {
        int stitchType = NORMAL;
        int b1 = binaryReadByte(file);
        int b2 = binaryReadByte(file);
        unsigned char commandByte = binaryReadByte(file);
        if(commandByte == 0x91)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        if((commandByte & 0x01) == 0x01)
            stitchType = TRIM;
        if((commandByte & 0x02) == 0x02)
            stitchType = STOP;
        if((commandByte & 0x20) == 0x20)
            b1 = -b1;
        if((commandByte & 0x40) == 0x40)
            b2 = -b2;
        embPattern_addStitchRel(pattern, b2 / 10.0, b1 / 10.0, stitchType, 1);
    }
    return 1;
}

int writeFxy(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WriteFxy */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
