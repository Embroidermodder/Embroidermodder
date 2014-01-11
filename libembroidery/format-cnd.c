#include "format-cnd.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readCnd(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-cnd.c readCnd(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-cnd.c readCnd(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readCnd */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeCnd(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-cnd.c writeCnd(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-cnd.c writeCnd(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeCnd */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
