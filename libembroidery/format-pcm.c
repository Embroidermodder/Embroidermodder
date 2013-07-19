#include "format-pcm.h"
#include "helpers-binary.h"
#include "helpers-misc.h"

double pcmDecode(unsigned char a1, unsigned char a2, unsigned char a3, unsigned char a4)
{
    int res = a1 + (a2 << 8) + (a3 << 16);
    if(res > 0x7FFFFF)
    {
        return (-((~(res) & 0x7FFFFF) - 1));
    }
    return res;
}

int readPcm(EmbPattern* pattern, const char* fileName)
{
    int i;
    unsigned char b[9];
    double dx = 0, dy = 0;
    int flags = 0, st = 0;
    FILE* file = fopen(fileName, "rb");

    if(file == 0)
    {
        return 0;
    }

    fseek(file, 4, SEEK_SET);

    for(i = 0; i < 16; i++)
    {
        char zero = fgetc(file);
        int colorNumber = fgetc(file);
        embPattern_addThread(pattern, pcmThreads[colorNumber]);
    }
    st = binaryReadUInt16BE(file);
    /* READ STITCH RECORDS */
    for(i = 0; i < st; i++)
    {
        flags = NORMAL;
        if(fread(b, 1, 9, file) != 9)
            break;

        if(b[8] & 0x01)
        {
            flags = STOP;
        }
        else if(b[8] & 0x04)
        {
            flags = TRIM;
        }
        else if(b[8] != 0)
        {
            /* TODO: ONLY INTERESTED IN THIS CASE TO LEARN MORE ABOUT THE FORMAT */
        }
        dx = pcmDecode(b[2], b[1], b[0], b[8]);
        dy = pcmDecode(b[6], b[5], b[4], b[8]);
        embPattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);
    return 1;
}

int writePcm(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writePcm */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
