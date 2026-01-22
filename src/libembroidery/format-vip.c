#include <stdlib.h>

#include "embroidery.h"

const unsigned char vipDecodingTable[] = {
    0x2E, 0x82, 0xE4, 0x6F, 0x38, 0xA9, 0xDC, 0xC6, 0x7B, 0xB6, 0x28, 0xAC, 0xFD, 0xAA, 0x8A, 0x4E,
    0x76, 0x2E, 0xF0, 0xE4, 0x25, 0x1B, 0x8A, 0x68, 0x4E, 0x92, 0xB9, 0xB4, 0x95, 0xF0, 0x3E, 0xEF,
    0xF7, 0x40, 0x24, 0x18, 0x39, 0x31, 0xBB, 0xE1, 0x53, 0xA8, 0x1F, 0xB1, 0x3A, 0x07, 0xFB, 0xCB,
    0xE6, 0x00, 0x81, 0x50, 0x0E, 0x40, 0xE1, 0x2C, 0x73, 0x50, 0x0D, 0x91, 0xD6, 0x0A, 0x5D, 0xD6,
    0x8B, 0xB8, 0x62, 0xAE, 0x47, 0x00, 0x53, 0x5A, 0xB7, 0x80, 0xAA, 0x28, 0xF7, 0x5D, 0x70, 0x5E,
    0x2C, 0x0B, 0x98, 0xE3, 0xA0, 0x98, 0x60, 0x47, 0x89, 0x9B, 0x82, 0xFB, 0x40, 0xC9, 0xB4, 0x00,
    0x0E, 0x68, 0x6A, 0x1E, 0x09, 0x85, 0xC0, 0x53, 0x81, 0xD1, 0x98, 0x89, 0xAF, 0xE8, 0x85, 0x4F,
    0xE3, 0x69, 0x89, 0x03, 0xA1, 0x2E, 0x8F, 0xCF, 0xED, 0x91, 0x9F, 0x58, 0x1E, 0xD6, 0x84, 0x3C,
    0x09, 0x27, 0xBD, 0xF4, 0xC3, 0x90, 0xC0, 0x51, 0x1B, 0x2B, 0x63, 0xBC, 0xB9, 0x3D, 0x40, 0x4D,
    0x62, 0x6F, 0xE0, 0x8C, 0xF5, 0x5D, 0x08, 0xFD, 0x3D, 0x50, 0x36, 0xD7, 0xC9, 0xC9, 0x43, 0xE4,
    0x2D, 0xCB, 0x95, 0xB6, 0xF4, 0x0D, 0xEA, 0xC2, 0xFD, 0x66, 0x3F, 0x5E, 0xBD, 0x69, 0x06, 0x2A,
    0x03, 0x19, 0x47, 0x2B, 0xDF, 0x38, 0xEA, 0x4F, 0x80, 0x49, 0x95, 0xB2, 0xD6, 0xF9, 0x9A, 0x75,
    0xF4, 0xD8, 0x9B, 0x1D, 0xB0, 0xA4, 0x69, 0xDB, 0xA9, 0x21, 0x79, 0x6F, 0xD8, 0xDE, 0x33, 0xFE,
    0x9F, 0x04, 0xE5, 0x9A, 0x6B, 0x9B, 0x73, 0x83, 0x62, 0x7C, 0xB9, 0x66, 0x76, 0xF2, 0x5B, 0xC9,
    0x5E, 0xFC, 0x74, 0xAA, 0x6C, 0xF1, 0xCD, 0x93, 0xCE, 0xE9, 0x80, 0x53, 0x03, 0x3B, 0x97, 0x4B,
    0x39, 0x76, 0xC2, 0xC1, 0x56, 0xCB, 0x70, 0xFD, 0x3B, 0x3E, 0x52, 0x57, 0x81, 0x5D, 0x56, 0x8D,
    0x51, 0x90, 0xD4, 0x76, 0xD7, 0xD5, 0x16, 0x02, 0x6D, 0xF2, 0x4D, 0xE1, 0x0E, 0x96, 0x4F, 0xA1,
    0x3A, 0xA0, 0x60, 0x59, 0x64, 0x04, 0x1A, 0xE4, 0x67, 0xB6, 0xED, 0x3F, 0x74, 0x20, 0x55, 0x1F,
    0xFB, 0x23, 0x92, 0x91, 0x53, 0xC8, 0x65, 0xAB, 0x9D, 0x51, 0xD6, 0x73, 0xDE, 0x01, 0xB1, 0x80,
    0xB7, 0xC0, 0xD6, 0x80, 0x1C, 0x2E, 0x3C, 0x83, 0x63, 0xEE, 0xBC, 0x33, 0x25, 0xE2, 0x0E, 0x7A,
    0x67, 0xDE, 0x3F, 0x71, 0x14, 0x49, 0x9C, 0x92, 0x93, 0x0D, 0x26, 0x9A, 0x0E, 0xDA, 0xED, 0x6F,
    0xA4, 0x89, 0x0C, 0x1B, 0xF0, 0xA1, 0xDF, 0xE1, 0x9E, 0x3C, 0x04, 0x78, 0xE4, 0xAB, 0x6D, 0xFF,
    0x9C, 0xAF, 0xCA, 0xC7, 0x88, 0x17, 0x9C, 0xE5, 0xB7, 0x33, 0x6D, 0xDC, 0xED, 0x8F, 0x6C, 0x18,
    0x1D, 0x71, 0x06, 0xB1, 0xC5, 0xE2, 0xCF, 0x13, 0x77, 0x81, 0xC5, 0xB7, 0x0A, 0x14, 0x0A, 0x6B,
    0x40, 0x26, 0xA0, 0x88, 0xD1, 0x62, 0x6A, 0xB3, 0x50, 0x12, 0xB9, 0x9B, 0xB5, 0x83, 0x9B, 0x37
};

