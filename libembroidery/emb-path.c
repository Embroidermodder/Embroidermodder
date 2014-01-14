#include "emb-path.h"
#include "emb-logging.h"
#include <stdlib.h>

/**************************************************/
/* EmbPathObject                                  */
/**************************************************/

EmbPathObject* embPathObject_create(EmbPointList* points, EmbFlagList* flags, EmbColor color, int lineType)
{
    EmbPathObject* heapPathObj = (EmbPathObject*)malloc(sizeof(EmbPathObject));
    if(!heapPathObj) { embLog_error("emb-path.c embPathObject_create(), cannot allocate memory for heapPathObj\n"); return 0; }
    heapPathObj->pointList = points;
    heapPathObj->flagList = flags;
    /* TODO: layer */
    heapPathObj->color = color;
    heapPathObj->lineType = lineType;
    return heapPathObj;
}

void embPathObject_free(EmbPathObject* pointer)
{
    embPointList_free(pointer->pointList);
    pointer->pointList = 0;
    embFlagList_free(pointer->flagList);
    pointer->flagList = 0;
    free(pointer);
    pointer = 0;
}

/**************************************************/
/* EmbPathObjectList                              */
/**************************************************/

EmbPathObjectList* embPathObjectList_create(EmbPathObject* data)
{
    EmbPathObjectList* heapPathObjList = (EmbPathObjectList*)malloc(sizeof(EmbPathObjectList));
    if(!heapPathObjList) { embLog_error("emb-path.c embPathObjectList_create(), cannot allocate memory for heapPathObjList\n"); return 0; }
    heapPathObjList->pathObj = data;
    heapPathObjList->next = 0;
    return heapPathObjList;
}

EmbPathObjectList* embPathObjectList_add(EmbPathObjectList* pointer, EmbPathObject* data)
{
    if(!pointer) { embLog_error("emb-path.c embPathObjectList_add(), pointer argument is null\n"); return 0; }
    pointer->next = (EmbPathObjectList*)malloc(sizeof(EmbPathObjectList));
    if(!pointer->next) { embLog_error("emb-path.c embPathObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->pathObj = data;
    pointer->next = 0;
    return pointer;
}

int embPathObjectList_count(EmbPathObjectList* pointer)
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

int embPathObjectList_empty(EmbPathObjectList* pointer)
{
    if(pointer == 0)
        return 1;
    return 0;
}

void embPathObjectList_free(EmbPathObjectList* pointer)
{
    EmbPathObjectList* tempPointer = pointer;
    EmbPathObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        embPathObject_free(tempPointer->pathObj);
        tempPointer->pathObj = 0;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
