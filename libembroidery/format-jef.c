#include "format-jef.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "emb-time.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include "emb-stitch.h"
#include <stdio.h>


#define HOOP_126X110 0
#define	HOOP_110X110 1
#define	HOOP_50X50   2
#define	HOOP_140X200 3
#define HOOP_230X200 4

static int jefGetHoopSize(int width, int height)
{
    if(width <  50 && height <  50) { return HOOP_50X50; }
    if(width < 110 && height < 110) { return HOOP_110X110; }
    if(width < 140 && height < 200) { return HOOP_140X200; }
    return ((int) HOOP_110X110);
}
static char jefDecode(unsigned char inputByte)
{
    if(inputByte >= 0x80)
        return (char) ((-~inputByte) - 1);
    return ((char) inputByte);
}
static void jefSetHoopFromId(EmbPattern* pattern, int hoopCode)
{
    if(!pattern) { embLog_error("format-jef.c jefSetHoopFromId(), pattern argument is null\n"); return; }

    switch(hoopCode)
    {
        case HOOP_126X110:
            pattern->hoop.height = 126.0;
            pattern->hoop.width = 110.0;
            break;
        case HOOP_50X50:
            pattern->hoop.height = 50.0;
            pattern->hoop.width = 50.0;
            break;
        case HOOP_110X110:
           pattern->hoop.height = 110.0;
            pattern->hoop.width = 110.0;
            break;
        case HOOP_140X200:
            pattern->hoop.height = 140.0;
            pattern->hoop.width = 200.0;
            break;
        case HOOP_230X200:
            pattern->hoop.height = 230.0;
            pattern->hoop.width = 200.0;
            break;
    }
}

