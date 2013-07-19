#include "format-pes.h"
#include "format-pec.h"
#include "helpers-binary.h"

short pecDecodeNormal(unsigned char inputByte)
{
    if(inputByte < 0x40)
    {
        return inputByte;
    }
    return (inputByte - 0x80);
}

int pecJumpDecode(unsigned char byte1, unsigned char byte2)
{
    unsigned n1 = (unsigned char) (byte1 & 0x0F);
    if(n1 <= 7)
    {
        int returnValue = (n1 << 8) + byte2;
        return returnValue;
    }
    else
    {
        int returnValue = -((256 - byte2) + ((15 - n1) << 8));
        return returnValue;
    }
}

int readPes(EmbPattern* pattern, const char* fileName)
{
    int pecstart, numColors, x;
    FILE* file = fopen(fileName, "rb");
    fseek(file, 8, SEEK_SET);
    pecstart = binaryReadInt32(file);

    fseek(file, pecstart + 48, SEEK_SET);
    numColors = fgetc(file) + 1;
    for(x = 0; x < numColors; x++)
    {
        embPattern_addThread(pattern, pecThreads[(unsigned char) fgetc(file)]);
    }

    fseek(file, pecstart + 532, SEEK_SET);
    readPecStitches(pattern, file);
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    embPattern_flipVertical(pattern);
    fclose(file);
    return 1;
}

void writeSewSegSection(EmbPattern* pattern, FILE *file)
{
    EmbStitchList* pointer;
    binaryWriteShort(file, 0x06); /* section count? */
    binaryWriteUShort(file, 0xFFFF);
    binaryWriteShort(file, 0x00);

    binaryWriteShort(file, 0x07); /* string length */
    binaryWriteBytes(file, "CSewSeg", 7);
    binaryWriteShort(file, 0x01);
    binaryWriteShort(file, 0x01);
    binaryWriteShort(file, 0x02);
    binaryWriteShort(file, 0x00);
    pointer = pattern->stitchList;
    while(pointer)
    {
        EmbStitch s = pointer->stitch;
        binaryWriteShort(file, (short)s.xx);
        binaryWriteShort(file, (short)s.yy);
        pointer = pointer->next;
    }
}
void writeEmbOneSection(EmbPattern* pattern, FILE *file)
{
    int i;
    EmbRect bounds;
    binaryWriteShort(file, 0x07); /* string length */
    binaryWriteBytes(file, "CEmbOne", 7);
    bounds = embPattern_calcBoundingBox(pattern);

    binaryWriteShort(file, (short)bounds.left);
    binaryWriteShort(file, (short)bounds.top);
    binaryWriteShort(file, (short)bounds.right);
    binaryWriteShort(file, (short)bounds.bottom);

    binaryWriteShort(file, (short)bounds.left);
    binaryWriteShort(file, (short)bounds.top);
    binaryWriteShort(file, (short)bounds.right);
    binaryWriteShort(file, (short)bounds.bottom);

    /* AffineTransform */
    binaryWriteFloat(file, 1.0f);
    binaryWriteFloat(file, 0.0f);
    binaryWriteFloat(file, 0.0f);
    binaryWriteFloat(file, 1.0f);
    binaryWriteFloat(file, 0.0f);
    binaryWriteFloat(file, 0.0f);

    binaryWriteShort(file, 1);
    binaryWriteShort(file, 0); /* Translate X */
    binaryWriteShort(file, 0); /* Translate Y */
    binaryWriteShort(file, (short)embRect_width(bounds));
    binaryWriteShort(file, (short)embRect_height(bounds));

    for(i = 0; i < 8; i++)
    {
        binaryWriteByte(file, 0);
    }

    /*WriteSubObjects(br, pes, SubBlocks); */
}

void writePaletteSection(EmbPattern* pattern, FILE* file)
{
    binaryWriteInt(file, embThread_count(pattern->threadList));
}

int writePes(EmbPattern* pattern, const char* fileName)
{
    int pecLocation;
    FILE* file = fopen(fileName, "wb");
    if(file == 0)
    {
        return 0;
    }
    embPattern_flipVertical(pattern);
    embPattern_scale(pattern, 10.0);
    binaryWriteBytes(file, "#PES0001", 8);
    /* WRITE PECPointer 32 bit int */
    binaryWriteInt(file, 0x00);

    binaryWriteShort(file, 0x00);
    binaryWriteShort(file, 0x01);

    /* Write object count */
    binaryWriteShort(file, 0x01);
    binaryWriteShort(file, 0xFFFF); /* command */
    binaryWriteShort(file, 0x00); /* unknown */

    writeEmbOneSection(pattern, file);
    writeSewSegSection(pattern, file);
    writePaletteSection(pattern, file);

    pecLocation = ftell(file);
    fseek(file, 0x08, SEEK_SET);
    binaryWriteByte(file, (unsigned char)(pecLocation & 0xFF));
    binaryWriteByte(file, (unsigned char)(pecLocation >> 8) & 0xFF);
    binaryWriteByte(file, (unsigned char)(pecLocation >> 16) & 0xFF);
    fseek(file, 0x00, SEEK_END);
    writePecStitches(pattern, file, fileName);
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
