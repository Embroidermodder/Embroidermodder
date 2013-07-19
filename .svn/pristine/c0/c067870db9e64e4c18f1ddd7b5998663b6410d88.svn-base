#include "format-emd.h"
#include "helpers-binary.h"

char emdDecode(unsigned char inputByte)
{
    return (inputByte >= 0x80) ? ((-~inputByte) - 1) : inputByte;
}

int readEmd(EmbPattern* pattern, const char* fileName)
{
    unsigned char b0 = 0, b1 = 0;
    char dx = 0, dy = 0;
    int flags = NORMAL;
    char endOfStream = 0;
    unsigned char jemd0[6];
    int width, height, colors;
    int i;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

    binaryReadBytes(file, jemd0, 6);
    width = binaryReadInt16(file);
    height = binaryReadInt16(file);
    colors = binaryReadInt16(file);

    fseek(file, 0x30, SEEK_SET);

    for(i = 0; !endOfStream; i++)
    {
            flags = NORMAL;
            b0 = binaryReadUInt8(file);
            b1 = binaryReadUInt8(file);

            if(b0 == 0x80)
            {
                if(b1 == 0x2A)
                {
                    embPattern_addStitchRel(pattern, 0, 0, STOP, 1);
                    continue;
                }
                else if((b1 == 0x80))
                {
                    b0 = binaryReadUInt8(file);
                    b1 = binaryReadUInt8(file);
                    flags = TRIM;
                }
                else if(b1 == 0xFD)
                {
                    embPattern_addStitchRel(pattern, 0, 0, END, 1);
                    break;
                }
                else
                {
                    continue;
                }
            }
            dx = emdDecode(b0);
            dy = emdDecode(b1);
            embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);
    return 1;
}

int writeEmd(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WriteEmd */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
