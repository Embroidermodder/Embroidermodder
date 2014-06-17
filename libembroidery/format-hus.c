#include "format-hus.h"
#include "emb-compress.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

/*TODO: 'husDecode' is defined but not used. Either remove it or use it. */
/*
static short husDecode(unsigned char a1, unsigned char a2)
{
    unsigned short res = (a2 << 8) + a1;
    if(res >= 0x8000)
    {
        return ((-~res) - 1);
    }
    else
    {
        return (res);
	}
}
*/

static int husDecodeStitchType(unsigned char b)
{
    switch(b)
    {
        case 0x80:
            return NORMAL;
        case 0x81:
            return JUMP;
        case 0x84:
            return STOP;
        case 0x90:
            return END;
        default:
            return NORMAL;
    }
}

static unsigned char* husDecompressData(unsigned char* input, int compressedInputLength, int decompressedContentLength)
{
    unsigned char* decompressedData = (unsigned char*)malloc(sizeof(unsigned char)*decompressedContentLength);
    if(!decompressedData) { embLog_error("format-hus.c husDecompressData(), cannot allocate memory for decompressedData\n"); return 0; }
    husExpand((unsigned char*) input, decompressedData, compressedInputLength, 10);
    return decompressedData;
}

static unsigned char* husCompressData(unsigned char* input, int decompressedInputSize, int* compressedSize)
{
    unsigned char* compressedData = (unsigned char*)malloc(sizeof(unsigned char)*decompressedInputSize*2);
    if(!compressedData) { embLog_error("format-hus.c husCompressData(), cannot allocate memory for compressedData\n"); return 0; }
    *compressedSize = husCompress(input, (unsigned long) decompressedInputSize, compressedData, 10, 0);
    return compressedData;
}

static int husDecodeByte(unsigned char b)
{
    return (char)b;
}

static unsigned char husEncodeByte(double f)
{
    return (unsigned char)(int)roundDouble(f);
}