struct hoop_padding
{
    int left;
    int right;
    int top;
    int bottom;
};

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readJef(EmbPattern* pattern, const char* fileName)
{
    int stitchOffset, formatFlags, numberOfColors, numberOfStitchs;
    int hoopSize, i;
    struct hoop_padding bounds, rectFrom110x110, rectFrom50x50, rectFrom200x140, rect_from_custom;
    int stitchCount;
    char date[8], time[8];
  
    EmbFile* file = 0;
 
    unsigned char b0 = 0, b1 = 0;
    char dx = 0, dy = 0;
    int flags = 0;


    if(!pattern) { embLog_error("format-jef.c readJef(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-jef.c readJef(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-jef.c readJef(), cannot open %s for reading\n", fileName);
        return 0;
    }

    stitchOffset = binaryReadInt32(file);
    formatFlags = binaryReadInt32(file); /* TODO: find out what this means */

    binaryReadBytes(file, (unsigned char*) date, 8); /* TODO: check return value */
    binaryReadBytes(file, (unsigned char*) time, 8); /* TODO: check return value */
    numberOfColors = binaryReadInt32(file);
    numberOfStitchs = binaryReadInt32(file);
    hoopSize = binaryReadInt32(file);
    jefSetHoopFromId(pattern, hoopSize);

    bounds.left = binaryReadInt32(file);
    bounds.top = binaryReadInt32(file);
    bounds.right = binaryReadInt32(file);
    bounds.bottom = binaryReadInt32(file);

    rectFrom110x110.left = binaryReadInt32(file);
    rectFrom110x110.top = binaryReadInt32(file);
    rectFrom110x110.right = binaryReadInt32(file);
    rectFrom110x110.bottom = binaryReadInt32(file);

    rectFrom50x50.left = binaryReadInt32(file);
    rectFrom50x50.top = binaryReadInt32(file);
    rectFrom50x50.right = binaryReadInt32(file);
    rectFrom50x50.bottom = binaryReadInt32(file);

    rectFrom200x140.left = binaryReadInt32(file);
    rectFrom200x140.top = binaryReadInt32(file);
    rectFrom200x140.right = binaryReadInt32(file);
    rectFrom200x140.bottom = binaryReadInt32(file);

    rect_from_custom.left = binaryReadInt32(file);
    rect_from_custom.top = binaryReadInt32(file);
    rect_from_custom.right = binaryReadInt32(file);
    rect_from_custom.bottom = binaryReadInt32(file);

    for(i = 0; i < numberOfColors; i++)
    {
        embPattern_addThread(pattern, jefThreads[binaryReadInt32(file) % 79]);
    }
    embFile_seek(file, stitchOffset, SEEK_SET);
    stitchCount = 0;
    while(stitchCount < numberOfStitchs + 100)
    {
        flags = NORMAL;
        b0 = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
        {
            break;
        }
        b1 = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
        {
            break;
        }
        if(b0 == 0x80)
        {
            if(b1 & 1)
            {
                b0 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                b1 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                flags = STOP;
            }
            else if((b1 == 2) || (b1 == 4) || b1 == 6)
            {
                flags = TRIM;
                b0 = (unsigned char)embFile_getc(file);
                if (embFile_eof(file))
                {
                    break;
                }
                b1 = (unsigned char)embFile_getc(file);
                if (embFile_eof(file))
                {
                    break;
                }
            }
            else if(b1 == 0x10)
            {
                embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
                break;
            }
        }
        dx = jefDecode(b0);
        dy = jefDecode(b1);
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if (pattern->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchRel(pattern, 0, 0, END, 1);
    }
    return 1;
}

static void jefEncode(unsigned char* b, char dx, char dy, int flags)
{
    if(!b)
    {
        embLog_error("format-jef.c expEncode(), b argument is null\n");
        return;
    }
    if(flags == STOP)
    {
        b[0] = 0x80;
        b[1] = 1;
        b[2] = dx;
        b[3] = dy;
    }
    else if (flags == END)
    {
        b[0] = 0x80;
        b[1] = 0x10;
        b[2] = 0;
        b[3] = 0;
    }
    else if(flags == TRIM || flags == JUMP)
    {
        b[0] = 0x80;
        b[1] = 2;
        b[2] = dx;
        b[3] = dy;
    }
    else
    {
        b[0] = dx;
        b[1] = dy;
    }
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeJef(EmbPattern* pattern, const char* fileName)
{
    int colorlistSize, minColors, designWidth, designHeight, i;
    EmbRect boundingRect;
    EmbFile* file = 0;
    EmbTime time;
    EmbThreadList* threadPointer = 0;
    EmbStitchList* stitches = 0;
    double dx = 0.0, dy = 0.0;
    double xx = 0.0, yy = 0.0;
    int flags = 0;
    unsigned char b[4];

    if(!pattern) { embLog_error("format-jef.c writeJef(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-jef.c writeJef(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-jef.c writeJef(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if (pattern->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchRel(pattern, 0, 0, END, 1);
    }
    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-jef.c writeJef(), cannot open %s for writing\n", fileName);
        return 0;
    }

    embPattern_correctForMaxStitchLength(pattern, 12.7, 12.7);

    colorlistSize = embThreadList_count(pattern->threadList);
    minColors = max(colorlistSize, 6);
    binaryWriteInt(file, 0x74 + (minColors * 4));
    binaryWriteInt(file, 0x0A);

    embTime_initNow(&time);

    embFile_printf(file, "%04d%02d%02d%02d%02d%02d", (int)(time.year + 1900),
            (int)(time.month + 1), (int)(time.day), (int)(time.hour),
            (int)(time.minute), (int)(time.second));
    binaryWriteByte(file, 0x00);
    binaryWriteByte(file, 0x00);
    binaryWriteInt(file, embThreadList_count(pattern->threadList));
    binaryWriteInt(file, embStitchList_count(pattern->stitchList) + max(0, (6 - colorlistSize) * 2) + 1);

    boundingRect = embPattern_calcBoundingBox(pattern);

    designWidth = (int)(embRect_width(boundingRect) * 10.0);
    designHeight = (int)(embRect_width(boundingRect) * 10.0);

    binaryWriteInt(file, jefGetHoopSize(designWidth, designHeight));

    /* Distance from center of Hoop */
    binaryWriteInt(file, (int) (designWidth / 2));  /* left */
    binaryWriteInt(file, (int) (designHeight / 2)); /* top */
    binaryWriteInt(file, (int) (designWidth / 2));  /* right */
    binaryWriteInt(file, (int) (designHeight / 2)); /* bottom */

    /* Distance from default 110 x 110 Hoop */
    if(min(550 - designWidth / 2, 550 - designHeight / 2) >= 0)
    {
        binaryWriteInt(file, (int) max(-1, 550 - designWidth / 2));  /* left */
        binaryWriteInt(file, (int) max(-1, 550 - designHeight / 2)); /* top */
        binaryWriteInt(file, (int) max(-1, 550 - designWidth / 2));  /* right */
        binaryWriteInt(file, (int) max(-1, 550 - designHeight / 2)); /* bottom */
    }
    else
    {
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
    }

    /* Distance from default 50 x 50 Hoop */
    if(min(250 - designWidth / 2, 250 - designHeight / 2) >= 0)
    {
        binaryWriteInt(file, (int) max(-1, 250 - designWidth / 2));  /* left */
        binaryWriteInt(file, (int) max(-1, 250 - designHeight / 2)); /* top */
        binaryWriteInt(file, (int) max(-1, 250 - designWidth / 2));  /* right */
        binaryWriteInt(file, (int) max(-1, 250 - designHeight / 2)); /* bottom */
    }
    else
    {
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
    }

    /* Distance from default 140 x 200 Hoop */
    binaryWriteInt(file, (int) (700 - designWidth / 2));   /* left */
    binaryWriteInt(file, (int) (1000 - designHeight / 2)); /* top */
    binaryWriteInt(file, (int) (700 - designWidth / 2));   /* right */
    binaryWriteInt(file, (int) (1000 - designHeight / 2)); /* bottom */

    /* repeated Distance from default 140 x 200 Hoop */
    /* TODO: Actually should be distance to custom hoop */
    binaryWriteInt(file, (int) (630 - designWidth / 2));  /* left */
    binaryWriteInt(file, (int) (550 - designHeight / 2)); /* top */
    binaryWriteInt(file, (int) (630 - designWidth / 2));  /* right */
    binaryWriteInt(file, (int) (550 - designHeight / 2)); /* bottom */

    threadPointer = pattern->threadList;

    while(threadPointer)
    {
        binaryWriteInt(file, embThread_findNearestColorInArray(threadPointer->thread.color, (EmbThread*)jefThreads, 79));
        threadPointer = threadPointer->next;
    }
    for(i = 0; i < (minColors - colorlistSize); i++)
    {
        binaryWriteInt(file, 0x0D);
    }
      stitches = pattern->stitchList;
    while(stitches)
    {
        dx = stitches->stitch.xx * 10.0 - xx;
        dy = stitches->stitch.yy * 10.0 - yy;
        xx = stitches->stitch.xx * 10.0;
        yy = stitches->stitch.yy * 10.0;
        flags = stitches->stitch.flags;
        jefEncode(b, (char)roundDouble(dx), (char)roundDouble(dy), flags);
        if((b[0] == 0x80) && ((b[1] == 1) || (b[1] == 2) || (b[1] == 4) || (b[1] == 0x10)))
        {
            embFile_printf(file, "%c%c%c%c", b[0], b[1], b[2], b[3]);
        }
        else
        {
            embFile_printf(file, "%c%c", b[0], b[1]);
        }
        stitches = stitches->next;
    }
    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
