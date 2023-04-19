/*
 * TXT FORMAT
 * This file is part of libembroidery.
 *
 * Copyright 2018-2023 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../embroidery_internal.h"

int
readTxt(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-txt.c readTxt(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-txt.c readTxt(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readTxt */
}

int
writeTxt(EmbPattern* pattern, const char* fileName)
{
    EmbStitchList* pointer = 0;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-txt.c writeTxt(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-txt.c writeTxt(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-txt.c writeTxt(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch && pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    file = embFile_open(fileName, "w");
    if(!file)
    {
        embLog_error("format-txt.c writeTxt(), cannot open %s for writing\n", fileName);
        return 0;
    }
    pointer = pattern->stitchList;
    embFile_printf(file, "%u\n", (unsigned int) embStitchList_count(pointer));

    while(pointer)
    {
        EmbStitch s = pointer->stitch;
        embFile_printf(file, "%.1f,%.1f color:%i flags:%i\n", s.xx, s.yy, s.color, s.flags);
        pointer = pointer->next;
    }

    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
