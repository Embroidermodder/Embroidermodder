/*
 * 10o FORMAT
 * This file is part of libembroidery.
 *
 * Copyright 2018-2023 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 */

#include <stdio.h>
#include <math.h>

#include "../embroidery_internal.h"

int
read10o(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-10o.c read10o(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-10o.c read10o(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName,"rb");
    if(!file)
    {
        embLog_error("format-10o.c read10o(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);

    while(1)
    {
        int x, y;
        int stitchType = NORMAL;
        unsigned char ctrl = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
            break;
        y = embFile_getc(file);
        if(embFile_eof(file))
            break;
        x = embFile_getc(file);
        if(embFile_eof(file))
            break;
        if(ctrl & 0x20)
            x = -x;
        if(ctrl & 0x40)
            y = -y;
        if(ctrl & 0x01)
            stitchType = TRIM;
        if((ctrl & 0x5) == 5)
        {
            stitchType = STOP;
        }
        if(ctrl == 0xF8 || ctrl == 0x91 || ctrl == 0x87)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, stitchType, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch && pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

int
write10o(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-10o.c write10o(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-10o.c write10o(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-10o.c write10o(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish write10o */
}
