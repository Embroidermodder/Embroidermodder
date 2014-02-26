#include "emb-flag.h"
#include "emb-logging.h"
#include <stdlib.h>

/**************************************************/
/* EmbFlagList                                    */
/**************************************************/

EmbFlagList* embFlagList_create(EmbFlag data)
{
    EmbFlagList* heapFlagList = (EmbFlagList*)malloc(sizeof(EmbFlagList));
    if(!heapFlagList) { embLog_error("emb-flag.c embFlagList_create(), cannot allocate memory for heapFlagList\n"); return 0; }
    heapFlagList->flag = data;
    heapFlagList->next = 0;
    return heapFlagList;
}

EmbFlagList* embFlagList_add(EmbFlagList* pointer, EmbFlag data)
{
    if(!pointer) { embLog_error("emb-flag.c embFlagList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-flag.c embFlagList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbFlagList*)malloc(sizeof(EmbFlagList));
    if(!pointer->next) { embLog_error("emb-flag.c embFlagList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->flag = data;
    pointer->next = 0;
    return pointer;
}

int embFlagList_count(EmbFlagList* pointer)
{
    int i = 1;
    if(!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embFlagList_empty(EmbFlagList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embFlagList_free(EmbFlagList* pointer)
{
    EmbFlagList* tempPointer = pointer;
    EmbFlagList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
