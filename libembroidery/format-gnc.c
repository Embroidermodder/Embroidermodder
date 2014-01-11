#include "format-gnc.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readGnc(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-gnc.c readGnc(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-gnc.c readGnc(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readGnc */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeGnc(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-gnc.c writeGnc(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-gnc.c writeGnc(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeGnc */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
