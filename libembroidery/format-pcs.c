#include "format-pcs.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include "helpers-misc.h"

static double pcsDecode(unsigned char a1, unsigned char a2, unsigned char a3)
{
    int res = a1 + (a2 << 8) + (a3 << 16);
    if(res > 0x7FFFFF)
    {
        return (-((~(res) & 0x7FFFFF) - 1));
    }
    return res;
}

static void pcsEncode(EmbFile* file, int dx, int dy, int flags)
{
    unsigned char flagsToWrite = 0;

    if(!file) { embLog_error("format-pcs.c pcsEncode(), file argument is null\n"); return; }

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

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPcs(EmbPattern* pattern, const char* fileName)
{
    char allZeroColor = 1;
    int i = 0;
    unsigned char b[9];
    double dx = 0, dy = 0;
    int flags = 0, st = 0;
    unsigned char version, hoopSize;
    unsigned short colorCount;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-pcs.c readPcs(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pcs.c readPcs(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-pcs.c readPcs(), cannot open %s for reading\n", fileName);
        return 0;
    }
    version = binaryReadByte(file);
    hoopSize = binaryReadByte(file);  /* 0 for PCD, 1 for PCQ (MAXI), 2 for PCS with small hoop(80x80), */
                                      /* and 3 for PCS with large hoop (115x120) */

    switch(hoopSize)
    {
        case 2:
            pattern->hoop.width = 80.0;
            pattern->hoop.height = 80.0;
            break;
        case 3:
            pattern->hoop.width = 115;
            pattern->hoop.height = 120.0;
            break;
    }

    colorCount = binaryReadUInt16(file);

    for(i = 0; i < colorCount; i++)
    {
        EmbThread t;
		t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        t.color.b = binaryReadByte(file);
        t.catalogNumber = "";
        t.description = "";
        if(t.color.r || t.color.g || t.color.b)
        {
            allZeroColor = 0;
        }
        embPattern_addThread(pattern, t);
        binaryReadByte(file);
    }
    if(allZeroColor)
        embPattern_loadExternalColorFile(pattern, fileName);
    st = binaryReadUInt16(file);
    /* READ STITCH RECORDS */
    for(i = 0; i < st; i++)
    {
        flags = NORMAL;
        if(embFile_read(b, 1, 9, file) != 9)
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
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePcs(EmbPattern* pattern, const char* fileName)
{
    EmbStitchList* pointer = 0;
    EmbThreadList* threadPointer = 0;
    EmbFile* file = 0;
    int i = 0;
    unsigned char colorCount = 0;
    double xx = 0.0, yy = 0.0;

    if(!pattern) { embLog_error("format-pcs.c writePcs(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pcs.c writePcs(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-pcs.c writePcs(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-pcs.c writePcs(), cannot open %s for writing\n", fileName);
        return 0;
    }

    binaryWriteByte(file, (unsigned char)'2');
    binaryWriteByte(file, 3); /* TODO: select hoop size defaulting to Large PCS hoop */
    colorCount = (unsigned char)embThreadList_count(pattern->threadList);
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

    binaryWriteUShort(file, (unsigned short)embStitchList_count(pattern->stitchList));
    /* write stitches */
    xx = yy = 0;
    pointer = pattern->stitchList;
    while(pointer)
    {
        pcsEncode(file, roundDouble(pointer->stitch.xx * 10.0), roundDouble(pointer->stitch.yy * 10.0), pointer->stitch.flags);
        pointer = pointer->next;
    }
    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
