#include "format-dem.h"
#include "emb-logging.h"

int readDem(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-dem.c readDem(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dem.c readDem(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readDem */
}

int writeDem(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-dem.c writeDem(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dem.c writeDem(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeDem */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