static unsigned char husEncodeStitchType(int st)
{
    switch(st)
    {
        case NORMAL:
            return (0x80);
        case JUMP:
        case TRIM:
            return (0x81);
        case STOP:
            return (0x84);
        case END:
            return (0x90);
        default:
            return (0x80);
    }
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readHus(EmbPattern* pattern, const char* fileName)
{
    int fileLength;
    int magicCode, numberOfStitches, numberOfColors;
    int postitiveXHoopSize, postitiveYHoopSize, negativeXHoopSize, negativeYHoopSize;

    int attributeOffset, xOffset, yOffset;
    unsigned char* attributeData = 0;
    unsigned char* attributeDataDecompressed = 0;

    unsigned char* xData = 0;
    unsigned char* xDecompressed = 0;

    unsigned char* yData = 0;
    unsigned char* yDecompressed = 0;
    unsigned char* stringVal = 0;
    int unknown, i = 0;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-hus.c readHus(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-hus.c readHus(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-hus.c readHus(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 0x00, SEEK_END);
    fileLength = embFile_tell(file);
    embFile_seek(file, 0x00, SEEK_SET);

    magicCode = binaryReadInt32(file);
    numberOfStitches = binaryReadInt32(file);
    numberOfColors = binaryReadInt32(file);

    postitiveXHoopSize = binaryReadInt16(file);
    postitiveYHoopSize = binaryReadInt16(file);
    negativeXHoopSize = binaryReadInt16(file);
    negativeYHoopSize = binaryReadInt16(file);

    attributeOffset = binaryReadInt32(file);
    xOffset = binaryReadInt32(file);
    yOffset = binaryReadInt32(file);

    stringVal = (unsigned char*)malloc(sizeof(unsigned char)*8);
    if(!stringVal) { embLog_error("format-hus.c readHus(), cannot allocate memory for stringVal\n"); return 0; }
    binaryReadBytes(file, stringVal, 8); /* TODO: check return value */

    unknown = binaryReadInt16(file);
    for(i = 0; i < numberOfColors; i++)
    {
        int pos = binaryReadInt16(file);
        embPattern_addThread(pattern, husThreads[pos]);
    }

    attributeData = (unsigned char*)malloc(sizeof(unsigned char)*(xOffset - attributeOffset + 1));
    if(!attributeData) { embLog_error("format-hus.c readHus(), cannot allocate memory for attributeData\n"); return 0; }
    binaryReadBytes(file, attributeData, xOffset - attributeOffset); /* TODO: check return value */
    attributeDataDecompressed = husDecompressData(attributeData, xOffset - attributeOffset, numberOfStitches + 1);

    xData = (unsigned char*)malloc(sizeof(unsigned char)*(yOffset - xOffset + 1));
    if(!xData) { embLog_error("format-hus.c readHus(), cannot allocate memory for xData\n"); return 0; }
    binaryReadBytes(file, xData, yOffset - xOffset); /* TODO: check return value */
    xDecompressed = husDecompressData(xData, yOffset - xOffset, numberOfStitches);

    yData = (unsigned char*)malloc(sizeof(unsigned char)*(fileLength - yOffset + 1));
    if(!yData) { embLog_error("format-hus.c readHus(), cannot allocate memory for yData\n"); return 0; }
    binaryReadBytes(file, yData, fileLength - yOffset); /* TODO: check return value */
    yDecompressed = husDecompressData(yData, fileLength - yOffset, numberOfStitches);

    for(i = 0; i < numberOfStitches; i++)
    {
        embPattern_addStitchRel(pattern,
                                husDecodeByte(xDecompressed[i]) / 10.0,
                                husDecodeByte(yDecompressed[i]) / 10.0,
                                husDecodeStitchType(attributeDataDecompressed[i]), 1);
    }

    if(stringVal) { free(stringVal); stringVal = 0; }
    if(xData) { free(xData); xData = 0; }
    if(xDecompressed) { free(xDecompressed); xDecompressed = 0; }
    if(yData) { free(yData); yData = 0; }
    if(yDecompressed) { free(yDecompressed); yDecompressed = 0; }
    if(attributeData) { free(attributeData); attributeData = 0; }
    if(attributeDataDecompressed) { free(attributeDataDecompressed); attributeDataDecompressed = 0; }

    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeHus(EmbPattern* pattern, const char* fileName)
{
    EmbRect boundingRect;
    int stitchCount, minColors, patternColor;
    int attributeSize = 0;
    int xCompressedSize = 0;
    int yCompressedSize = 0;
    double previousX = 0;
    double previousY = 0;
    unsigned char* xValues = 0, *yValues = 0, *attributeValues = 0;
    EmbStitchList* pointer = 0;
    double xx = 0.0;
    double yy = 0.0;
    int flags = 0;
    int i = 0;
    unsigned char* attributeCompressed = 0, *xCompressed = 0, *yCompressed = 0;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-hus.c writeHus(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-hus.c writeHus(), fileName argument is null\n"); return 0; }

    stitchCount = embStitchList_count(pattern->stitchList);
    if(!stitchCount)
    {
        embLog_error("format-hus.c writeHus(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchRel(pattern, 0, 0, END, 1);
        stitchCount++;
    }

    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-hus.c writeHus(), cannot open %s for writing\n", fileName);
        return 0;
    }

    /* embPattern_correctForMaxStitchLength(pattern, 0x7F, 0x7F); */
    minColors = embThreadList_count(pattern->threadList);
    patternColor = minColors;
    if(minColors > 24) minColors = 24;
    binaryWriteUInt(file, 0x00C8AF5B);
    binaryWriteUInt(file, stitchCount);
    binaryWriteUInt(file, minColors);

    boundingRect = embPattern_calcBoundingBox(pattern);
    binaryWriteShort(file, (short) roundDouble(boundingRect.right * 10.0));
    binaryWriteShort(file, (short) -roundDouble(boundingRect.top * 10.0 - 1.0));
    binaryWriteShort(file, (short) roundDouble(boundingRect.left * 10.0));
    binaryWriteShort(file, (short) -roundDouble(boundingRect.bottom * 10.0 - 1.0));

    binaryWriteUInt(file, 0x2A + 2 * minColors);

    xValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    if(!xValues) { embLog_error("format-hus.c writeHus(), cannot allocate memory for xValues\n"); return 0; }
    yValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    if(!yValues) { embLog_error("format-hus.c writeHus(), cannot allocate memory for yValues\n"); return 0; }
    attributeValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    if(!attributeValues) { embLog_error("format-hus.c writeHus(), cannot allocate memory for attributeValues\n"); return 0; }

    pointer = pattern->stitchList;
    while(pointer)
    {
        xx = pointer->stitch.xx;
        yy = pointer->stitch.yy;
        flags = pointer->stitch.flags;
        xValues[i] = husEncodeByte((xx - previousX) * 10.0);
        previousX = xx;
        yValues[i] = husEncodeByte((yy - previousY) * 10.0);
        previousY = yy;
        attributeValues[i] = husEncodeStitchType(flags);
        pointer = pointer->next;
        i++;
    }
    attributeCompressed = husCompressData(attributeValues, stitchCount, &attributeSize);
    xCompressed = husCompressData(xValues, stitchCount, &xCompressedSize);
    yCompressed = husCompressData(yValues, stitchCount, &yCompressedSize);
    /* TODO: error if husCompressData returns zero? */

    binaryWriteUInt(file, (unsigned int) (0x2A + 2 * patternColor + attributeSize));
    binaryWriteUInt(file, (unsigned int) (0x2A + 2 * patternColor + attributeSize + xCompressedSize));
    binaryWriteUInt(file, 0x00000000);
    binaryWriteUInt(file, 0x00000000);
    binaryWriteUShort(file, 0x0000);

    for(i = 0; i < patternColor; i++)
    {
        binaryWriteShort(file, (short)embThread_findNearestColorInArray(embThreadList_getAt(pattern->threadList, i).color, (EmbThread*)husThreads, husThreadCount));
    }

    binaryWriteBytes(file, (char*) attributeCompressed, attributeSize);
    binaryWriteBytes(file, (char*) xCompressed, xCompressedSize);
    binaryWriteBytes(file, (char*) yCompressed, yCompressedSize);

    free(xValues); xValues = 0;
    free(xCompressed); xCompressed = 0;
    free(yValues); yValues = 0;
    free(yCompressed); yCompressed = 0;
    free(attributeValues); attributeValues = 0;
    free(attributeCompressed); attributeCompressed = 0;

    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
