#include "format-bro.h"
#include "helpers-binary.h"

int readBro(EmbPattern* pattern, const char* fileName)
{
    unsigned char x55;
    short unknown1, unknown2, unknown3, unknown4, moreBytesToEnd;
    char name[8];
    int stitchType;
    FILE* filein;

    filein = fopen(fileName,"rb");
    if(filein==0)
    {
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

    x55 = binaryReadByte(filein);
    unknown1 = binaryReadInt16(filein);

    fread(name, 1, 8, filein);
    unknown2 = binaryReadInt16(filein);
    unknown3 = binaryReadInt16(filein);
    unknown4 = binaryReadInt16(filein);
    moreBytesToEnd = binaryReadInt16(filein);

    fseek(filein, 0x100, SEEK_SET);

    while(1)
    {
        short b1, b2;
        stitchType = NORMAL;
        b1 = binaryReadByte(filein);
        b2 = binaryReadByte(filein);
        if (b1 == -128)
        {
            unsigned char bCode = binaryReadByte(filein);
            b1 = binaryReadInt16(filein);
            b2 = binaryReadInt16(filein);
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
    fclose(filein);
    return 1;
}

int writeBro(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeBro */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
