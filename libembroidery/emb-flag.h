/*! @file emb-flag.h */
#ifndef EMB_FLAG_H
#define EMB_FLAG_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef int EmbFlag;

typedef struct EmbFlagList_
{
    int flag;
    struct EmbFlagList_* next;
} EmbFlagList;

extern EMB_PUBLIC EmbFlagList* EMB_CALL embFlagList_create(EmbFlag data);
extern EMB_PUBLIC EmbFlagList* EMB_CALL embFlagList_add(EmbFlagList* pointer, EmbFlag data);
extern EMB_PUBLIC int EMB_CALL embFlagList_count(EmbFlagList* pointer);
extern EMB_PUBLIC int EMB_CALL embFlagList_empty(EmbFlagList* pointer);
extern EMB_PUBLIC void EMB_CALL embFlagList_free(EmbFlagList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_FLAG_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
