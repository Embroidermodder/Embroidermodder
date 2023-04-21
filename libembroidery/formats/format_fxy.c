#include "embroidery.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readFxy(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-fxy.c readFxy(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-fxy.c readFxy(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-fxy.c readFxy(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);
    embFile_seek(file, 0x100, SEEK_SET); /* TODO: review for combining code. This line appears to be the only difference from the GT format. */

    while(!embFile_eof(file))
    {
        int stitchType = NORMAL;
        int b1 = (int)binaryReadByte(file);
        int b2 = (int)binaryReadByte(file);
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
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch && pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeFxy(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-fxy.c writeFxy(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-fxy.c writeFxy(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-fxy.c writeFxy(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch && pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeFxy */
}
