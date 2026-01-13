#include "format-inf.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdlib.h>
#include <string.h>

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readInf(EmbPattern* pattern, const char* fileName)
{
    int numberOfColors;
    int i;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-inf.c readInf(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-inf.c readInf(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        /* NOTE: The .inf format is an optional color file. Do not log an error if the file does not exist */
        return 0;
    }

    binaryReadUInt32BE(file);
    binaryReadUInt32BE(file);
    binaryReadUInt32BE(file);
    numberOfColors = binaryReadUInt32BE(file);

    embThreadList_free(pattern->threadList);
    pattern->threadList = 0;
    pattern->lastThread = 0;

    for(i = 0; i < numberOfColors; i++)
    {
        char colorType[50];
        char colorDescription[50];
        EmbThread t;
        binaryReadUInt16(file);
        binaryReadUInt16(file);
        t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        t.color.b = binaryReadByte(file);
        t.catalogNumber = "";
        t.description = "";
        embPattern_addThread(pattern, t);
        binaryReadUInt16(file);
        binaryReadString(file, colorType, 50);
        binaryReadString(file, colorDescription, 50);
    }
    embFile_close(file);
    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeInf(EmbPattern* pattern, const char* fileName)
{
    EmbThreadList* pointer = 0;
    int i = 1, bytesRemaining;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-inf.c writeInf(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-inf.c writeInf(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-inf.c writeInf(), cannot open %s for writing\n", fileName);
        return 0;
    }
    binaryWriteUIntBE(file, 0x01);
    binaryWriteUIntBE(file, 0x08);
    /* write place holder offset */
    binaryWriteUIntBE(file, 0x00);
    binaryWriteUIntBE(file, embThreadList_count(pattern->threadList));

    pointer = pattern->threadList;
    while(pointer)
    {
        char buffer[50];
        EmbColor c;
        c = pointer->thread.color;
        sprintf(buffer, "RGB(%d,%d,%d)", (int)c.r, (int)c.g, (int)c.b);
        binaryWriteUShortBE(file, (unsigned short)(14 + strlen(buffer))); /* record length */
        binaryWriteUShortBE(file, (unsigned short)i); /* record number */
        binaryWriteByte(file, c.r);
        binaryWriteByte(file, c.g);
        binaryWriteByte(file, c.b);
        binaryWriteUShortBE(file, (unsigned short)i); /* needle number */
        binaryWriteBytes(file, "RGB\0", 4);
        embFile_printf(file, buffer);
        binaryWriteByte(file, 0);
        pointer = pointer->next;
        i++;
    }
    embFile_seek(file, -8, SEEK_END);
    bytesRemaining = embFile_tell(file);
    embFile_seek(file, 8, SEEK_SET);
    binaryWriteUIntBE(file, bytesRemaining);
    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
