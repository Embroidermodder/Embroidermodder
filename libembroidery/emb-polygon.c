#include "emb-polygon.h"
#include <stdlib.h>

/**************************************************/
/* EmbPolygonObjectList                           */
/**************************************************/

EmbPolygonObjectList* embPolygonObjectList_create(EmbPolygonObject* data)
{
    EmbPolygonObjectList* heapPointObj = (EmbPolygonObjectList*)malloc(sizeof(EmbPolygonObjectList));
    /* TODO: malloc fail error */
    heapPointObj->polygonObj = data;
    heapPointObj->next = 0;
    return heapPointObj;
}

EmbPolygonObjectList* embPolygonObjectList_add(EmbPolygonObjectList* pointer, EmbPolygonObject* data)
{
    pointer->next = (EmbPolygonObjectList*)malloc(sizeof(EmbPolygonObjectList));
    /* TODO: malloc fail error */
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
    if(pointer == 0)
        return 1;
    return 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
