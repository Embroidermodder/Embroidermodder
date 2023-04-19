/*
 * VP3 FORMAT
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
readEdr(EmbPattern* pattern, const char* fileName)
{
    int numberOfColors;
    int i;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-edr.c readEdr(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-edr.c readEdr(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        /* NOTE: The .edr format is an optional color file. Do not log an error if the file does not exist */
        return 0;
    }
    embFile_seek(file, 0x00, SEEK_END);
    numberOfColors = embFile_tell(file) / 4;
    embFile_seek(file, 0x00, SEEK_SET);

    embThreadList_free(pattern->threadList);
    pattern->threadList = 0;
    pattern->lastThread = 0;

    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread t;
        t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        t.color.b = binaryReadByte(file);
        t.catalogNumber = "";
        t.description = "";
        binaryReadByte(file);
        embPattern_addThread(pattern, t);
    }
    embFile_close(file);
    return 1;
}

int
writeEdr(EmbPattern* pattern, const char* fileName)
{
    EmbThreadList* pointer = 0;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-edr.c writeEdr(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-edr.c writeEdr(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-edr.c writeEdr(), cannot open %s for writing\n", fileName);
        return 0;
    }
    pointer = pattern->threadList;
    while(pointer)
    {
        EmbColor c;
        c = pointer->thread.color;
        binaryWriteByte(file, c.r);
        binaryWriteByte(file, c.g);
        binaryWriteByte(file, c.b);
        binaryWriteByte(file, 0);
        pointer = pointer->next;
    }
    embFile_close(file);
    return 1;
}
