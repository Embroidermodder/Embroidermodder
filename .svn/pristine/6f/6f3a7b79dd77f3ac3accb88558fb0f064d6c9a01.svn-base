#ifndef EMB_TIME_H
#define EMB_TIME_H

/* Disable warnings about unsafe use of fopen, fseek etc */
#pragma warning(disable: 4996)

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

void embTime_initNow(EmbTime *t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_TIME_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
