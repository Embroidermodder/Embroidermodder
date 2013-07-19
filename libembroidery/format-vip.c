#include <stdio.h>
#include <stdlib.h>
#include "format-vip.h"
#include "helpers-binary.h"
#include "emb-compress.h"

int DecodeByte(unsigned char b)
{
    if (b >= 0x80) return (-(unsigned char) (~b + 1));
    return b;
}

int DecodeStitchType(unsigned char b)
{
    switch (b)
    {
        case 0x80:
            return NORMAL;
		case 0x81:
            return TRIM;
        case 0x84:
            return STOP;
        case 0x90:
            return END;
        default:
            return NORMAL;
    }
}

unsigned char* DecompressData(unsigned char* input, int inputLength, int decompressedContentLength)
{
    unsigned char* decompressedData = (unsigned char*)malloc(decompressedContentLength);
    husExpand((unsigned char*)input, decompressedData, inputLength, 10);
    return decompressedData;
}

int readVip(EmbPattern* pattern, const char* fileName)
{
    int fileLength, magicCode, numberOfStitches, numberOfColors;
    int i, attributeOffset, xOffset, yOffset, unknown, colorLength;
	unsigned char* stringVal;
    unsigned char prevByte = 0;
	int postitiveXHoopSize,postitiveYHoopSize,negativeXHoopSize,negativeYHoopSize;
    unsigned char *attributeData, *decodedColors, *attributeDataDecompressed;
    unsigned char *xData, *xDecompressed, *yData, *yDecompressed;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        /* TODO: set messages here "Error opening VIP file for read:" */
        return 0;
    }
    fseek(file, 0x0, SEEK_END);
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

	colorLength = binaryReadInt32(file);
	decodedColors = (unsigned char *)malloc(numberOfColors*4);
    for(i = 0; i < numberOfColors*4; ++i)
    {
        unsigned char inputByte = binaryReadByte(file);
        unsigned char tmpByte = (unsigned char) (inputByte ^ vipDecodingTable[i]);
        decodedColors[i] = (unsigned char) (tmpByte ^ prevByte);
        prevByte = inputByte;
    }
    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread thread;
        int startIndex = i << 2;
        thread.color.r = decodedColors[startIndex];
        thread.color.g = decodedColors[startIndex + 1];
        thread.color.b = decodedColors[startIndex + 2];
		/* printf("%d\n", decodedColors[startIndex + 3]); */
        embPattern_addThread(pattern, thread);
    }
	fseek(file, attributeOffset, SEEK_SET);
    attributeData = (unsigned char *)malloc(xOffset - attributeOffset);
    binaryReadBytes(file, attributeData, xOffset - attributeOffset);
    attributeDataDecompressed = DecompressData(attributeData, xOffset - attributeOffset, numberOfStitches);

    fseek(file, xOffset, SEEK_SET);
    xData = (unsigned char *)malloc(yOffset - xOffset);
    binaryReadBytes(file, xData, yOffset - xOffset);
    xDecompressed = DecompressData(xData, yOffset - xOffset, numberOfStitches);

    fseek(file, yOffset, SEEK_SET);
    yData = (unsigned char *)malloc(fileLength - yOffset);
    binaryReadBytes(file, yData, fileLength - yOffset);
    yDecompressed = DecompressData(yData, fileLength - yOffset, numberOfStitches);

    for(i = 0; i < numberOfStitches; i++)
    {
        embPattern_addStitchRel(pattern, DecodeByte(xDecompressed[i]) / 10.0,
            DecodeByte(yDecompressed[i]) / 10.0, DecodeStitchType(attributeDataDecompressed[i]), 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);

    return 1; /*TODO: finish readVip */
}

unsigned char* vipCompressData(unsigned char* input, int inputSize, int* compressedSize)
{
    unsigned char* compressedData = (unsigned char*)malloc(sizeof(unsigned char)*inputSize*2);
    *compressedSize = husCompress(input, (unsigned long) inputSize, compressedData, 10, 0);
    return compressedData;
}

unsigned char vipEncodeByte(double f)
{
    return (unsigned char)(int)roundDouble(f);
}

unsigned char vipEncodeStitchType(int st)
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

