#include "format-tap.h"
#include <stdio.h>

int decodeRecordFlags(unsigned char b2)
{
    if(b2 == 0xF3)
        return END;
    switch(b2 & 0xC3)
    {
        case 0x03:
            return NORMAL;
        case 0x83:
            return TRIM;
        case 0xC3:
            return STOP;
        default:
            return NORMAL;
    }
}

int readTap(EmbPattern* pattern, const char* fileName)
{
    unsigned char b[3];
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

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
        flags = decodeRecordFlags(b[2]);
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, flags, 1);
        if(flags == END)
            break;
    }
    fclose(file);
    return 1;
}

int writeTap(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WriteTap */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
