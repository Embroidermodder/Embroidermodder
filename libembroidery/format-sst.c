#include "format-sst.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdio.h>

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readSst(EmbPattern* pattern, const char* fileName)
{
    int fileLength;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-sst.c readSst(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-sst.c readSst(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "rb");
    if(!file)
    {
        embLog_error("format-sst.c readSst(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);
    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0xA0, SEEK_SET); /* skip the all zero header */
    while(ftell(file) < fileLength)
    {
        int stitchType = NORMAL;

        int b1 = (int) binaryReadByte(file);
        int b2 = (int) binaryReadByte(file);
        unsigned char commandByte = binaryReadByte(file);

        if(commandByte == 0x04)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }

        if((commandByte & 0x01) == 0x01)
            stitchType = STOP;
        if((commandByte & 0x02) == 0x02)
            stitchType = JUMP;
        if((commandByte & 0x10) != 0x10)
            b2 = -b2;
        if((commandByte & 0x40) == 0x40)
            b1 = -b1;
        embPattern_addStitchRel(pattern, b1 / 10.0, b2 / 10.0, stitchType, 1);
    }
    if(pattern->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
    }
    return 1; /*TODO: finish readSst */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeSst(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-sst.c writeSst(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-sst.c writeSst(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeSst */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
