#include "format-t09.h"
#include "emb-file.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readT09(EmbPattern* pattern, const char* fileName)
{
    unsigned char b[3];
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-t09.c readT09(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-t09.c readT09(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-t09.c readT09(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 0x0C, SEEK_SET);

    while(embFile_read(b, 1, 3, file) == 3)
    {
        int stitchType = NORMAL;
        int b1 = b[0];
        int b2 = b[1];
        unsigned char commandByte = b[2];
        if(commandByte == 0x00)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        if(commandByte & 0x10) stitchType = STOP;
        if(commandByte & 0x20) b1 = -b1;
        if(commandByte & 0x40) b2 = -b2;

        embPattern_addStitchRel(pattern, b2 / 10.0, b1 / 10.0, stitchType, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeT09(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-t09.c writeT09(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-t09.c writeT09(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-t09.c writeT09(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeT09 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
