/*! @file emb-thread.h */
#ifndef EMB_THREAD_H
#define EMB_THREAD_H

/* TODO: what the heck is math.h doing here? This needs moved to the source file instead of being here. */
#include <math.h>
#include "emb-color.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

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

extern EMB_PUBLIC int EMB_CALL embThread_findNearestColor(EmbColor color, EmbThreadList* colors);
extern EMB_PUBLIC int EMB_CALL embThread_findNearestColorInArray(EmbColor color, EmbThread* colorArray, int count);
extern EMB_PUBLIC EmbThread EMB_CALL embThread_getRandom(void);

extern EMB_PUBLIC EmbThreadList* EMB_CALL embThreadList_create(EmbThread data);
extern EMB_PUBLIC EmbThreadList* EMB_CALL embThreadList_add(EmbThreadList* pointer, EmbThread data);
extern EMB_PUBLIC int EMB_CALL embThreadList_count(EmbThreadList* pointer);
extern EMB_PUBLIC int EMB_CALL embThreadList_empty(EmbThreadList* pointer);
extern EMB_PUBLIC void EMB_CALL embThreadList_free(EmbThreadList* pointer);
extern EMB_PUBLIC EmbThread EMB_CALL embThreadList_getAt(EmbThreadList* pointer, int num);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_THREAD_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
