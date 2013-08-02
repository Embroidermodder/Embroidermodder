#include <stdio.h>
#include "format-pec.h"
#include "format-phc.h"
#include "helpers-binary.h"

int readPhc(EmbPattern* pattern, const char* fileName)
{
    int colorChanges, version, fileLength, bytesInSection2;
    unsigned short pecOffset, bytesInSection, bytesInSection3;
    char pecAdd;
    FILE* file = fopen(fileName, "rb");
    int i;

    if(file == 0)
    {
        return 0;
    }
    fseek(file, 0x07, SEEK_SET);
    version = binaryReadByte(file) - 0x30; /* converting from ansi number */
    fseek(file, 0x4D, SEEK_SET);
    colorChanges = binaryReadUInt16(file);

    for(i = 0; i < colorChanges; i++)
    {
        EmbThread t = pecThreads[binaryReadByte(file)];
        embPattern_addThread(pattern, t);
    }
    fseek(file, 0x2B, SEEK_SET);
    pecAdd = binaryReadByte(file);
    fileLength = binaryReadUInt32(file);
    pecOffset = binaryReadUInt16(file);
    fseek(file, pecOffset + pecAdd, SEEK_SET);
    bytesInSection = binaryReadUInt16(file);
    fseek(file, bytesInSection, SEEK_CUR);
    bytesInSection2 = binaryReadUInt32(file);
    fseek(file, bytesInSection2, SEEK_CUR);
    bytesInSection3 = binaryReadUInt16(file);
    fseek(file, bytesInSection3 + 0x12, SEEK_CUR);

    readPecStitches(pattern, file);

    embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
    fclose(file);
    embPattern_flipVertical(pattern);
    return 1; /*TODO: finish ReadPhc */
}

int writePhc(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WritePhc */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
