/*
 * COL FORMAT
 * This file is part of libembroidery.
 *
 * Copyright 2018-2023 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../embroidery_internal.h"

int
readCol(EmbPattern* pattern, const char* fileName)
{
    int numberOfColors, i;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-col.c readCol(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-col.c readCol(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "r");
    if(!file)
    {
        /* NOTE: The .col format is an optional color file. Do not log an error if the file does not exist */
        return 0;
    }

    embThreadList_free(pattern->threadList);
    pattern->threadList = 0;
    pattern->lastThread = 0;

    /* TODO: replace all scanf code */
    if(fscanf(file, "%d\r", &numberOfColors) < 1) /* TODO: needs to work cross-platform - Win: \r\n Mac: \r Linux: \n */
    {
        /* TODO: log error */
        return 0;
    }
    for(i = 0; i < numberOfColors; i++)
    {
        int num, blue, green, red;
        EmbThread t;
        char line[30];
        /* TODO: replace all scanf code */
        if(fscanf(file, "%s\r", line) < 1) /* TODO: needs to work cross-platform - Win: \r\n Mac: \r Linux: \n */
        {
            /* TODO: log error */
            return 0;
        }
        /* TODO: replace all scanf code */
        if(sscanf(line,"%d,%d,%d,%d\n\r", &num, &blue, &green, &red) != 4) /* TODO: needs to work cross-platform - Win: \r\n Mac: \r Linux: \n */
        {
            break;
        }
        t.color.r = (unsigned char)red;
        t.color.g = (unsigned char)green;
        t.color.b = (unsigned char)blue;
        t.catalogNumber = "";
        t.description = "";
        embPattern_addThread(pattern, t);
    }
    fclose(file);
    return 1;
}

int
writeCol(EmbPattern* pattern, const char* fileName)
{
    FILE* file = 0;
    int i, colorCount;
    EmbThreadList *colors;

    if(!pattern) { embLog_error("format-col.c writeCol(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-col.c writeCol(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "w");
    if(!file)
    {
        embLog_error("format-col.c writeCol(), cannot open %s for writing\n", fileName);
        return 0;
    }
    colorCount = embThreadList_count(pattern->threadList);
    fprintf(file, "%d\n\r", colorCount); /* TODO: needs to be \r\n */
    colors = pattern->threadList;
    i = 0;
    while(colors)
    {
        EmbColor c;
        c = colors->thread.color;
        fprintf(file, "%d,%d,%d,%d\n\r", i, (int)c.r, (int)c.g, (int)c.b); /* TODO: needs to be \r\n */
        i++;
        colors = colors->next;
    }
    fclose(file);
    return 1;
}
