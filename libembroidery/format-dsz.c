#include "format-dsz.h"
#include "emb-file.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readDsz(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-dsz.c readDsz(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dsz.c readDsz(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName,"rb");
    if(!file)
    {
        embLog_error("format-dsz.c readDsz(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);

    embFile_seek(file, 0x200, SEEK_SET);
    while(1)
    {
        int x, y;
        unsigned char ctrl;
        int stitchType = NORMAL;

        y = embFile_getc(file);
        if(embFile_eof(file)) break;
        x = embFile_getc(file);
        if(embFile_eof(file)) break;
        ctrl = (unsigned char)embFile_getc(file);
        if(embFile_eof(file)) break;
        if(ctrl & 0x01) stitchType = TRIM;
        if(ctrl & 0x20) y = -y;
        if(ctrl & 0x40) x = -x;

        if(ctrl & 0x0E)
        {
            int headNumber = (ctrl & 0x0E) >> 1;
            stitchType = STOP;
        }
        if(ctrl & 0x10)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        embPattern_addStitchRel(pattern, x  / 10.0, y  / 10.0, stitchType, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeDsz(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-dsz.c writeDsz(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dsz.c writeDsz(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-dsz.c writeDsz(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeDsz */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
