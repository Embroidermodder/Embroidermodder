#include "format-thr.h"
#include "helpers-binary.h"
#include "emb-file.h"
#include "emb-logging.h"
#include <string.h>

#define NOTFRM 0x00080000

typedef struct ThredHeader_     /* thred file header */
{
    unsigned int sigVersion;    /* signature and version */
    unsigned int length;        /* length of ThredHeader + length of stitch data */
    unsigned short numStiches;  /* number of stitches */
    unsigned short hoopSize;    /* size of hoop */
    unsigned short reserved[7]; /* reserved for expansion */
} ThredHeader;

typedef struct ThredExtension_  /* thred v1.0 file header extension */
{
    float hoopX;                /* hoop size x dimension in 1/6 mm units */
    float hoopY;                /* hoop size y dimension in 1/6 mm units */
    float stitchGranularity;    /* stitches per millimeter--not implemented */
    char creatorName[50];       /* name of the file creator */
    char modifierName[50];      /* name of last file modifier */
    char auxFormat;             /* auxillary file format, 0=PCS,1=DST,2=PES */
    char reserved[31];          /* reserved for expansion */
} ThredExtension;

/*
bit definitions for attributes of stitch
0-3     stitch color
4-14    form pointer
15-18   spares
19      not a form stitch
20      center walk stitch
21      edge walk stitch
22      underlay stitch
23      knot stitch
24      feather stitch
25-27   layer
28      spare
29-30   stitch type 00=not a form stitch, 01=form fill, 10=form border fill, 11=applique stitches
31      set for user edited stitches
*/

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readThr(EmbPattern* pattern, const char* fileName)
{
    ThredHeader header;
    unsigned char r, g, b;
    int currentColor;
    int i;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-thr.c readThr(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-thr.c readThr(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-thr.c readThr(), cannot open %s for reading\n", fileName);
        return 0;
    }

    header.sigVersion  = binaryReadUInt32(file);
    header.length      = binaryReadUInt32(file);
    header.numStiches  = binaryReadUInt16(file);
    header.hoopSize    = binaryReadUInt16(file);
    header.reserved[0] = binaryReadUInt16(file);
    header.reserved[1] = binaryReadUInt16(file);
    header.reserved[2] = binaryReadUInt16(file);
    header.reserved[3] = binaryReadUInt16(file);
    header.reserved[4] = binaryReadUInt16(file);
    header.reserved[5] = binaryReadUInt16(file);
    header.reserved[6] = binaryReadUInt16(file);

    if((header.sigVersion & 0xffffff) == 0x746872)
    {
        unsigned int verVar = (header.sigVersion & 0xff000000) >> 24;
        switch(verVar)
        {
            case 0:
                break;
            case 1:
            case 2:
                embFile_seek(file, 144, SEEK_CUR); /* skip the file header extension */
                break;
            default:
                return 0; /* unsuported version */
        }
    }

    currentColor = -1;
    for(i = 0; i < header.numStiches; i++)
    {
        int type = NORMAL;
        float x = binaryReadFloat(file) / 10.0f;
        float y = binaryReadFloat(file) / 10.0f;
        unsigned int color = binaryReadUInt32(file);

        if((int)(color & 0xF) != currentColor)
        {
            currentColor = (int)color & 0xF;
            embPattern_changeColor(pattern, currentColor);
            type = STOP | TRIM;
        }
        embPattern_addStitchAbs(pattern, x, y, type, 0);
    }
    embFile_seek(file, 16, SEEK_CUR); /* skip bitmap name (16 chars) */

    r = binaryReadByte(file);
    g = binaryReadByte(file);
    b = binaryReadByte(file);
    binaryReadByte(file);

    for(i = 0; i < 16; i++)
    {
        EmbThread thread;
        thread.description = NULL;
        thread.catalogNumber = NULL;
        thread.color.r = binaryReadByte(file);
        thread.color.g = binaryReadByte(file);
        thread.color.b = binaryReadByte(file);
        binaryReadByte(file);
        embPattern_addThread(pattern, thread);
    }
    /*  64 bytes of rgbx(4 bytes) colors (16 custom colors) */
    /*  16 bytes of thread size (ascii representation ie. '4') */

    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeThr(EmbPattern* pattern, const char* fileName)
{
    int i, stitchCount;
    unsigned char version = 0;
    ThredHeader header;
    ThredExtension extension;
    char bitmapName[16];
    EmbStitchList* pointer = 0;
    EmbThreadList* colorpointer = 0;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-thr.c writeThr(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-thr.c writeThr(), fileName argument is null\n"); return 0; }

    stitchCount = embStitchList_count(pattern->stitchList);
    if(!stitchCount)
    {
        embLog_error("format-thr.c writeThr(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchRel(pattern, 0, 0, END, 1);
        stitchCount++;
    }

    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-thr.c writeThr(), cannot open %s for writing\n", fileName);
        return 0;
    }

    memset(&header, 0, sizeof(ThredHeader));
    header.sigVersion = 0x746872 | (version << 24);
    header.length = stitchCount * 12 + 16;
    if(version == 1 || version == 2)
    {
        header.length = header.length + sizeof(ThredHeader);
    }
    header.numStiches = (unsigned short)stitchCount; /* number of stitches in design */
    header.hoopSize = 5;

    binaryWriteUInt(file, header.sigVersion);
    binaryWriteUInt(file, header.length);
    binaryWriteUShort(file, header.numStiches);
    binaryWriteUShort(file, header.hoopSize);
    binaryWriteUShort(file, header.reserved[0]);
    binaryWriteUShort(file, header.reserved[1]);
    binaryWriteUShort(file, header.reserved[2]);
    binaryWriteUShort(file, header.reserved[3]);
    binaryWriteUShort(file, header.reserved[4]);
    binaryWriteUShort(file, header.reserved[5]);
    binaryWriteUShort(file, header.reserved[6]);

    if(version == 1 || version == 2)
    {
        memset(&extension, 0, sizeof(ThredExtension));
        extension.auxFormat = 1;
        extension.hoopX = 640;
        extension.hoopY = 640;

        binaryWriteFloat(file, extension.hoopX);
        binaryWriteFloat(file, extension.hoopY);
        binaryWriteFloat(file, extension.stitchGranularity);
        binaryWriteBytes(file, extension.creatorName, 50);
        binaryWriteBytes(file, extension.modifierName, 50);
        binaryWriteByte(file, extension.auxFormat);
        binaryWriteBytes(file, extension.reserved, 31);
    }

    /* write stitches */
    i = 0;
    pointer = pattern->stitchList;
    while(pointer)
    {
        binaryWriteFloat(file, (float)(pointer->stitch.xx * 10.0));
        binaryWriteFloat(file, (float)(pointer->stitch.yy * 10.0));
        binaryWriteUInt(file, NOTFRM | (pointer->stitch.color & 0x0F));
        pointer = pointer->next;
        i++;
        if(i >= stitchCount) break;
    }
    binaryWriteBytes(file, bitmapName, 16);
    /* background color */
    binaryWriteByte(file, 0xFF); /* r */
    binaryWriteByte(file, 0xFF); /* g */
    binaryWriteByte(file, 0xFF); /* b */
    binaryWriteByte(file, 0x00);

    i = 0;
    colorpointer = pattern->threadList;
    while(colorpointer)
    {
        binaryWriteByte(file, colorpointer->thread.color.r);
        binaryWriteByte(file, colorpointer->thread.color.g);
        binaryWriteByte(file, colorpointer->thread.color.b);
        binaryWriteByte(file, 0);
        colorpointer = colorpointer->next;
        i++;
        if(i >= 16) break;
    }

    /* write remaining colors if not yet 16 */
    for(; i < 16; i++)
    {
        binaryWriteUInt(file, 0);
    }

    /* write custom colors */
    i = 0;
    colorpointer = pattern->threadList;
    while(colorpointer)
    {
        binaryWriteByte(file, colorpointer->thread.color.r);
        binaryWriteByte(file, colorpointer->thread.color.g);
        binaryWriteByte(file, colorpointer->thread.color.b);
        binaryWriteByte(file, 0);
        colorpointer = colorpointer->next;
        i++;
        if(i >= 16) break;
    }

    /* write remaining colors if not yet 16 */
    for(; i < 16; i++)
    {
        binaryWriteUInt(file, 0);
    }

    for(i = 0; i < 16; i++)
    {
        binaryWriteByte(file, '4');
    }

    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
