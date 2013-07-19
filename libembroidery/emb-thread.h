#ifndef EMB_THREAD_H
#define EMB_THREAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "emb-color.h"

typedef struct EmbThread_
{
    EmbColor color;
    const char* description;
    const char* catalogNumber;
} EmbThread;

typedef struct EmbThreadList_
{
    EmbThread thread;
    struct EmbThreadList_* next;
} EmbThreadList;

int embThread_findNearestColor(EmbColor color, EmbThreadList* colors);
int embThread_findNearestColorInArray(EmbColor color, EmbThread* colorArray, int count);
void embThread_add(EmbThreadList* pointer, EmbThread data);
EmbThread embThread_getRandom();
EmbThread embThread_getAt(EmbThreadList* pointer, int num);
int embThread_count(EmbThreadList* pointer);
int embThread_empty(EmbThreadList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_THREAD_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
