#include "format-100.h"
#include "emb-file.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int read100(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;
    int x, y;
    int stitchType;
    unsigned char b[4];

    if(!pattern) { embLog_error("format-100.c read100(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-100.c read100(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-100.c read100(), cannot open %s for reading\n", fileName);
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);
    while(embFile_read(b, 1, 4, file) == 4)
    {
        stitchType = NORMAL;
        x = (b[2] > 0x80) ? -(b[2] - 0x80) : b[2];
        y = (b[3] > 0x80) ? -(b[3] - 0x80) : b[3];
        /*if(!(b[0] & 0xFC)) stitchType = JUMP; TODO: review & fix */
        if(!(b[0] & 0x01)) stitchType = STOP;
        if(b[0] == 0x1F) stitchType = END;
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, stitchType, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int write100(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-100.c write100(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-100.c write100(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-100.c write100(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish write100 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
