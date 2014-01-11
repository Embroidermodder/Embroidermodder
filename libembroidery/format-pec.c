#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers-binary.h"
#include "helpers-misc.h"
#include "format-pec.h"

void readPecStitches(EmbPattern* pattern, FILE* file)
{
    int stitchNumber = 0;
    while(!feof(file))
    {
        int val1 = (int)binaryReadUInt8(file);
        int val2 = (int)binaryReadUInt8(file);

        int stitchType = NORMAL;
        if(val1 == 0xFF && val2 == 0x00)
        {
            embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
            break;
        }
        if(val1 == 0xFE && val2 == 0xB0)
        {
            (void)binaryReadByte(file);
            embPattern_addStitchRel(pattern, 0.0, 0.0, STOP, 1);
            stitchNumber++;
            continue;
        }
        /* High bit set means 12-bit offset, otherwise 7-bit signed delta */
        if(val1 & 0x80)
        {
            if(val1 & 0x20) stitchType = TRIM;
            if(val1 & 0x10) stitchType = JUMP;
            val1 = ((val1 & 0x0F) << 8) + val2;

            /* Signed 12-bit arithmetic */
            if(val1 & 0x800)
            {
                val1 -= 0x1000;
            }

            val2 = binaryReadUInt8(file);
        }
        else if(val1 >= 0x40)
        {
            val1 -= 0x80;
        }
        if(val2 & 0x80)
        {
            if(val2 & 0x20) stitchType = TRIM;
            if(val2 & 0x10) stitchType = JUMP;
            val2 = ((val2 & 0x0F) << 8) + binaryReadUInt8(file);

            /* Signed 12-bit arithmetic */
            if(val2 & 0x800)
            {
                val2 -= 0x1000;
            }
        }
        else if(val2 >= 0x40)
        {
            val2 -= 0x80;
        }
        embPattern_addStitchRel(pattern, val1 / 10.0, val2 / 10.0, stitchType, 1);
        stitchNumber++;
    }
}

static void pecEncodeJump(FILE* file, int x, int types)
{
    int outputVal = abs(x) & 0x7FF;
    unsigned int orPart = 0x80;
    if(types & TRIM)
    {
        orPart |= 0x20;
    }
    else if(types & JUMP)
    {
        orPart |= 0x10;
    }

    if(x < 0)
    {
        outputVal = x + 0x1000 & 0x7FF;
        outputVal |= 0x800;
    }
    binaryWriteByte(file, (unsigned char)(((outputVal >> 8) & 0x0F) | orPart));
    binaryWriteByte(file, (unsigned char)(outputVal & 0xFF));
}

