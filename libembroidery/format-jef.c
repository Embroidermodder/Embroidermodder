#include "emb-time.h"
#include "format-jef.h"
#include "helpers-binary.h"
#include "helpers-misc.h"

char jefDecode(unsigned char inputByte)
{
    return (inputByte >= 0x80) ? (char) ((-~inputByte) - 1) : (char) inputByte;
}

int readJef(EmbPattern* pattern, const char* fileName)
{
    int stitchOffset, byte0xA, numberOfColors, numberOfStitchs;
    int hoopSize,hoopOffsetLeft,hoopOffsetTop,hoopOffsetRight,hoopOffsetBottom;
    int patternLeft,patternTop,patternRight,patternBottom;
    int dunnoLeft,dunnoTop,dunnoRight,dunnoBottom;
    int patternRelativeLeft,patternRelativeTop,patternRelativeRight,patternRelativeBottom;
    int patternRelative2Left,patternRelative2Top,patternRelative2Right,patternRelative2Bottom,i;
    int stitchCount;
    char date[8], time[8];
    char dx, dy;

    FILE* file;
    file = fopen(fileName, "rb");
    if(file == 0)
    {
        /*TODO: set status here "Error opening JEF file for read:" */
        return 0;
    }

    stitchOffset = binaryReadInt32(file);
    byte0xA = binaryReadInt32(file);


    binaryReadBytes(file, (unsigned char*) date, 8);
    binaryReadBytes(file, (unsigned char*) time, 8);
    numberOfColors = binaryReadInt32(file);
    numberOfStitchs = binaryReadInt32(file);
    hoopSize = binaryReadInt32(file);
    hoopOffsetLeft = binaryReadInt32(file);
    hoopOffsetTop = binaryReadInt32(file);
    hoopOffsetRight = binaryReadInt32(file);
    hoopOffsetBottom = binaryReadInt32(file);

    patternLeft = binaryReadInt32(file);
    patternTop = binaryReadInt32(file);
    patternRight = binaryReadInt32(file);
    patternBottom = binaryReadInt32(file);

    dunnoLeft = binaryReadInt32(file);
    dunnoTop = binaryReadInt32(file);
    dunnoRight = binaryReadInt32(file);
    dunnoBottom = binaryReadInt32(file);

    patternRelativeLeft = binaryReadInt32(file);
    patternRelativeTop = binaryReadInt32(file);
    patternRelativeRight = binaryReadInt32(file);
    patternRelativeBottom = binaryReadInt32(file);

    patternRelative2Left = binaryReadInt32(file);
    patternRelative2Top = binaryReadInt32(file);
    patternRelative2Right = binaryReadInt32(file);
    patternRelative2Bottom = binaryReadInt32(file);
    for(i = 0; i < numberOfColors; i++)
    {
        embPattern_addThread(pattern, jefThreads[binaryReadInt32(file) % 78]);
    }

    for(i = 0; i < (6 - numberOfColors); i++)
    {
        binaryReadInt32(file);
    }

    stitchCount = 0;
    while(stitchCount < numberOfStitchs + 100)
    {
        int flags = NORMAL;
        unsigned char b0 = binaryReadByte(file);
        unsigned char b1 = binaryReadByte(file);

        if(b0 == 0x80)
        {
            if(b1 & 0x01)
            {
                b0 = binaryReadByte(file);
                b1 = binaryReadByte(file);
                flags = STOP;
            }
            else if(b1 == 0x02 || b1 == 0x04)
            {
                b0 = binaryReadByte(file);
                b1 = binaryReadByte(file);
                flags = TRIM;
            }
            else if(b1 == 0x10)
            {
                embPattern_addStitchRel(pattern, 0, 0, END, 1);
                break;
            }
        }
        dx = jefDecode(b0);
        dy = jefDecode(b1);
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
        stitchCount++;
    }
    fclose(file);
    return 1;
}

unsigned char jefEncodeByte(float inputByte)
{
    if(inputByte < 0)
    {
        return (unsigned char) ((~((int) inputByte)) - 1);
    }
    return ((unsigned char) inputByte);
}

void jefEncode(FILE* file, float x, float y, int stitchType)
{
    unsigned char dx = jefEncodeByte(x * 10.0f);
    unsigned char dy = jefEncodeByte(y * 10.0f);
    if(stitchType == TRIM)
    {
        binaryWriteByte(file, 128);
        binaryWriteByte(file, 2);
        binaryWriteByte(file, dx );
        binaryWriteByte(file, dy);
    }
    else if(stitchType == STOP)
    {
        binaryWriteByte(file, 128);
        binaryWriteByte(file, 1);
        binaryWriteByte(file, dx);
        binaryWriteByte(file, dy);
    }
    else if (stitchType == END)
    {
        binaryWriteByte(file, 128);
        binaryWriteByte(file, 0x10);
    }
    else
    {
        binaryWriteByte(file, dx);
        binaryWriteByte(file, dy);
    }
}

#define	HOOP_110X110 1
#define	HOOP_50X50 2
#define	HOOP_140X200 3

int jefGetHoopSize(int width, int height)
{
    if(width <  50 && height <  50) { return HOOP_50X50; }
    if(width < 110 && height < 110) { return HOOP_110X110; }
    if(width < 140 && height < 200) { return HOOP_140X200; }
    return ((int) HOOP_110X110);
}

int writeJef(EmbPattern* pattern, const char* fileName)
{
    int colorlistSize, minColors, designWidth, designHeight, i;
    EmbRect boundingRect;
    FILE* file;
    EmbTime time;
    EmbThreadList* threadPointer;
    EmbStitch c;
    EmbStitchList* pointer;
    double prevX, prevY;

    file = fopen(fileName, "wb");
    if(file == 0)
    {
        return 0;
    }
    embPattern_correctForMaxStitchLength(pattern, 127, 127);

    colorlistSize = embThread_count(pattern->threadList);
    minColors = max(colorlistSize, 6);
    binaryWriteInt(file, 0x74 + (minColors * 4));
    binaryWriteInt(file, 0x0A);

    embTime_initNow(&time);

    fprintf(file, "%04d%02d%02d%02d%02d%02d", time.year + 1900,
            time.month + 1, time.day, time.hour,
            time.minute, time.second);
    binaryWriteByte(file, 0x00);
    binaryWriteByte(file, 0x00);
    binaryWriteInt(file, embThread_count(pattern->threadList));
    binaryWriteInt(file, embStitch_count(pattern->stitchList) + max(0, (6 - colorlistSize) * 2) + 1);

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

    /* Distance from default 110 x 110 Hoop */
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
    pointer = pattern->stitchList;

    prevX = pointer->stitch.xx;
    prevY = pointer->stitch.yy;
    jefEncode(file, (unsigned char) prevX, (unsigned char) prevY, pointer->stitch.flags);
    pointer = pointer->next;
    while(pointer)
    {
        c = pointer->stitch;
        jefEncode(file, (unsigned char) (c.xx - prevX), (unsigned char) (c.yy - prevY), c.flags);
        prevX = c.xx;
        prevY = c.yy;
        pointer = pointer->next;
    }
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
