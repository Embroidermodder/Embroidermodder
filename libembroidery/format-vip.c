#include "format-vip.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include "emb-compress.h"
#include "emb-file.h"
#include "emb-logging.h"
#include <stdlib.h>

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
    if(pattern->lastStitch->stitch.flags != END)
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
