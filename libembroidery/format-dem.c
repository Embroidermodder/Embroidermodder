#include "format-dem.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readDem(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-dem.c readDem(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dem.c readDem(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readDem */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeDem(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-dem.c writeDem(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dem.c writeDem(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeDem */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
