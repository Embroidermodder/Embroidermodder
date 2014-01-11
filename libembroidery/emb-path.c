#include "emb-path.h"
#include "emb-logging.h"
#include <stdlib.h>

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
        embPointList_free(tempPointer->pathObj->pointList);
        tempPointer->pathObj->pointList = 0;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
