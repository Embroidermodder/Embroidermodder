#include <stdio.h>
#include "format-phb.h"
#include "format-pec.h"
#include "helpers-binary.h"

int readPhb(EmbPattern* pattern, const char* fileName)
{
    unsigned int fileOffset;
    short colorCount;
    FILE* file = fopen(fileName, "rb");
    int i;

    if(file == 0)
    {
        return 0;
    }
    fseek(file, 0x71, SEEK_SET);
    colorCount = binaryReadInt16(file);

    for(i = 0; i < colorCount; i++)
    {
        EmbThread t = pecThreads[binaryReadByte(file)];
        embPattern_addThread(pattern, t);
    }

    /* TODO: check that file begins with #PHB */
    fseek(file, 0x54, SEEK_SET);
    fileOffset = 0x52;
    fileOffset += binaryReadUInt32(file);

    fseek(file, fileOffset, SEEK_SET);
    fileOffset += binaryReadUInt32(file) + 2;

    fseek(file, fileOffset, SEEK_SET);
    fileOffset += binaryReadUInt32(file);

    fseek(file, fileOffset + 14, SEEK_SET); /* 28 */

    colorCount = (short)binaryReadByte(file);
    for(i = 0; i< colorCount; i++)
    {
        binaryReadByte(file);
    }
    binaryReadInt32(file); /* bytes to end of file */
    binaryReadInt32(file);
    binaryReadByte(file);

    binaryReadInt16(file);
    binaryReadInt16(file);
    binaryReadInt16(file);
    binaryReadInt16(file);
    binaryReadInt16(file);
    binaryReadInt16(file);
    readPecStitches(pattern, file);

    embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
    fclose(file);
    embPattern_flipVertical(pattern);
    return 1; /*TODO: finish ReadPhb */
}

int writePhb(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WritePhb */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
