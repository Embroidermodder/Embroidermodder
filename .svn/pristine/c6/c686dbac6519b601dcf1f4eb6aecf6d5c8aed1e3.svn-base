#include "emb-time.h"
#include <time.h>

void embTime_initNow(EmbTime *t)
{
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    t->year   = timeinfo->tm_year;
    t->month  = timeinfo->tm_mon;
    t->day    = timeinfo->tm_mday;
    t->hour   = timeinfo->tm_hour;
    t->minute = timeinfo->tm_min;
    t->second = timeinfo->tm_sec;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
