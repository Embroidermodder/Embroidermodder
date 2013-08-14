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
EmbThread embThread_getRandom(void);

void embThreadList_add(EmbThreadList* pointer, EmbThread data);
int embThreadList_count(EmbThreadList* pointer);
int embThreadList_empty(EmbThreadList* pointer);
void embThreadList_free(EmbThreadList* pointer);
EmbThread embThreadList_getAt(EmbThreadList* pointer, int num);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_THREAD_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
