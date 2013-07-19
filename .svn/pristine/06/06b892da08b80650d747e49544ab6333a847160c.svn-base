#include "format-vp3.h"
#include "helpers-binary.h"
#include <stdlib.h>
#include <stdio.h>

unsigned char* ReadString(FILE* file)
{
    int stringLength = binaryReadInt16BE(file);
    unsigned char* charString = (unsigned char*)malloc(stringLength);
    binaryReadBytes(file, charString, stringLength);
    return charString;
}

int vp3Decode(unsigned char inputByte)
{
    return (inputByte > 0x80) ? -((unsigned char) (~inputByte) + 1) : inputByte;
}

short vp3DecodeInt16(short input)
{
    if((unsigned short)input > 0x8000)
    {
        return -((short) ((~input) + 1));
    }
    return (input);
}

void vp3ReadHoopSection(FILE* file)
{
    int right = binaryReadInt32BE(file);
    int bottom = binaryReadInt32BE(file);
    int left = binaryReadInt32BE(file);
    int top = binaryReadInt32BE(file);

    int unknown1 = binaryReadInt32BE(file);
    int unknown2 = binaryReadInt32BE(file);
    int unknown3 = binaryReadInt32BE(file);
    int numberOfBytesRemaining = binaryReadInt32BE(file);

    int xOffset = binaryReadInt32BE(file);
    int yOffset = binaryReadInt32BE(file);

    unsigned char byte1 = binaryReadByte(file);
    unsigned char byte2 = binaryReadByte(file);
    unsigned char byte3 = binaryReadByte(file);

    /* Centered hoop dimensions */
    int right2 = binaryReadInt32BE(file);
    int left2 = binaryReadInt32BE(file);
    int bottom2 = binaryReadInt32BE(file);
    int top2 = binaryReadInt32BE(file);

    int width = binaryReadInt32BE(file);
    int height = binaryReadInt32BE(file);
}

int readVp3(EmbPattern* pattern, const char* fileName)
{
    unsigned char magicString[5];
    unsigned char some, *someString;
    unsigned char v1, v2,v3,v4,v5,v6,v7,v8,v9,v10, v11, v12,v13,v14,v15,v16,v17,v18;
    unsigned char* unknownString3;
    int numberOfColors;
    long colorSectionOffset;
    unsigned char magicCode[6];
    short someShort;
    unsigned char someByte;
    int bytesRemainingInFile;
    unsigned char* unknownByteString;
    int hoopConfigurationOffset;
    unsigned char* unknownString2;
    int i;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        /*TODO: set messages here "Error opening VP3 file for read:" */
        return 0;
    }

    binaryReadBytes(file, magicString, 5); /* %vsm% */

    some = binaryReadByte(file); /* 0 */
    someString = ReadString(file);
    someShort = binaryReadInt16(file);
    someByte = binaryReadByte(file);
    bytesRemainingInFile = binaryReadInt32(file);
    unknownByteString = ReadString(file);
    hoopConfigurationOffset =(int) ftell(file);

    vp3ReadHoopSection(file);

    unknownString2 = ReadString(file);

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

    unknownString3 = ReadString(file);

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

        t.color.b = binaryReadByte(file);
        t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        embPattern_addThread(pattern, t);
        fseek(file, 6*tableSize, SEEK_CUR);

        str1 = ReadString(file);
        str2 = ReadString(file);
        str3 = ReadString(file);

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


int writeVp3(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WriteVp3 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
