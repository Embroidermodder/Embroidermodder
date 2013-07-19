#include "format-u00.h"
#include <stdio.h>
#include "helpers-binary.h"

int readU00(EmbPattern* pattern, const char* fileName)
{
    int i;
    char dx = 0, dy = 0;
    int flags = NORMAL;
    char endOfStream = 0;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    /* 16 3byte RGB's start @ 0x08 followed by 14 bytes between 0 and 15 with index of color for each color change */
    fseek(file, 0x08, SEEK_SET);

    for(i = 0; i < 16; i++)
    {
        EmbThread t;
        t.color.r = binaryReadUInt8(file);
        t.color.g = binaryReadUInt8(file);
        t.color.b = binaryReadUInt8(file);
        embPattern_addThread(pattern, t);
    }

    fseek(file, 0x100, SEEK_SET);
    for(i = 0; !endOfStream; i++)
    {
        char negativeX , negativeY;
        unsigned char b0 = binaryReadUInt8(file);
        unsigned char b1 = binaryReadUInt8(file);
        unsigned char b2 = binaryReadUInt8(file);

        if(b0 == 0xF8 || b0 == 0x87 || b0 == 0x91)
        {
            break;
        }
        if((b0 & 0x0F) == 0)
        {
            flags = NORMAL;
        }
        else if((b0 & 0x1f) == 1)
        {
            flags = JUMP;
        }
        else if((b0 & 0x0F) > 0)
        {
            flags = STOP;
        }
        negativeX = ((b0 & 0x20) > 0);
        negativeY = ((b0 & 0x40) > 0);

        dx = (char) b2;
        dy = (char) b1;
        if(negativeX) dx = (char) -dx;
        if(negativeY) dy = (char) -dy;
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    flags = END;
    embPattern_addStitchRel(pattern, 0.0, 0.0, flags, 1);

    fclose(file);
    return 1;
}


int writeU00(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WriteU00 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
