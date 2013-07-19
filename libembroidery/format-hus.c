#include "format-hus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <limits.h>
#include "helpers-binary.h"
#include "emb-compress.h"

/*****************************************
 * EmbReaderWriter Functions
 ****************************************/

short husDecode(unsigned char a1, unsigned char a2)
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

int husDecodeStitchType(unsigned char b)
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

unsigned char* husDecompressData(unsigned char* input, int inputLength, int decompressedContentLength)
{
    unsigned char* decompressedData = (unsigned char*)malloc(sizeof(unsigned char)*decompressedContentLength);
    husExpand((unsigned char*) input, decompressedData, inputLength, 10);
    return decompressedData;
}

unsigned char* husCompressData(unsigned char* input, int inputSize, int* compressedSize)
{
    unsigned char* compressedData = (unsigned char*)malloc(sizeof(unsigned char)*inputSize*2);
    *compressedSize = husCompress(input, (unsigned long) inputSize, compressedData, 10, 0);
    return compressedData;
}

int husDecodeByte(unsigned char b)
{
    return (char)b;
}

unsigned char husEncodeByte(double f)
{
    return (unsigned char)(int)roundDouble(f);
}

unsigned char husEncodeStitchType(int st)
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

int readHus(EmbPattern* pattern, const char* fileName)
{
    int fileLength;
    int magicCode,numberOfStitches,numberOfColors;
    int postitiveXHoopSize,postitiveYHoopSize,negativeXHoopSize,negativeYHoopSize;

    int attributeOffset,xOffset,yOffset;
    unsigned char* attributeData;
    unsigned char* attributeDataDecompressed;

    unsigned char* xData;
    unsigned char* xDecompressed;

    unsigned char* yData;
    unsigned char* yDecompressed;
    unsigned char* stringVal;
    int unknown, i = 0;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        /*TODO: set status here "Error opening HUS file for read:" */
        return 0;
    }

    fseek(file, 0x00, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0x00, SEEK_SET);

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
    binaryReadBytes(file, stringVal, 8);

    unknown = binaryReadInt16(file);
    for(i = 0; i < numberOfColors; i++)
    {
        int pos = binaryReadInt16(file);
        embPattern_addThread(pattern, husThreads[pos]);
    }

    attributeData = (unsigned char*)malloc(sizeof(unsigned char)*(xOffset - attributeOffset));
    binaryReadBytes(file, attributeData, xOffset - attributeOffset);
    attributeDataDecompressed = husDecompressData(attributeData, xOffset - attributeOffset, numberOfStitches);

    xData = (unsigned char*)malloc(sizeof(unsigned char)*(yOffset - xOffset));
    binaryReadBytes(file, xData, yOffset - xOffset);
    xDecompressed = husDecompressData(xData, yOffset - xOffset, numberOfStitches);

    yData = (unsigned char*)malloc(sizeof(unsigned char)*(fileLength - yOffset));
    binaryReadBytes(file, yData, fileLength - yOffset);
    yDecompressed = husDecompressData(yData, fileLength - yOffset, numberOfStitches);

    for(i = 0; i < numberOfStitches; i++)
    {
        embPattern_addStitchRel(pattern, husDecodeByte(xDecompressed[i]) / 10.0,
                                husDecodeByte(yDecompressed[i]) / 10.0,
                                husDecodeStitchType(attributeDataDecompressed[i]), 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);

	free(xData);
	free(xDecompressed);
	free(yData);
	free(yDecompressed);
	free(attributeData);
	free(attributeDataDecompressed);
	
	fclose(file);
    return 1;
}

int writeHus(EmbPattern* pattern, const char* fileName)
{
    EmbRect boundingRect;
    int stitchCount, minColors, patternColor;
    int attributeSize = 0;
    int xCompressedSize = 0;
    int yCompressedSize = 0;
    double previousX = 0;
    double previousY = 0;
    unsigned char* xValues, *yValues, *attributeValues;
    EmbStitchList* pointer;
    double xx = 0.0;
    double yy = 0.0;
    int flags = 0;
    int i = 0;
    unsigned char* attributeCompressed, *xCompressed, *yCompressed;

    FILE* file = fopen(fileName, "wb");
    if(file == 0)
    {
        /*TODO: set status here "Error opening HUS file for write:" */
        return 0;
    }

    stitchCount = embStitch_count(pattern->stitchList);
    /* embPattern_correctForMaxStitchLength(pattern, 0x7F, 0x7F); */
    minColors = embThread_count(pattern->threadList);
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
    yValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    attributeValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));

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

    binaryWriteUInt(file, (unsigned int) (0x2A + 2 * patternColor + attributeSize));
    binaryWriteUInt(file, (unsigned int) (0x2A + 2 * patternColor + attributeSize + xCompressedSize));
    binaryWriteUInt(file, 0x00000000);
    binaryWriteUInt(file, 0x00000000);
    binaryWriteUShort(file, 0x0000);

    for(i = 0; i < patternColor; i++)
    {
        binaryWriteShort(file, embThread_findNearestColorInArray(embThread_getAt(pattern->threadList, i).color, (EmbThread*)husThreads, husThreadCount));
    }

    binaryWriteBytes(file, (char*) attributeCompressed, attributeSize);
    binaryWriteBytes(file, (char*) xCompressed, xCompressedSize);
    binaryWriteBytes(file, (char*) yCompressed, yCompressedSize);

	free(xValues);
    free(xCompressed);
	free(yValues);
	free(yCompressed);
	free(attributeValues);
	free(attributeCompressed);

    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
