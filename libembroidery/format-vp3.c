#include "format-vp3.h"
#include "helpers-binary.h"
#include "emb-logging.h"
#include <stdlib.h>
#include <stdio.h>

static unsigned char* vp3ReadString(FILE* file)
{
    int stringLength = binaryReadInt16BE(file);
    unsigned char* charString = (unsigned char*)malloc(stringLength);
    if(!charString) { embLog_error("format-vp3.c vp3ReadString(), cannot allocate memory for charString\n"); return 0; }
    binaryReadBytes(file, charString, stringLength);
    return charString;
}

static int vp3Decode(unsigned char inputByte)
{
    return (inputByte > 0x80) ? -((unsigned char) (~inputByte) + 1) : inputByte;
}

static short vp3DecodeInt16(short input)
{
    if((unsigned short)input > 0x8000)
    {
        return -((short) ((~input) + 1));
    }
    return (input);
}

typedef struct _vp3Hoop
{
    int right;
    int bottom;
    int left;
    int top;
    int unknown1;
    int unknown2;
    int unknown3;
    int numberOfBytesRemaining;

    int xOffset;
    int yOffset;

    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;

    /* Centered hoop dimensions */
    int right2;
    int left2;
    int bottom2;
    int top2;

    int width;
    int height;
} vp3Hoop;

static vp3Hoop vp3ReadHoopSection(FILE* file)
{
    vp3Hoop hoop;
    hoop.right = binaryReadInt32BE(file);
    hoop.bottom = binaryReadInt32BE(file);
    hoop.left = binaryReadInt32BE(file);
    hoop.top = binaryReadInt32BE(file);

    hoop.unknown1 = binaryReadInt32BE(file);
    hoop.unknown2 = binaryReadInt32BE(file);
    hoop.unknown3 = binaryReadInt32BE(file);
    hoop.numberOfBytesRemaining = binaryReadInt32BE(file);

    hoop.xOffset = binaryReadInt32BE(file);
    hoop.yOffset = binaryReadInt32BE(file);

    hoop.byte1 = binaryReadByte(file);
    hoop.byte2 = binaryReadByte(file);
    hoop.byte3 = binaryReadByte(file);

    /* Centered hoop dimensions */
    hoop.right2 = binaryReadInt32BE(file);
    hoop.left2 = binaryReadInt32BE(file);
    hoop.bottom2 = binaryReadInt32BE(file);
    hoop.top2 = binaryReadInt32BE(file);

    hoop.width = binaryReadInt32BE(file);
    hoop.height = binaryReadInt32BE(file);
    return hoop;
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readVp3(EmbPattern* pattern, const char* fileName)
{
    unsigned char magicString[5];
    unsigned char some;
    unsigned char* someString = 0;
    unsigned char v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18;
    unsigned char* unknownString3 = 0;
    int numberOfColors;
    long colorSectionOffset;
    unsigned char magicCode[6];
    short someShort;
    unsigned char someByte;
    int bytesRemainingInFile;
    unsigned char* unknownByteString = 0;
    int hoopConfigurationOffset;
    unsigned char* unknownString2 = 0;
    int i;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-vp3.c readVp3(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-vp3.c readVp3(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "rb");
    if(!file)
    {
        embLog_error("format-vp3.c readVp3(), cannot open %s for reading\n", fileName);
        return 0;
    }

    binaryReadBytes(file, magicString, 5); /* %vsm% */

    some = binaryReadByte(file); /* 0 */
    someString = vp3ReadString(file);
    someShort = binaryReadInt16(file);
    someByte = binaryReadByte(file);
    bytesRemainingInFile = binaryReadInt32(file);
    unknownByteString = vp3ReadString(file);
    hoopConfigurationOffset =(int) ftell(file);

    vp3ReadHoopSection(file);

    unknownString2 = vp3ReadString(file);

    /* TODO: review v1 thru v18 variables and use emb_unused() if needed */
    v1 = binaryReadByte(file);
    v2 = binaryReadByte(file);
    v3 = binaryReadByte(file);
    v4 = binaryReadByte(file);
    v5 = binaryReadByte(file);
    v6 = binaryReadByte(file);
    v7 = binaryReadByte(file);
    v8 = binaryReadByte(file);
    v9 = binaryReadByte(file);
    v10 = binaryReadByte(file);
    v11 = binaryReadByte(file);
    v12 = binaryReadByte(file);
    v13 = binaryReadByte(file);
    v14 = binaryReadByte(file);
    v15 = binaryReadByte(file);
    v16 = binaryReadByte(file);
    v17 = binaryReadByte(file);
    v18 = binaryReadByte(file);

    binaryReadBytes(file, magicCode, 6); /* 0x78 0x78 0x55 0x55 0x01 0x00 */

    unknownString3 = vp3ReadString(file);

    numberOfColors = binaryReadInt16BE(file);
    colorSectionOffset = (int) ftell(file);

    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread t;
        char tableSize;
        int unknownX, unknownY, unknownX2, unknownY2;
        unsigned char* str1, *str2, *str3;
        int unknownThreadString, numberOfBytesInColor;

        fseek(file, 0x03 + colorSectionOffset, SEEK_SET);
        colorSectionOffset = binaryReadInt32BE(file);
        colorSectionOffset += ftell(file);
        unknownX = binaryReadInt32BE(file);
        unknownY = binaryReadInt32BE(file);
        /* TODO: review commented section below
        pattern.embPattern_addStitchAbsolute(-unknownX / 100, -unknownY/ 100, StitchType.Jump, false);
        fs.Seek(0x08, SeekOrigin.Current);
        */
        tableSize = binaryReadByte(file);
        binaryReadByte(file);
        t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        t.color.b = binaryReadByte(file);
        embPattern_addThread(pattern, t);
        fseek(file, 6*tableSize - 1, SEEK_CUR);

        str1 = vp3ReadString(file);
        str2 = vp3ReadString(file);
        str3 = vp3ReadString(file);

        unknownX2 = binaryReadInt32BE(file);
        unknownY2 = binaryReadInt32BE(file);
        /*fs.Seek(0x02, SeekOrigin.Current); */
        unknownThreadString = binaryReadInt16BE(file);
        fseek(file, unknownThreadString, SEEK_CUR);
        numberOfBytesInColor = binaryReadInt32BE(file);
        fseek(file, 0x3, SEEK_CUR);
        while(ftell(file) < colorSectionOffset - 1)
        {
            int x = vp3Decode(binaryReadByte(file));
            int y = vp3Decode(binaryReadByte(file));
            if(x == 0x80)
            {
                switch (y)
                {
                    case 0x00:
                    case 0x03:
                        break;
                    case 0x01:
                        x = vp3DecodeInt16(binaryReadInt16BE(file));
                        y = vp3DecodeInt16(binaryReadInt16BE(file));
                        binaryReadInt16BE(file);
                        embPattern_addStitchRel(pattern, x/ 10.0, y / 10.0, TRIM, 1);
                        break;
                    default:
                        break;
                }
            }
            else
            {
                embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, NORMAL, 1);
            }
        }
        if(i + 1 < numberOfColors) embPattern_addStitchRel(pattern, 0, 0, STOP, 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    embPattern_flipVertical(pattern);
    fclose(file);
    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeVp3(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-vp3.c writeVp3(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-vp3.c writeVp3(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeVp3 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
