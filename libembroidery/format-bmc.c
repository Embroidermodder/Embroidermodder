#include "format-bmc.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readBmc(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-bmc.c readBmc(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-bmc.c readBmc(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readBmc */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeBmc(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-bmc.c writeBmc(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-bmc.c writeBmc(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-bmc.c writeBmc(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeBmc */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
