#include "format-pes.h"
#include "format-pec.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdlib.h>

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPes(EmbPattern* pattern, const char* fileName)
{
    int pecstart, numColors, x;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-pes.c readPes(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pes.c readPes(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-pes.c readPes(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 8, SEEK_SET);
    pecstart = binaryReadInt32(file);

    embFile_seek(file, pecstart + 48, SEEK_SET);
    numColors = embFile_getc(file) + 1;
    for(x = 0; x < numColors; x++)
    {
        embPattern_addThread(pattern, pecThreads[(unsigned char) embFile_getc(file)]);
    }

    embFile_seek(file, pecstart + 532, SEEK_SET);
    readPecStitches(pattern, file);

    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    embPattern_flipVertical(pattern);

    return 1;
}

static void pesWriteSewSegSection(EmbPattern* pattern, EmbFile* file)
{
    /* TODO: pointer safety */
    EmbStitchList* pointer = 0;
    EmbStitchList* mainPointer = 0;
    short* colorInfo = 0;
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

    binaryWriteShort(file, (short)blockCount); /* block count */
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
            colorInfo[colorInfoIndex++] = (short)blockCount;
            colorInfo[colorInfoIndex++] = (short)newColorCode;
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

        binaryWriteShort(file, (short)stitchType); /* 1 for jump, 0 for normal */
        binaryWriteShort(file, (short)colorCode); /* color code */
        binaryWriteShort(file, (short)count); /* stitches in block */
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
    binaryWriteShort(file, (short)colorCount);
    for(i = 0; i < colorCount; i++)
    {
        binaryWriteShort(file, colorInfo[i * 2]);
        binaryWriteShort(file, colorInfo[i * 2 + 1]);
    }
    binaryWriteInt(file, 0);
    if(colorInfo)
    {
        free(colorInfo);
        colorInfo = 0;
    }
}

static void pesWriteEmbOneSection(EmbPattern* pattern, EmbFile* file)
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

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePes(EmbPattern* pattern, const char* fileName)
{
    int pecLocation;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-pes.c writePes(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pes.c writePes(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "wb");
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

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

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

    pecLocation = embFile_tell(file);
    embFile_seek(file, 0x08, SEEK_SET);
    binaryWriteByte(file, (unsigned char)(pecLocation & 0xFF));
    binaryWriteByte(file, (unsigned char)(pecLocation >> 8) & 0xFF);
    binaryWriteByte(file, (unsigned char)(pecLocation >> 16) & 0xFF);
    embFile_seek(file, 0x00, SEEK_END);
    writePecStitches(pattern, file, fileName);
    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
