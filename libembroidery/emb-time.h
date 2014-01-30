/*! @file emb-time.h */
#ifndef EMB_TIME_H
#define EMB_TIME_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbTime_
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
} EmbTime;

extern EMB_PUBLIC void EMB_CALL embTime_initNow(EmbTime* t);
extern EMB_PUBLIC EmbTime EMB_CALL embTime_time(EmbTime* t);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_TIME_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
