#include <stdio.h>
#include "format-exy.h"

int exyDecodeFlags(unsigned char b2)
{
    int returnCode = 0;
    if(b2 == 0xF3)
        return (END);
    if((b2 & 0xC3) == 0xC3)
        return TRIM | STOP;
    if(b2 & 0x80)
        returnCode |= TRIM;
    if(b2 & 0x40)
        returnCode |= STOP;
    return returnCode;
}

int readExy(EmbPattern* pattern, const char* fileName)
{
    unsigned char b[3];
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        /*TODO: set status here "Error opening EXY file for read:" */
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);

    fseek(file, 0x100, SEEK_SET);

    while(fread(b, 1, 3, file) == 3)
    {
        int flags;
        int x = 0;
        int y = 0;
        if(b[0] & 0x01)
            x += 1;
        if(b[0] & 0x02)
            x -= 1;
        if(b[0] & 0x04)
            x += 9;
        if(b[0] & 0x08)
            x -= 9;
        if(b[0] & 0x80)
            y += 1;
        if(b[0] & 0x40)
            y -= 1;
        if(b[0] & 0x20)
            y += 9;
        if(b[0] & 0x10)
            y -= 9;
        if(b[1] & 0x01)
            x += 3;
        if(b[1] & 0x02)
            x -= 3;
        if(b[1] & 0x04)
            x += 27;
        if(b[1] & 0x08)
            x -= 27;
        if(b[1] & 0x80)
            y += 3;
        if(b[1] & 0x40)
            y -= 3;
        if(b[1] & 0x20)
            y += 27;
        if(b[1] & 0x10)
            y -= 27;
        if(b[2] & 0x04)
            x += 81;
        if(b[2] & 0x08)
            x -= 81;
        if(b[2] & 0x20)
            y += 81;
        if(b[2] & 0x10)
            y -= 81;
        flags = exyDecodeFlags(b[2]);
        if((flags & END) == END)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, flags, 1);
    }
    fclose(file);
    return 1;
}

int writeExy(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeExy */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
