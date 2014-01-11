#include "format-pem.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPem(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-pem.c readPem(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pem.c readPem(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish ReadPem */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePem(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-pem.c writePem(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pem.c writePem(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writePem */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
