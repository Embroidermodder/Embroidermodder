#include "format-u01.h"
#include "emb-file.h"
#include "emb-logging.h"

/* TODO: AFAIK this is a duplicate of U00. Review for differences and merge files and handle accordingly. */

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readU01(EmbPattern* pattern, const char* fileName)
{
    int fileLength, negativeX = 0, negativeY = 0, flags = NORMAL;
    char dx, dy;
    unsigned char data[3];
    EmbFile* file = 0;

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        return 0;
    }
    embFile_seek(file, 0, SEEK_END);
    fileLength = embFile_tell(file);
    embFile_seek(file, 0x100, SEEK_SET);
    while(embFile_read(data, 1, 3, file) == 3)
    {
        if(data[0] == 0xF8 || data[0] == 0x87 || data[0] == 0x91)
        {
            break;
        }
        if((data[0] & 0x0F) == 0)
        {
            flags = NORMAL;
        }
        else if((data[0] & 0x1f) == 1)
        {
            flags = JUMP;
        }
        else if((data[0] & 0x0F) > 0)
        {
            flags = STOP;
        }
        negativeX = ((data[0] & 0x20) > 0);
        negativeY = ((data[0] & 0x40) > 0);

        dx = (char) data[2];
        dy = (char) data[1];
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
int writeU01(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-u01.c writeU01(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-u01.c writeU01(), fileName argument is null\n"); return 0; }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeU01 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
