#include "format-gt.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <stdio.h>

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readGt(EmbPattern* pattern, const char* fileName)
{
    FILE* file = 0;

    if(!pattern) { embLog_error("format-gt.c readGt(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-gt.c readGt(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "rb");
    if(!file)
    {
        embLog_error("format-gt.c readGt(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);
    fseek(file, 0x200, SEEK_SET);

    while(1)
    {
        int stitchType = NORMAL;
        int b1 = (int) binaryReadByte(file);
        int b2 = (int) binaryReadByte(file);
        unsigned char commandByte = binaryReadByte(file);

        if(commandByte == 0x91)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }

        if((commandByte & 0x01) == 0x01)
            stitchType = TRIM;
        if((commandByte & 0x02) == 0x02)
            stitchType = STOP;
        if((commandByte & 0x20) == 0x20)
            b1 = -b1;
        if((commandByte & 0x40) == 0x40)
            b2 = -b2;
        embPattern_addStitchRel(pattern, b2 / 10.0, b1 / 10.0, stitchType, 1);
    }
    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeGt(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-gt.c writeGt(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-gt.c writeGt(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeGt */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
