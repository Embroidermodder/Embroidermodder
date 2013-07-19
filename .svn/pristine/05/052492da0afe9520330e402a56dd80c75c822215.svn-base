#include "format-dat.h"
#include "helpers-binary.h"

int readDat(EmbPattern* pattern, const char* fileName)
{
    int fileLength, stitchesRemaining;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);
    fseek(file, 0x00, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0x02, SEEK_SET);
    stitchesRemaining = binaryReadUInt16(file);
    fseek(file, 0x100, SEEK_SET);

    while(ftell(file)< fileLength)
    {
        int b1 = (int)binaryReadUInt8(file);
        int b2 = (int)binaryReadUInt8(file);
        unsigned char b0 = binaryReadByte(file);

        int stitchType = NORMAL;
        stitchesRemaining--;

        if((b0 & 0x02) == 0) stitchType = TRIM;

        if(b0 == 0x87)
        {
            stitchType = STOP;
        }
        if(b0 == 0xF8)
        {
            break;
        }
        if(b1 >= 0x80)
        {
            b1 = -(b1 & 0x7F);
        }
        if(b2 >= 0x80)
        {
            b2 = -(b2 & 0x7F);
        }
        embPattern_addStitchRel(pattern, b1 / 10.0, b2 / 10.0, stitchType, 1);
    }
    fclose(file);
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    return 1;
}

int writeDat(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeDat */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
