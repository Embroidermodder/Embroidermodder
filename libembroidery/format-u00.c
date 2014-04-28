#include "format-u00.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readU00(EmbPattern* pattern, const char* fileName)
{
    int i;
    char dx = 0, dy = 0;
    int flags = NORMAL;
    char endOfStream = 0;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-u00.c readU00(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-u00.c readU00(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-u00.c readU00(), cannot open %s for reading\n", fileName);
        return 0;
    }

    /* 16 3byte RGB's start @ 0x08 followed by 14 bytes between 0 and 15 with index of color for each color change */
    embFile_seek(file, 0x08, SEEK_SET);

    for(i = 0; i < 16; i++)
    {
        EmbThread t;
        t.color.r = binaryReadUInt8(file);
        t.color.g = binaryReadUInt8(file);
        t.color.b = binaryReadUInt8(file);
        embPattern_addThread(pattern, t);
    }

    embFile_seek(file, 0x100, SEEK_SET);
    for(i = 0; !endOfStream; i++)
    {
        char negativeX , negativeY;
        unsigned char b0 = binaryReadUInt8(file);
        unsigned char b1 = binaryReadUInt8(file);
        unsigned char b2 = binaryReadUInt8(file);

        if(b0 == 0xF8 || b0 == 0x87 || b0 == 0x91)
        {
            break;
        }
        if((b0 & 0x0F) == 0)
        {
            flags = NORMAL;
        }
        else if((b0 & 0x1f) == 1)
        {
            flags = JUMP;
        }
        else if((b0 & 0x0F) > 0)
        {
            flags = STOP;
        }
        negativeX = ((b0 & 0x20) > 0);
        negativeY = ((b0 & 0x40) > 0);

        dx = (char) b2;
        dy = (char) b1;
        if(negativeX) dx = (char) -dx;
        if(negativeY) dy = (char) -dy;
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeU00(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-u00.c writeU00(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-u00.c writeU00(), fileName argument is null\n"); return 0; }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish WriteU00 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
