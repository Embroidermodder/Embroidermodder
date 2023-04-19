/*
 * DEM FORMAT
 * This file is part of libembroidery.
 *
 * Copyright 2018-2023 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 */

#include <stdio.h>
#include <math.h>

#include "../embroidery_internal.h"

int
readDem(EmbPattern* pattern, const char* fileName)
{
    puts("readDem is not implemented.");
    puts("Overridden, defaulting to dst.");
    if(!pattern) { embLog_error("format-dem.c readDem(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dem.c readDem(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readDem */
}

int
writeDem(EmbPattern* pattern, const char* fileName)
{
    puts("writeDem is not implemented.");
    puts("Overridden, defaulting to dst.");
    if(!pattern) { embLog_error("format-dem.c writeDem(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dem.c writeDem(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-dem.c writeDem(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch && pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeDem */
}
