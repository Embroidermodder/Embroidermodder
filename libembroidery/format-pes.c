#include "format-pes.h"
#include "format-pec.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdlib.h>

static short pecDecodeNormal(unsigned char inputByte)
{
    if(inputByte < 0x40)
    {
        return inputByte;
    }
    return (inputByte - 0x80);
}

static int pecJumpDecode(unsigned char byte1, unsigned char byte2)
{
    unsigned n1 = (unsigned char) (byte1 & 0x0F);
    if(n1 <= 7)
    {
        int returnValue = (n1 << 8) + byte2;
        return returnValue;
    }
    else
    {
        int returnValue = -(int)((256 - byte2) + ((15 - n1) << 8));
        return returnValue;
    }
}

int readPes(EmbPattern* pattern, const char* fileName)
{
    int pecstart, numColors, x;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-pes.c readPes(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pes.c readPes(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "rb");
    if(!file)
    {
        embLog_error("format-pes.c readPes(), cannot open %s for reading\n", fileName);
        return 0;
    }

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

static void pesWriteSewSegSection(EmbPattern* pattern, FILE* file)
{
    /* TODO: pointer safety */
    EmbStitchList* pointer = 0;
    EmbStitchList* mainPointer = 0;
    short *colorInfo = 0;
    int flag = 0;
    int count = 0;
    int colorCode = -1;
    int stitchType = 0;
    int blockCount = 0;
    int colorCount = 0;
    int newColorCode = 0;
    int colorInfoIndex = 0;
    int i;
    EmbRect bounds = embPattern_calcBoundingBox(pattern);
    EmbColor color;

    mainPointer = pattern->stitchList;
    while(mainPointer)
    {
        pointer = mainPointer;
        flag = pointer->stitch.flags;
        color = embThreadList_getAt(pattern->threadList, pointer->stitch.color).color;
        newColorCode = embThread_findNearestColorInArray(color, (EmbThread*)pecThreads, pecThreadCount);
        if(newColorCode != colorCode)
        {
            colorCount++;
            colorCode = newColorCode;
        }
        while(pointer && (flag == pointer->stitch.flags))
        {
            count++;
            pointer = pointer->next;
        }
        blockCount++;
        mainPointer = pointer;
    }

    binaryWriteShort(file, blockCount); /* block count */
    binaryWriteUShort(file, 0xFFFF);
    binaryWriteShort(file, 0x00);

    binaryWriteShort(file, 0x07); /* string length */
    binaryWriteBytes(file, "CSewSeg", 7);

    colorInfo = (short *) calloc(colorCount * 2, sizeof(short));
    mainPointer = pattern->stitchList;
    colorCode = -1;
    blockCount = 0;
    while(mainPointer)
    {
        pointer = mainPointer;
        flag = pointer->stitch.flags;
        color = embThreadList_getAt(pattern->threadList, pointer->stitch.color).color;
        newColorCode = embThread_findNearestColorInArray(color, (EmbThread*)pecThreads, pecThreadCount);
        if(newColorCode != colorCode)
        {
            colorInfo[colorInfoIndex++] = blockCount;
            colorInfo[colorInfoIndex++] = newColorCode;
            colorCode = newColorCode;
        }
        count = 0;
        while(pointer && (flag == pointer->stitch.flags))
        {
            count++;
            pointer = pointer->next;
        }
        if(flag & JUMP)
        {
            stitchType = 1;
        }
        else
        {
            stitchType = 0;
        }

        binaryWriteShort(file, stitchType); /* 1 for jump, 0 for normal */
        binaryWriteShort(file, (short)colorCode); /* color code */
        binaryWriteShort(file, count); /* stitches in block */
        pointer = mainPointer;
        while(pointer && (flag == pointer->stitch.flags))
        {
            EmbStitch s = pointer->stitch;
            binaryWriteShort(file, (short)(s.xx - bounds.left));
            binaryWriteShort(file, (short)(s.yy + bounds.top));
            pointer = pointer->next;
        }
        if(pointer)
        {
            binaryWriteShort(file, 0x8003);
        }
        blockCount++;
        mainPointer = pointer;
    }
    binaryWriteShort(file, colorCount);
    for(i = 0; i < colorCount; i++)
    {
        binaryWriteShort(file, colorInfo[i * 2]);
        binaryWriteShort(file, colorInfo[i * 2 + 1]);
    }
    binaryWriteInt(file, 0);
    if(colorInfo)
    {
        free(colorInfo);
    }
}
static void pesWriteEmbOneSection(EmbPattern* pattern, FILE* file)
{
    /* TODO: pointer safety */
    int i;
    int hoopHeight = 1800, hoopWidth = 1300;
    EmbRect bounds;
    binaryWriteShort(file, 0x07); /* string length */
    binaryWriteBytes(file, "CEmbOne", 7);
    bounds = embPattern_calcBoundingBox(pattern);

    binaryWriteShort(file, 0);
    binaryWriteShort(file, 0);
    binaryWriteShort(file, 0);
    binaryWriteShort(file, 0);

    binaryWriteShort(file, 0);
    binaryWriteShort(file, 0);
    binaryWriteShort(file, 0);
    binaryWriteShort(file, 0);

    /* AffineTransform */
    binaryWriteFloat(file, 1.0f);
    binaryWriteFloat(file, 0.0f);
    binaryWriteFloat(file, 0.0f);
    binaryWriteFloat(file, 1.0f);
    binaryWriteFloat(file, (float)((embRect_width(bounds) - hoopWidth) / 2));
    binaryWriteFloat(file, (float)((embRect_height(bounds) + hoopHeight) / 2));

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

int writePes(EmbPattern* pattern, const char* fileName)
{
    int pecLocation;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-pes.c writePes(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pes.c writePes(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "wb");
    if(!file)
    {
        embLog_error("format-pes.c writePes(), cannot open %s for writing\n", fileName);
        return 0;
    }

    if(!pattern->stitchList || embStitchList_count(pattern->stitchList) == 0) /* TODO: review this. seems like only embStitchList_count should be needed. */
    {
        embLog_error("format-pes.c writePes(), pattern contains no stitches\n");
        return 0;
    }

    embPattern_flipVertical(pattern);
    embPattern_scale(pattern, 10.0);
    binaryWriteBytes(file, "#PES0001", 8);
    /* WRITE PECPointer 32 bit int */
    binaryWriteInt(file, 0x00);

    binaryWriteShort(file, 0x01);
    binaryWriteShort(file, 0x01);

    /* Write object count */
    binaryWriteShort(file, 0x01);
    binaryWriteShort(file, 0xFFFF); /* command */
    binaryWriteShort(file, 0x00); /* unknown */

    pesWriteEmbOneSection(pattern, file);
    pesWriteSewSegSection(pattern, file);

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
