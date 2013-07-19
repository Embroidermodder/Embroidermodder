#include "format-pcs.h"
#include "helpers-binary.h"
#include "helpers-misc.h"

double pcsDecode(unsigned char a1, unsigned char a2, unsigned char a3)
{
    int res = a1 + (a2 << 8) + (a3 << 16);
    if(res > 0x7FFFFF)
    {
        return (-((~(res) & 0x7FFFFF) - 1));
    }
    return res;
}

void pcsEncode(FILE * file, int dx, int dy, int flags)
{
    unsigned char flagsToWrite = 0;
    binaryWriteByte(file, (unsigned char)0);
    binaryWriteByte(file, (unsigned char)(dx & 0xFF));
    binaryWriteByte(file, (unsigned char)((dx >> 8) & 0xFF));
    binaryWriteByte(file, (unsigned char)((dx >> 16) & 0xFF));
    
    binaryWriteByte(file, (unsigned char)0);
    binaryWriteByte(file, (unsigned char)(dy & 0xFF));
    binaryWriteByte(file, (unsigned char)((dy >> 8) & 0xFF));
    binaryWriteByte(file, (unsigned char)((dy >> 16) & 0xFF));
    if(flags & STOP)
    {
        flagsToWrite |= 0x01;
    }
    if(flags & TRIM)
    {
        flagsToWrite |= 0x04;
    }
    binaryWriteByte(file, flagsToWrite);
}

int readPcs(EmbPattern* pattern, const char* fileName)
{
    char allZeroColor = 1;
    int i;
    unsigned char b[9];
    double dx = 0, dy = 0;
    int flags = 0, st = 0;
    unsigned char version, hoopSize;
    unsigned short colorCount;
    FILE* file;

    file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    version = binaryReadByte(file);
    hoopSize = binaryReadByte(file);  /* 0 for PCD, 1 for PCQ (MAXI), 2 for PCS with small hoop(80x80), */
                                      /* and 3 for PCS with large hoop (115x120) */
    colorCount = binaryReadUInt16(file);

    for(i = 0; i < colorCount; i++)
    {
        EmbThread t;
        t.color.r = fgetc(file);
        t.color.g = fgetc(file);
        t.color.b = fgetc(file);
        t.catalogNumber = "";
        t.description = "";
        if(t.color.r || t.color.g || t.color.b)
        {
            allZeroColor = 0;
        }
        embPattern_addThread(pattern, t);
        fgetc(file);
    }
    if(allZeroColor)
        embPattern_loadExternalColorFile(pattern, fileName);
    st = binaryReadUInt16(file);
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
        dx = pcsDecode(b[1], b[2], b[3]);
        dy = pcsDecode(b[5], b[6], b[7]);
        embPattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);
    return 1;
}

int writePcs(EmbPattern* pattern, const char* fileName)
{
    EmbStitchList* pointer;
    EmbThreadList* threadPointer;
    FILE* file;
    int flags = 0, i;
    unsigned char colorCount;
    double xx = 0.0, yy = 0.0, dx = 0.0, dy = 0.0;

    file = fopen(fileName, "wb");
    if(file == 0)
    {
        return 0;
    }
    binaryWriteByte(file, (unsigned char)'2');
    binaryWriteByte(file, 3); /* TODO: select hoop size defaulting to Large PCS hoop */
    colorCount = embThread_count(pattern->threadList);
    binaryWriteUShort(file, (unsigned short)colorCount);
    threadPointer = pattern->threadList;
    i = 0;
    while(threadPointer)
    {
        EmbColor color = threadPointer->thread.color;
        binaryWriteByte(file, color.r);
        binaryWriteByte(file, color.g);
        binaryWriteByte(file, color.b);
        binaryWriteByte(file, 0);
        threadPointer = threadPointer->next;
        i++;
    }

    for(; i < 16; i++)
    {
        binaryWriteUInt(file, 0); /* write remaining colors to reach 16 */
    }

    binaryWriteUShort(file, (unsigned int)embStitch_count(pattern->stitchList));
    /* write stitches */
    xx = yy = 0;
    pointer = pattern->stitchList;
    while(pointer)
    {
        pcsEncode(file, pointer->stitch.xx * 10.0, pointer->stitch.yy * 10.0, pointer->stitch.flags);
        pointer = pointer->next;
    }
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
