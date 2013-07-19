#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "format-xxx.h"
#include "helpers-binary.h"
#include "helpers-misc.h"

char xxxDecodeByte(unsigned char inputByte)
{
    if(inputByte >= 0x80)
        return (char) ((-~inputByte) - 1);
    return ((char) inputByte);
}

int readXxx(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    unsigned char b0, b1;
    int dx = 0, dy = 0;
    int flags;
    char endOfStream = 0;
    int numberOfColors;
    int paletteOffset;
    int i;
    char thisStitchJump = 0;
    EmbStitchList* lastStitch;
    EmbStitchList* secondLast = 0;

    file = fopen(fileName, "rb");
    if(file == 0)
    {
        /*TODO: set messages here "Error opening XXX file for read:" */
        return 0;
    }

    fseek(file, 0x27, SEEK_SET);
    numberOfColors = binaryReadInt16(file);
    fseek(file, 0xFC, SEEK_SET);
    paletteOffset = binaryReadInt32(file);
    fseek(file, paletteOffset + 6, SEEK_SET);

    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread thread;
        binaryReadByte(file);
        thread.color.r = binaryReadByte(file);
        thread.color.g = binaryReadByte(file);
        thread.color.b = binaryReadByte(file);
        embPattern_addThread(pattern, thread);
    }
    fseek(file, 0x100, SEEK_SET);

    for(i = 0; !endOfStream && ftell(file) < paletteOffset; i++)
    {
        flags = NORMAL;
        if(thisStitchJump) flags = TRIM;
        thisStitchJump = 0;
        b0 = binaryReadByte(file);
        b1 = binaryReadByte(file);

        if(b0 == 0x7E || b0 == 0x7D) /* TODO: ARE THERE OTHER BIG JUMP CODES? */
        {
            dx = b1 + (binaryReadByte(file) << 8);
            dx = ((short) dx);
            dy = binaryReadInt16(file);
            flags = TRIM;
        }
        else if(b0 == 0x7F)
        {
            if(b1 != 0x17 && b1 != 0x46 && b1 >= 8) /* TODO: LOOKS LIKE THESE CODES ARE IN THE HEADER */
            {
                b0 = 0;
                b1 = 0;
                thisStitchJump = 1;
                flags = STOP;
            }
            else if(b1 == 1)
            {
                flags = TRIM;
                b0 = binaryReadByte(file);
                b1 = binaryReadByte(file);
            }
            else
            {
                continue;
            }
            dx = xxxDecodeByte(b0);
            dy = xxxDecodeByte(b1);
        }
        else
        {
            dx = xxxDecodeByte(b0);
            dy = xxxDecodeByte(b1);
        }
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    lastStitch = pattern->stitchList;
    secondLast = 0;
    while(lastStitch->next)
    {
        secondLast = lastStitch;
        lastStitch=lastStitch->next;
    }
    if((!pattern->stitchList) && lastStitch->stitch.flags == STOP)
    {
        free(lastStitch);
        secondLast->next = NULL;
        embPattern_changeColor(pattern, pattern->currentColorIndex - 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);
    return 1;
}

void xxxEncodeStop(FILE* file, EmbStitch s)
{
    binaryWriteByte(file, 0x7F);
    binaryWriteByte(file, s.color + 8);
}

double round(double d) /*TODO: this should probably be renamed to avoid compiling issues or find out if roundDouble() in helpers.h that returns an int will work. */
{
    return floor(d + 0.5);
}

void xxxEncodeStitch(FILE* file, double deltaX, double deltaY, int flags)
{
    if((flags & (JUMP | TRIM)) && (fabs(deltaX) > 124 || fabs(deltaY) > 124))
    {
        binaryWriteByte(file, 0x7E);
        binaryWriteShort(file, (short)deltaX);
        binaryWriteShort(file, (short)deltaY);
    }
    else
    {
        binaryWriteByte(file, (char)round(deltaX));
        binaryWriteByte(file, (char)round(deltaY));
    }
}

void xxxEncodeDesign(FILE* file, EmbPattern* p)
{
    double thisX = 0.0f;
    double thisY = 0.0f;
    EmbStitchList* stitches;

    if(!embStitch_empty(p->stitchList))
    {
        thisX = (float)p->stitchList->stitch.xx;
        thisY = (float)p->stitchList->stitch.yy;
    }
    stitches = p->stitchList;
    while(stitches)
    {
        EmbStitch s = stitches->stitch;
        double deltaX, deltaY;
        double previousX = thisX;
        double previousY = thisY;
        thisX = s.xx;
        thisY = s.yy;
        deltaX = thisX - previousX;
        deltaY = thisY - previousY;
        if(s.flags & STOP)
        {
            xxxEncodeStop(file, s);
        }
        else if(s.flags & END)
        {
        }
        else
        {
            xxxEncodeStitch(file, deltaX * 10.0f, deltaY * 10.0f, s.flags);
        }
        stitches = stitches->next;
    }
}

int writeXxx(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    int i;
    EmbRect rect;
    int endOfStitches;
    EmbThreadList* colors;
    int curColor = 0;

    embPattern_correctForMaxStitchLength(pattern, 124, 127);
    file = fopen(fileName, "wb");
    if(file==0)
    {
        /*TODO: set status here "Error opening XXX file for write:" */
        return 0;
    }
    for(i = 0; i < 0x17; i++)
    {
        binaryWriteByte(file, 0x00);
    }
    binaryWriteUInt(file, (unsigned int) embStitch_count(pattern->stitchList));
    for(i = 0; i < 0x0C; i++)
    {
        binaryWriteByte(file, 0x00);
    }
    binaryWriteUShort(file, (unsigned short)embThread_count(pattern->threadList));
    binaryWriteShort(file, 0x0000);

    rect = embPattern_calcBoundingBox(pattern);
    binaryWriteShort(file, (short)(embRect_width(rect) * 10.0));
    binaryWriteShort(file, (short)(embRect_height(rect) * 10.0));

    binaryWriteShort(file, (short)(embRect_width(rect) / 2.0 * 10));  /*TODO: xEnd from start point x=0 */
    binaryWriteShort(file, (short)(embRect_height(rect) / 2.0 * 10)); /*TODO: yEnd from start point y=0 */
    binaryWriteShort(file, (short)(embRect_width(rect)/2.0 * 10));    /*TODO: left from start x = 0     */
    binaryWriteShort(file, (short)(embRect_height(rect)/2.0 * 10));   /*TODO: bottom from start y = 0   */
    for(i = 0; i < 0xC5; i++)
    {
        binaryWriteByte(file, 0x00);
    }
    binaryWriteInt(file, 0x0000); /* place holder for end of stitches */

    xxxEncodeDesign(file, pattern);

    endOfStitches = ftell(file);

    fseek(file, 0xFC, SEEK_SET);

    binaryWriteUInt(file, endOfStitches);

    fseek(file, 0, SEEK_END);

    binaryWriteByte(file, 0x7F); /* is this really correct? */
    binaryWriteByte(file, 0x7F);
    binaryWriteByte(file, 0x03);
    binaryWriteByte(file, 0x14);
    binaryWriteByte(file, 0x00);
    binaryWriteByte(file, 0x00);
    colors = pattern->threadList;
    while(colors)
    {
        binaryWriteByte(file, 0x00);
        binaryWriteByte(file, colors->thread.color.r);
        binaryWriteByte(file, colors->thread.color.g);
        binaryWriteByte(file, colors->thread.color.b);
        curColor++;
        colors = colors->next;
    }
    for(i = 0; i < (22 - curColor); i++)
    {
        binaryWriteUInt(file, 0x01000000);
    }
    binaryWriteByte(file, 0x00);
    binaryWriteByte(file, 0x01);
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
