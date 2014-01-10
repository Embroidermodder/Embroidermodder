#include "format-cnd.h"
#include "emb-logging.h"

int readCnd(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-cnd.c readCnd(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-cnd.c readCnd(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish readCnd */
}

int writeCnd(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-cnd.c writeCnd(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-cnd.c writeCnd(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeCnd */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