static int vipDecodeByte(unsigned char b)
{
    if (b >= 0x80) return (-(unsigned char) (~b + 1));
    return b;
}

static int vipDecodeStitchType(unsigned char b)
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

static unsigned char* vipDecompressData(unsigned char* input, int compressedInputLength, int decompressedContentLength)
{
    unsigned char* decompressedData = (unsigned char*)malloc(decompressedContentLength);
    if(!decompressedData)
    {
        embLog_error("format-vip.c vipDecompressData(), cannot allocate memory for decompressedData\n");
        return 0;
    }
    husExpand((unsigned char*)input, decompressedData, compressedInputLength, 10);
    return decompressedData;
}

typedef struct VipHeader_
{
    int magicCode;
    int numberOfStitches;
    int numberOfColors;
    short postitiveXHoopSize;
    short postitiveYHoopSize;
    short negativeXHoopSize;
    short negativeYHoopSize;
    int attributeOffset;
    int xOffset;
    int yOffset;
    unsigned char stringVal[8];
    short unknown;
    int colorLength;
} VipHeader;

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readVip(EmbPattern* pattern, const char* fileName)
{
    int fileLength;
    int i;
    unsigned char prevByte = 0;
    unsigned char *attributeData = 0, *decodedColors = 0, *attributeDataDecompressed = 0;
    unsigned char *xData = 0, *xDecompressed = 0, *yData = 0, *yDecompressed = 0;
    VipHeader header;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-vip.c readVip(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-vip.c readVip(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-vip.c readVip(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 0x0, SEEK_END);
    fileLength = embFile_tell(file);
    embFile_seek(file, 0x00, SEEK_SET);
    header.magicCode = binaryReadInt32(file);
    header.numberOfStitches = binaryReadInt32(file);
    header.numberOfColors = binaryReadInt32(file);

    header.postitiveXHoopSize = binaryReadInt16(file);
    header.postitiveYHoopSize = binaryReadInt16(file);
    header.negativeXHoopSize = binaryReadInt16(file);
    header.negativeYHoopSize = binaryReadInt16(file);

    header.attributeOffset = binaryReadInt32(file);
    header.xOffset = binaryReadInt32(file);
    header.yOffset = binaryReadInt32(file);

    /*stringVal = (unsigned char*)malloc(sizeof(unsigned char)*8); TODO: review this and uncomment or remove
        if(!stringVal) { embLog_error("format-vip.c readVip(), cannot allocate memory for stringVal\n"); return 0; }
     */

    binaryReadBytes(file, header.stringVal, 8); /* TODO: check return value */

    header.unknown = binaryReadInt16(file);

    header.colorLength = binaryReadInt32(file);
    decodedColors = (unsigned char*)malloc(header.numberOfColors*4);
    if(!decodedColors) { embLog_error("format-vip.c readVip(), cannot allocate memory for decodedColors\n"); return 0; }
    for(i = 0; i < header.numberOfColors*4; ++i)
    {
        unsigned char inputByte = binaryReadByte(file);
        unsigned char tmpByte = (unsigned char) (inputByte ^ vipDecodingTable[i]);
        decodedColors[i] = (unsigned char) (tmpByte ^ prevByte);
        prevByte = inputByte;
    }
    for(i = 0; i < header.numberOfColors; i++)
    {
        EmbThread thread;
        int startIndex = i << 2;
        thread.color.r = decodedColors[startIndex];
        thread.color.g = decodedColors[startIndex + 1];
        thread.color.b = decodedColors[startIndex + 2];
        /* printf("%d\n", decodedColors[startIndex + 3]); */
        embPattern_addThread(pattern, thread);
    }
    embFile_seek(file, header.attributeOffset, SEEK_SET);
    attributeData = (unsigned char*)malloc(header.xOffset - header.attributeOffset);
    if(!attributeData) { embLog_error("format-vip.c readVip(), cannot allocate memory for attributeData\n"); return 0; }
    binaryReadBytes(file, attributeData, header.xOffset - header.attributeOffset); /* TODO: check return value */
    attributeDataDecompressed = vipDecompressData(attributeData, header.xOffset - header.attributeOffset, header.numberOfStitches);

    embFile_seek(file, header.xOffset, SEEK_SET);
    xData = (unsigned char*)malloc(header.yOffset - header.xOffset);
    if(!xData) { embLog_error("format-vip.c readVip(), cannot allocate memory for xData\n"); return 0; }
    binaryReadBytes(file, xData, header.yOffset - header.xOffset); /* TODO: check return value */
    xDecompressed = vipDecompressData(xData, header.yOffset - header.xOffset, header.numberOfStitches);

    embFile_seek(file, header.yOffset, SEEK_SET);
    yData = (unsigned char*)malloc(fileLength - header.yOffset);
    if(!yData) { embLog_error("format-vip.c readVip(), cannot allocate memory for yData\n"); return 0; }
    binaryReadBytes(file, yData, fileLength - header.yOffset); /* TODO: check return value */
    yDecompressed = vipDecompressData(yData, fileLength - header.yOffset, header.numberOfStitches);

    for(i = 0; i < header.numberOfStitches; i++)
    {
        embPattern_addStitchRel(pattern,
                                vipDecodeByte(xDecompressed[i]) / 10.0,
                                vipDecodeByte(yDecompressed[i]) / 10.0,
                                vipDecodeStitchType(attributeDataDecompressed[i]), 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);

    embFile_close(file);

    free(attributeData);             attributeData = 0;
    free(xData);                     xData = 0;
    free(yData);                     yData = 0;
    free(attributeDataDecompressed); attributeDataDecompressed = 0;
    free(xDecompressed);             xDecompressed = 0;
    free(yDecompressed);             yDecompressed = 0;

    return 1;
}

static unsigned char* vipCompressData(unsigned char* input, int decompressedInputSize, int* compressedSize)
{
    unsigned char* compressedData = (unsigned char*)malloc(sizeof(unsigned char)*decompressedInputSize*2);
    if(!compressedData)
    {
        embLog_error("format-vip.c vipCompressData(), cannot allocate memory for compressedData\n");
        return 0;
    }
    *compressedSize = husCompress(input, (unsigned long) decompressedInputSize, compressedData, 10, 0);
    return compressedData;
}

static unsigned char vipEncodeByte(double f)
{
    return (unsigned char)(int)roundDouble(f);
}

static unsigned char vipEncodeStitchType(int st)
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

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeVip(EmbPattern* pattern, const char* fileName)
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
    unsigned char* attributeCompressed = 0, *xCompressed = 0, *yCompressed = 0, *decodedColors = 0, *encodedColors = 0;
    unsigned char prevByte = 0;
    EmbThreadList* colorPointer = 0;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-vip.c writeVip(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-vip.c writeVip(), fileName argument is null\n"); return 0; }

    stitchCount = embStitchList_count(pattern->stitchList);
    if(!stitchCount)
    {
        embLog_error("format-vip.c writeVip(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch && pattern->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchRel(pattern, 0, 0, END, 1);
        stitchCount++;
    }

    file = embFile_open(fileName, "wb");
    if(file == 0)
    {
        embLog_error("format-vip.c writeVip(), cannot open %s for writing\n", fileName);
        return 0;
    }

    minColors = embThreadList_count(pattern->threadList);
    decodedColors = (unsigned char*)malloc(minColors << 2);
    if(!decodedColors) return 0;
    encodedColors = (unsigned char*)malloc(minColors << 2);
    if(encodedColors) /* TODO: review this line. It looks clearly wrong. If not, note why. */
    {
        free(decodedColors);
        decodedColors = 0;
        return 0;
    }
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
    if(xValues && yValues && attributeValues)
    {
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

        for(i = 0; i < minColors; i++)
        {
            int byteChunk = i << 2;
            EmbColor currentColor = colorPointer->thread.color;
            decodedColors[byteChunk] = currentColor.r;
            decodedColors[byteChunk + 1] = currentColor.g;
            decodedColors[byteChunk + 2] = currentColor.b;
            decodedColors[byteChunk + 3] = 0x01;
            colorPointer = colorPointer->next;
        }

        for(i = 0; i < minColors << 2; ++i)
        {
            unsigned char tmpByte = (unsigned char) (decodedColors[i] ^ vipDecodingTable[i]);
            prevByte = (unsigned char) (tmpByte ^ prevByte);
            binaryWriteByte(file, prevByte);
        }
        for(i = 0; i <= minColors; i++)
        {
            binaryWriteInt(file, 1);
        }
        binaryWriteUInt(file, 0); /* string length */
        binaryWriteShort(file, 0);
        binaryWriteBytes(file, (char*) attributeCompressed, attributeSize);
        binaryWriteBytes(file, (char*) xCompressed, xCompressedSize);
        binaryWriteBytes(file, (char*) yCompressed, yCompressedSize);
    }

    if(attributeCompressed) { free(attributeCompressed); attributeCompressed = 0; }
    if(xCompressed) { free(xCompressed); xCompressed = 0; }
    if(yCompressed) { free(yCompressed); yCompressed = 0; }

    if(attributeValues) { free(attributeValues); attributeValues = 0; }
    if(xValues) { free(xValues); xValues = 0; }
    if(yValues) { free(yValues); yValues = 0; }

    if(decodedColors) { free(decodedColors); decodedColors = 0; }
    if(encodedColors) { free(encodedColors); encodedColors = 0; }

    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
