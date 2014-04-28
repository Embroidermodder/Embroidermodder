#include "format-exy.h"
#include "emb-file.h"
#include "emb-logging.h"

static int exyDecodeFlags(unsigned char b2)
{
    int returnCode = 0;
    if(b2 == 0xF3)
        return (END);
    if((b2 & 0xC3) == 0xC3)
        return TRIM | STOP;
    if(b2 & 0x80)
        returnCode |= TRIM;
    if(b2 & 0x40)
        returnCode |= STOP;
    return returnCode;
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readExy(EmbPattern* pattern, const char* fileName)
{
    unsigned char b[3];
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-exy.c readExy(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-exy.c readExy(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-exy.c readExy(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);

    embFile_seek(file, 0x100, SEEK_SET);

    while(embFile_read(b, 1, 3, file) == 3)
    {
        int flags;
        int x = 0;
        int y = 0;
        if(b[0] & 0x01)
            x += 1;
        if(b[0] & 0x02)
            x -= 1;
        if(b[0] & 0x04)
            x += 9;
        if(b[0] & 0x08)
            x -= 9;
        if(b[0] & 0x80)
            y += 1;
        if(b[0] & 0x40)
            y -= 1;
        if(b[0] & 0x20)
            y += 9;
        if(b[0] & 0x10)
            y -= 9;
        if(b[1] & 0x01)
            x += 3;
        if(b[1] & 0x02)
            x -= 3;
        if(b[1] & 0x04)
            x += 27;
        if(b[1] & 0x08)
            x -= 27;
        if(b[1] & 0x80)
            y += 3;
        if(b[1] & 0x40)
            y -= 3;
        if(b[1] & 0x20)
            y += 27;
        if(b[1] & 0x10)
            y -= 27;
        if(b[2] & 0x04)
            x += 81;
        if(b[2] & 0x08)
            x -= 81;
        if(b[2] & 0x20)
            y += 81;
        if(b[2] & 0x10)
            y -= 81;
        flags = exyDecodeFlags(b[2]);
        if((flags & END) == END)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeExy(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-exy.c writeExy(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-exy.c writeExy(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-exy.c writeExy(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeExy */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
