#include "format-bro.h"
#include "helpers-binary.h"
#include "emb-logging.h"

int readBro(EmbPattern* pattern, const char* fileName)
{
    unsigned char x55;
    short unknown1, unknown2, unknown3, unknown4, moreBytesToEnd;
    char name[8];
    int stitchType;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-bro.c readBro(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-bro.c readBro(), fileName argument is null\n"); return 0; }

    file = fopen(fileName,"rb");
    if(!file)
    {
        embLog_error("format-bro.c readBro(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);

    x55 = binaryReadByte(file);
    unknown1 = binaryReadInt16(file);

    fread(name, 1, 8, file);
    unknown2 = binaryReadInt16(file);
    unknown3 = binaryReadInt16(file);
    unknown4 = binaryReadInt16(file);
    moreBytesToEnd = binaryReadInt16(file);

    fseek(file, 0x100, SEEK_SET);

    while(1)
    {
        short b1, b2;
        stitchType = NORMAL;
        b1 = binaryReadByte(file);
        b2 = binaryReadByte(file);
        if (b1 == -128)
        {
            unsigned char bCode = binaryReadByte(file);
            b1 = binaryReadInt16(file);
            b2 = binaryReadInt16(file);
            if (bCode == 2)
            {
                stitchType = STOP;
            }
            else if (bCode == 3)
            {
                stitchType = TRIM;
            }
            else if (bCode == 0x7E)
            {
                embPattern_addStitchRel(pattern, 0, 0, END, 1);
                break;
            }
        }
        embPattern_addStitchRel(pattern, b1 / 10.0, b2 / 10.0, stitchType, 1);
    }
    fclose(file);
    return 1;
}

int writeBro(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-bro.c writeBro(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-bro.c writeBro(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeBro */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
