#include "emb-polygon.h"
#include "emb-logging.h"
#include <stdlib.h>

/**************************************************/
/* EmbPolygonObject                              */
/**************************************************/

EmbPolygonObject* embPolygonObject_create(EmbPointList* points, EmbColor color, int lineType)
{
    EmbPolygonObject* heapPolygonObj = (EmbPolygonObject*)malloc(sizeof(EmbPolygonObject));
    if(!heapPolygonObj) { embLog_error("emb-polygon.c embPolygonObject_create(), cannot allocate memory for heapPolygonObj\n"); return 0; }
    heapPolygonObj->pointList = points;
    /* TODO: layer */
    heapPolygonObj->color = color;
    heapPolygonObj->lineType = lineType;
    return heapPolygonObj;
}

void embPolygonObject_free(EmbPolygonObject* pointer)
{
    embPointList_free(pointer->pointList);
    pointer->pointList = 0;
    free(pointer);
    pointer = 0;
}

/**************************************************/
/* EmbPolygonObjectList                           */
/**************************************************/

EmbPolygonObjectList* embPolygonObjectList_create(EmbPolygonObject* data)
{
    EmbPolygonObjectList* heapPolygonObjList = (EmbPolygonObjectList*)malloc(sizeof(EmbPolygonObjectList));
    if(!heapPolygonObjList) { embLog_error("emb-polygon.c embPolygonObjectList_create(), cannot allocate memory for heapPolygonObjList\n"); return 0; }
    heapPolygonObjList->polygonObj = data;
    heapPolygonObjList->next = 0;
    return heapPolygonObjList;
}

EmbPolygonObjectList* embPolygonObjectList_add(EmbPolygonObjectList* pointer, EmbPolygonObject* data)
{
    if(!pointer) { embLog_error("emb-polygon.c embPolygonObjectList_add(), pointer argument is null\n"); return 0; }
    pointer->next = (EmbPolygonObjectList*)malloc(sizeof(EmbPolygonObjectList));
    if(!pointer->next) { embLog_error("emb-polygon.c embPolygonObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->polygonObj = data;
    pointer->next = 0;
    return pointer;
}

int embPolygonObjectList_count(EmbPolygonObjectList* pointer)
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

int embPolygonObjectList_empty(EmbPolygonObjectList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embPolygonObjectList_free(EmbPolygonObjectList* pointer)
{
    EmbPolygonObjectList* tempPointer = pointer;
    EmbPolygonObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        embPolygonObject_free(tempPointer->polygonObj);
        tempPointer->polygonObj = 0;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
