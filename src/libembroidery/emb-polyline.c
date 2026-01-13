#include "emb-polyline.h"
#include "emb-logging.h"
#include <stdlib.h>

/**************************************************/
/* EmbPolylineObject                              */
/**************************************************/

EmbPolylineObject* embPolylineObject_create(EmbPointList* pointList, EmbColor color, int lineType)
{
    EmbPolylineObject* heapPolylineObj = 0;
    if(!pointList) { embLog_error("emb-polyline.c embPolylineObject_create(), pointList argument is null\n"); return 0; }
    heapPolylineObj = (EmbPolylineObject*)malloc(sizeof(EmbPolylineObject));
    if(!heapPolylineObj) { embLog_error("emb-polyline.c embPolylineObject_create(), cannot allocate memory for heapPolylineObj\n"); return 0; }
    heapPolylineObj->pointList = pointList;
    /* TODO: layer */
    heapPolylineObj->color = color;
    heapPolylineObj->lineType = lineType;
    return heapPolylineObj;
}

void embPolylineObject_free(EmbPolylineObject* pointer)
{
    embPointList_free(pointer->pointList);
    pointer->pointList = 0;
    free(pointer);
    pointer = 0;
}

/**************************************************/
/* EmbPolylineObjectList                          */
/**************************************************/

EmbPolylineObjectList* embPolylineObjectList_create(EmbPolylineObject* data)
{
    EmbPolylineObjectList* heapPolylineObjList = 0;
    if(!data) { embLog_error("emb-polyline.c embPolylineObjectList_create(), data argument is null\n"); return 0; }
    heapPolylineObjList = (EmbPolylineObjectList*)malloc(sizeof(EmbPolylineObjectList));
    if(!heapPolylineObjList) { embLog_error("emb-polyline.c embPolylineObjectList_create(), cannot allocate memory for heapPolylineObjList\n"); return 0; }
    heapPolylineObjList->polylineObj = data;
    heapPolylineObjList->next = 0;
    return heapPolylineObjList;
}

EmbPolylineObjectList* embPolylineObjectList_add(EmbPolylineObjectList* pointer, EmbPolylineObject* data)
{
    if(!pointer) { embLog_error("emb-polyline.c embPolylineObjectList_add(), pointer argument is null\n"); return 0; }
    if(!data) { embLog_error("emb-polyline.c embPolylineObjectList_add(), data argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-polyline.c embPolylineObjectList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbPolylineObjectList*)malloc(sizeof(EmbPolylineObjectList));
    if(!pointer->next) { embLog_error("emb-polyline.c embPolylineObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->polylineObj = data;
    pointer->next = 0;
    return pointer;
}

int embPolylineObjectList_count(EmbPolylineObjectList* pointer)
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

int embPolylineObjectList_empty(EmbPolylineObjectList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embPolylineObjectList_free(EmbPolylineObjectList* pointer)
{
    EmbPolylineObjectList* tempPointer = pointer;
    EmbPolylineObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        embPolylineObject_free(tempPointer->polylineObj);
        tempPointer->polylineObj = 0;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
