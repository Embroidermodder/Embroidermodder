#include "format-pel.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPel(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-pel.c readPel(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pel.c readPel(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readPel */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePel(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-pel.c writePel(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pel.c writePel(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-pel.c writePel(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writePel */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
