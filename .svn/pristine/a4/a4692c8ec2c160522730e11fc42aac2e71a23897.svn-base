#include "format-shv.h"
#include <stdio.h>
#include <string.h>
#include "format-jef.h"
#include "helpers-binary.h"

char shvDecode(unsigned char inputByte)
{
    if(inputByte >= 0x80)
    {
        return (char)-((unsigned char)((~inputByte) + 1));
    }
    return ((char)inputByte);
}

short shvDecodeShort(unsigned short inputByte)
{
    if(inputByte > 0x8000)
    {
        return (short)-((unsigned short)((~inputByte) + 1));
    }
    return ((short)inputByte);
}

int readShv(EmbPattern* pattern, const char* fileName)
{
    int i, currentColor = 0;
    char inJump = 0;
    unsigned char fileNameLength, designWidth, designHeight;
    char halfDesignWidth, halfDesignHeight, halfDesignWidth2, halfDesignHeight2;
    char* headerText = "Embroidery disk created using software licensed from Viking Sewing Machines AB, Sweden";
    char dx = 0, dy = 0;
    char numberOfColors;
    unsigned short magicCode;
    int something;
    short left,top,right,bottom;

    char something2,numberOfSections, something3 ;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }

    fseek(file, strlen(headerText), SEEK_SET);
    fileNameLength = binaryReadUInt8(file);
    fseek(file, fileNameLength, SEEK_CUR);
    designWidth = binaryReadUInt8(file);
    designHeight = binaryReadUInt8(file);
    halfDesignWidth = binaryReadUInt8(file);
    halfDesignHeight = binaryReadUInt8(file);
    halfDesignWidth2 = binaryReadUInt8(file);
    halfDesignHeight2 = binaryReadUInt8(file);
    if((designHeight % 2) == 1)
        fseek(file, ((designHeight + 1)*designWidth)/2, SEEK_CUR);
    else
        fseek(file, (designHeight*designWidth)/2, SEEK_CUR);

    numberOfColors = binaryReadUInt8(file);
    magicCode = binaryReadUInt16(file);
    binaryReadByte(file);
    something = binaryReadInt32(file);
    left = binaryReadInt16(file);
    top = binaryReadInt16(file);
    right = binaryReadInt16(file);
    bottom = binaryReadInt16(file);

    something2 = binaryReadByte(file);
    numberOfSections = binaryReadUInt8(file);
    something3 = binaryReadByte(file);

    for(i = 0; i < numberOfColors; i++)
    {
        unsigned char colorNumber;
        binaryReadUInt16(file);
        binaryReadUInt16(file);
        colorNumber = binaryReadUInt8(file);
        embPattern_addThread(pattern, jefThreads[colorNumber]);
        fseek(file, 9, SEEK_CUR);
    }

    fseek(file, -2, SEEK_CUR);
    for(i = 0; !feof(file); i++)
    {
        unsigned char b0, b1;
        int flags;
        if(inJump)
            flags = JUMP;
        else
            flags = NORMAL;

        b0 = binaryReadUInt8(file);
        b1 = binaryReadUInt8(file);
        if(b0 == 0 && b1 == 0 && i > 0) flags = STOP;
        if(b0 == 0x80)
        {
            if(b1 == 3)
            {
                /*embPattern_addStitchRel(pattern, 0, 0, STOP); TODO: review this */
                continue;
            }
            else if(b1 == 0x02)
            {
                inJump = 0;
                continue;
            }
            else if(b1 == 0x01)
            {
                unsigned short sx, sy;
                sx = binaryReadUInt8(file);
                sx = (unsigned short)(sx << 8 | binaryReadUInt8(file));
                sy = binaryReadUInt8(file);
                sy = (unsigned short)(sy << 8 | binaryReadUInt8(file));
                flags = TRIM;
                inJump = 1;
                embPattern_addStitchRel(pattern, shvDecodeShort(sx) / 10.0, shvDecodeShort(sy) / 10.0, flags, 1);
                continue;
            }
        }
        dx = shvDecode(b0);
        dy = shvDecode(b1);
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    embPattern_flipVertical(pattern);

    fclose(file);
    return 1;
}


int writeShv(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeShv */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
