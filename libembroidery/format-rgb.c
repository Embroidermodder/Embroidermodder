#include "format-rgb.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdio.h>
#include <stdlib.h>

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readRgb(EmbPattern* pattern, const char* fileName)
{
    int i, numberOfColors;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-rgb.c readRgb(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-rgb.c readRgb(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "rb");
    if(!file)
    {
        /* NOTE: The .rgb format is an optional color file. Do not log an error if the file does not exist */
        return 0;
    }
    fseek(file, 0x00, SEEK_END);
    numberOfColors = ftell(file) / 4;

    embThreadList_free(pattern->threadList);

    fseek(file, 0x00, SEEK_SET);
    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread t;
        t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        t.color.b = binaryReadByte(file);
        binaryReadByte(file);
        embPattern_addThread(pattern, t);
    }
    fclose(file);
    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeRgb(EmbPattern* pattern, const char* fileName)
{
    EmbThreadList* colors = pattern->threadList;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-rgb.c writeRgb(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-rgb.c writeRgb(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "wb");
    if(!file)
    {
        embLog_error("format-rgb.c writeRgb(), cannot open %s for writing\n", fileName);
        return 0;
    }

    while(colors)
    {
        EmbColor c = colors->thread.color;
        binaryWriteByte(file, c.r);
        binaryWriteByte(file, c.g);
        binaryWriteByte(file, c.b);
        binaryWriteByte(file, 0);
        colors = colors->next;
    }
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