static void pecEncodeStop(FILE* file, unsigned char val)
{
    binaryWriteByte(file, 0xFE);
    binaryWriteByte(file, 0xB0);
    binaryWriteByte(file, val);
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPec(EmbPattern* pattern, const char* fileName)
{
    unsigned int graphicsOffset;
    unsigned char colorChanges;
    int i;
    FILE* file = fopen(fileName, "rb");
    if(!file)
    {
        return 0;
    }
    fseek(file, 0x38, SEEK_SET);
    colorChanges = (unsigned char)binaryReadByte(file);
    for(i = 0; i <= colorChanges; i++)
    {
        embPattern_addThread(pattern, pecThreads[binaryReadByte(file) % 65]);
    }

    /* Get Graphics offset */
    fseek(file, 0x20A, SEEK_SET);

    graphicsOffset = (unsigned int)(binaryReadUInt8(file));
    graphicsOffset |= (binaryReadUInt8(file) << 8);
    graphicsOffset |= (binaryReadUInt8(file) << 16);

    (void)binaryReadByte(file); /* 0x31 */
    (void)binaryReadByte(file); /* 0xFF */
    (void)binaryReadByte(file); /* 0xF0 */
    /* Get X and Y size in .1 mm */
    /* 0x210 */
    binaryReadInt16(file); /* x size */
    binaryReadInt16(file); /* y size */

    binaryReadInt16(file); /* 0x01E0 */
    binaryReadInt16(file); /* 0x01B0 */
    binaryReadInt16(file); /* distance left from start */
    binaryReadInt16(file); /* distance up from start */

    /* Begin Stitch Data */
    /* 0x21C */
    /*unsigned int end = graphicsOffset + 0x208; */
    readPecStitches(pattern, file);

    embPattern_flipVertical(pattern);
    fclose(file);
    return 1;
}

static void PecEncode(FILE* file, EmbPattern* p)
{
    double thisX = 0.0;
    double thisY = 0.0;
    unsigned char stopCode = 2;
    EmbStitchList* list = p->stitchList;
    while(list)
    {
        int deltaX, deltaY;
        EmbStitch s = list->stitch;

        deltaX = roundDouble(s.xx - thisX);
        deltaY = roundDouble(s.yy - thisY);
        thisX += (double)deltaX;
        thisY += (double)deltaY;

        if(s.flags & STOP)
        {
            pecEncodeStop(file, stopCode);
            if(stopCode == (unsigned char)2)
            {
                stopCode = (unsigned char)1;
            }
            else
            {
                stopCode = (unsigned char)2;
            }
        }
        else if(s.flags & END)
        {
            binaryWriteByte(file, 0xFF);
            break;
        }
        else if(deltaX < 63 && deltaX > -64 && deltaY < 63 && deltaY > -64 && (!(s.flags & (JUMP | TRIM))))
        {
            binaryWriteByte(file, (deltaX < 0) ? (unsigned char)(deltaX + 0x80) : (unsigned char)deltaX);
            binaryWriteByte(file, (deltaY < 0) ? (unsigned char)(deltaY + 0x80) : (unsigned char)deltaY);
        }
        else
        {
            pecEncodeJump(file, deltaX, s.flags);
            pecEncodeJump(file, deltaY, s.flags);
        }
        list = list->next;
    }
}

static void clearImage(unsigned char image[][48])
{
    memcpy(image, imageWithFrame, 48*38);
}

static void writeImage(FILE *file, unsigned char image[][48])
{
    int i, j;
    for(i = 0; i < 38; i++)
    {
        for(j = 0; j < 6; j++)
        {
            int offset = j * 8;
            unsigned char output = 0;
            output |= (unsigned char)(image[i][offset] != 0);
            output |= (unsigned char)(image[i][offset + 1] != (unsigned char)0) << 1;
            output |= (unsigned char)(image[i][offset + 2] != (unsigned char)0) << 2;
            output |= (unsigned char)(image[i][offset + 3] != (unsigned char)0) << 3;
            output |= (unsigned char)(image[i][offset + 4] != (unsigned char)0) << 4;
            output |= (unsigned char)(image[i][offset + 5] != (unsigned char)0) << 5;
            output |= (unsigned char)(image[i][offset + 6] != (unsigned char)0) << 6;
            output |= (unsigned char)(image[i][offset + 7] != (unsigned char)0) << 7;
            binaryWriteByte(file, output);
        }
    }
}

void writePecStitches(EmbPattern* pattern, FILE* file, const char* fileName)
{
    EmbStitchList *tempStitches;
    EmbRect bounds;
    unsigned char image[38][48];
    int i, flen, currentThreadCount, graphicsOffsetLocation, graphicsOffsetValue, height, width;
    double xFactor, yFactor;
    const char* forwardSlashPos = strrchr(fileName, '/');
    const char* backSlashPos = strrchr(fileName, '\\');
    const char* dotPos = strrchr(fileName, '.');
    const char* start;
    start = 0;
    if(forwardSlashPos)
    {
        start = forwardSlashPos + 1;
    }
    if(backSlashPos && backSlashPos > start)
    {
        start = backSlashPos + 1;
    }
    if(!start)
    {
        start = fileName;
    }
    binaryWriteBytes(file, "LA:", 3);
    flen = (int)(dotPos - start);

    while(start < dotPos)
    {
        binaryWriteByte(file, (unsigned char)*start);
        start++;
    }
    for(i = 0; i < (int)(16-flen); i++)
    {
        binaryWriteByte(file, (unsigned char)0x20);
    }
    binaryWriteByte(file, 0x0D);
    for(i = 0; i < 12; i++)
    {
        binaryWriteByte(file, (unsigned char)0x20);
    }
    binaryWriteByte(file, (unsigned char)0xFF);
    binaryWriteByte(file, (unsigned char)0x00);
    binaryWriteByte(file, (unsigned char)0x06);
    binaryWriteByte(file, (unsigned char)0x26);

    for(i = 0; i < 12; i++)
    {
        binaryWriteByte(file, (unsigned char)0x20);
    }
    currentThreadCount = embThreadList_count(pattern->threadList);
    binaryWriteByte(file, (unsigned char)(currentThreadCount-1));

    for(i = 0; i < currentThreadCount; i++)
    {
        binaryWriteByte(file, (unsigned char)embThread_findNearestColorInArray(embThreadList_getAt(pattern->threadList, i).color, (EmbThread*)pecThreads, pecThreadCount));
    }
    for(i = 0; i < (int)(0x1CF - currentThreadCount); i++)
    {
        binaryWriteByte(file, (unsigned char)0x20);
    }
    binaryWriteShort(file, (short)0x0000);

    graphicsOffsetLocation = ftell(file);
    /* placeholder bytes to be overwritten */
    binaryWriteByte(file, (unsigned char)0x00);
    binaryWriteByte(file, (unsigned char)0x00);
    binaryWriteByte(file, (unsigned char)0x00);

    binaryWriteByte(file, (unsigned char)0x31);
    binaryWriteByte(file, (unsigned char)0xFF);
    binaryWriteByte(file, (unsigned char)0xF0);

    bounds = embPattern_calcBoundingBox(pattern);

    height = roundDouble(embRect_height(bounds));
    width = roundDouble(embRect_width(bounds));
    /* write 2 byte x size */
    binaryWriteShort(file, (short)width);
    /* write 2 byte y size */
    binaryWriteShort(file, (short)height);

    /* Write 4 miscellaneous int16's */
    binaryWriteShort(file, (short)0x1E0);
    binaryWriteShort(file, (short)0x1B0);

    binaryWriteUShortBE(file, (unsigned short)(0x9000 | -roundDouble(bounds.left)));
    binaryWriteUShortBE(file, (unsigned short)(0x9000 | -roundDouble(bounds.top)));

    PecEncode(file, pattern);
    graphicsOffsetValue = ftell(file) - graphicsOffsetLocation + 2;
    fseek(file, graphicsOffsetLocation, SEEK_SET);

    binaryWriteByte(file, (unsigned char)(graphicsOffsetValue & 0xFF));
    binaryWriteByte(file, (unsigned char)((graphicsOffsetValue >> 8) & 0xFF));
    binaryWriteByte(file, (unsigned char)((graphicsOffsetValue >> 16) & 0xFF));

    fseek(file, 0x00, SEEK_END);

    /* Writing all colors */
    clearImage(image);
    tempStitches = pattern->stitchList;

    yFactor = 32.0 / height;
    xFactor = 42.0 / width;
    while(tempStitches->next)
    {
        int x = roundDouble((tempStitches->stitch.xx - bounds.left) * xFactor) + 3;
        int y = roundDouble((tempStitches->stitch.yy - bounds.top) * yFactor) + 3;
        image[y][x] = 1;
        tempStitches = tempStitches->next;
    }
    writeImage(file, image);

    /* Writing each individual color */
    tempStitches = pattern->stitchList;
    for(i = 0; i < currentThreadCount; i++)
    {
        clearImage(image);
        while(tempStitches->next)
        {
            int x = roundDouble((tempStitches->stitch.xx - bounds.left) * xFactor) + 3;
            int y = roundDouble((tempStitches->stitch.yy - bounds.top) * yFactor) + 3;
            if(tempStitches->stitch.flags & STOP)
            {
                tempStitches = tempStitches->next;
                break;
            }
            image[y][x] = 1;
            tempStitches = tempStitches->next;
        }
        writeImage(file, image);
    }
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePec(EmbPattern* pattern, const char* fileName)
{
    FILE* file = fopen(fileName, "wb");
    if(!file)
    {
        return 0;
    }
    embPattern_flipVertical(pattern);
    embPattern_fixColorCount(pattern);
    embPattern_correctForMaxStitchLength(pattern,12.7, 204.7);
    embPattern_scale(pattern, 10.0);

    binaryWriteBytes(file, "#PEC0001", 8);

    writePecStitches(pattern, file, fileName);

    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
