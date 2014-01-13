/*! @file emb-flag.h */
#ifndef EMB_FLAG_H
#define EMB_FLAG_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int EmbFlag;

typedef struct EmbFlagList_
{
    int flag;
    struct EmbFlagList_* next;
} EmbFlagList;

EmbFlagList* embFlagList_create(EmbFlag data);
EmbFlagList* embFlagList_add(EmbFlagList* pointer, EmbFlag data);
int embFlagList_count(EmbFlagList* pointer);
int embFlagList_empty(EmbFlagList* pointer);
void embFlagList_free(EmbFlagList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_FLAG_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