int writeVip(EmbPattern* pattern, const char* fileName)
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
    unsigned char* attributeCompressed, *xCompressed, *yCompressed, *decodedColors, *encodedColors;
	unsigned char prevByte = 0;
	EmbThreadList *colorPointer;

    FILE* file = fopen(fileName, "wb");
    if(file == 0)
    {
        /*TODO: set status here "Error opening HUS file for write:" */
        return 0;
    }

    stitchCount = embStitch_count(pattern->stitchList);
	minColors = embThread_count(pattern->threadList);
	decodedColors = (unsigned char *) malloc(minColors* 4 *sizeof(unsigned char));
	encodedColors = (unsigned char *) malloc(minColors* 4 *sizeof(unsigned char));
    /* embPattern_correctForMaxStitchLength(pattern, 0x7F, 0x7F); */
    
    patternColor = minColors;
    if(minColors > 24) minColors = 24;

    binaryWriteUInt(file, 0x0190FC5D); 
    binaryWriteUInt(file, stitchCount);
    binaryWriteUInt(file, minColors);

    boundingRect = embPattern_calcBoundingBox(pattern);
    binaryWriteShort(file, (short) roundDouble(boundingRect.right * 10.0));
    binaryWriteShort(file, (short) -roundDouble(boundingRect.top * 10.0 - 1.0));
    binaryWriteShort(file, (short) roundDouble(boundingRect.left * 10.0));
    binaryWriteShort(file, (short) -roundDouble(boundingRect.bottom * 10.0 - 1.0));
	 
    binaryWriteUInt(file, 0x38 + (minColors << 3));

    xValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    yValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));
    attributeValues = (unsigned char*)malloc(sizeof(unsigned char)*(stitchCount));

    pointer = pattern->stitchList;
    while(pointer)
    {
        xx = pointer->stitch.xx;
        yy = pointer->stitch.yy;
        flags = pointer->stitch.flags;
        xValues[i] = vipEncodeByte((xx - previousX) * 10.0);
        previousX = xx;
        yValues[i] = vipEncodeByte((yy - previousY) * 10.0);
        previousY = yy;
        attributeValues[i] = vipEncodeStitchType(flags);
        pointer = pointer->next;
        i++;
    }
    attributeCompressed = vipCompressData(attributeValues, stitchCount, &attributeSize);
    xCompressed = vipCompressData(xValues, stitchCount, &xCompressedSize);
    yCompressed = vipCompressData(yValues, stitchCount, &yCompressedSize);

    binaryWriteUInt(file, (unsigned int) (0x38 + (minColors << 3) + attributeSize));
    binaryWriteUInt(file, (unsigned int) (0x38 + (minColors << 3) + attributeSize + xCompressedSize));
    binaryWriteUInt(file, 0x00000000);
    binaryWriteUInt(file, 0x00000000);
    binaryWriteUShort(file, 0x0000);

	binaryWriteInt(file, minColors << 2);

	colorPointer = pattern->threadList;

    for (i = 0; i < minColors; i++)
    {
        int byteChunk = i << 2;
		EmbColor currentColor = colorPointer->thread.color;
		decodedColors[byteChunk] = currentColor.r;
        decodedColors[byteChunk + 1] = currentColor.g;
        decodedColors[byteChunk + 2] = currentColor.b;
        decodedColors[byteChunk + 3] = 0x01;
		colorPointer = colorPointer->next;
    }
    
    for (i = 0; i < minColors << 2; ++i)
    {
        unsigned char tmpByte = (unsigned char) (decodedColors[i] ^ vipDecodingTable[i]);
		prevByte = (unsigned char) (tmpByte ^ prevByte);
		binaryWriteByte(file, prevByte);
	}
	for (i = 0; i <= minColors; i++)
    {
        binaryWriteInt(file, 1);
    }
    binaryWriteUInt(file, 0); /* string length */
	binaryWriteShort(file, 0);
    binaryWriteBytes(file, (char*) attributeCompressed, attributeSize);
	binaryWriteBytes(file, (char*) xCompressed, xCompressedSize);
	binaryWriteBytes(file, (char*) yCompressed, yCompressedSize);
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
