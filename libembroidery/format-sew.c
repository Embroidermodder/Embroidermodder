#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "format-sew.h"
#include "helpers-binary.h"
#include "format-jef.h"

char sewDecode(unsigned char inputByte)
{
    return (inputByte >= 0x80) ? (char) (-~(inputByte - 1)) : (char) inputByte;
}

int readSew(EmbPattern* pattern, const char* fileName)
{
    int i;
    int fileLength;
    char dx = 0, dy = 0;
    int flags;
    int numberOfColors;
    char thisStitchIsJump = 0;

    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    fseek(file, 0x00, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0x00, SEEK_SET);
    numberOfColors = binaryReadByte(file);
    numberOfColors += (binaryReadByte(file) << 8);

    for(i = 0; i < numberOfColors; i++)
    {
        embPattern_addThread(pattern, jefThreads[binaryReadInt16(file)]);
    }
    fseek(file, 0x1D78, SEEK_SET);

    for(i = 0; ftell(file) < fileLength; i++)
    {
        unsigned char b0 = binaryReadByte(file);
        unsigned char b1 = binaryReadByte(file);

        flags = NORMAL;
        if(thisStitchIsJump)
        {
            flags = TRIM;
            thisStitchIsJump = 0;
        }
        if(b0 == 0x80)
        {
            if(b1 == 1)
            {
                b0 = binaryReadByte(file);
                b1 = binaryReadByte(file);
                flags = STOP;
            }
            else if((b1 == 0x02) || (b1 == 0x04))
            {
                thisStitchIsJump = 1;
                b0 = binaryReadByte(file);
                b1 = binaryReadByte(file);
                flags = TRIM;
            }
            else if(b1 == 0x10)
            {
               break;
            }
        }
        dx = sewDecode(b0);
        dy = sewDecode(b1);
        if(abs(dx) == 127 || abs(dy) == 127)
        {
            thisStitchIsJump = 1;
            flags = TRIM;
        }

        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    printf("current position: %ld\n", ftell(file));
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);
    return 1;
}

int writeSew(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeSew */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
