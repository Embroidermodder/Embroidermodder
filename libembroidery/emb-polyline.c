#include "emb-polyline.h"
#include <stdlib.h>
/**************************************************/
/* EmbPolylineObjectList                          */
/**************************************************/

EmbPolylineObjectList* embPolylineObjectList_create(EmbPolylineObject* data)
{
    EmbPolylineObjectList* heapPointObj = (EmbPolylineObjectList*)malloc(sizeof(EmbPolylineObjectList));
    /* TODO: malloc fail error */
    heapPointObj->polylineObj = data;
    heapPointObj->next = 0;
    return heapPointObj;
}

EmbPolylineObjectList* embPolylineObjectList_add(EmbPolylineObjectList* pointer, EmbPolylineObject* data)
{
    pointer->next = (EmbPolylineObjectList*)malloc(sizeof(EmbPolylineObjectList));
    /* TODO: malloc fail error */
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
    if(pointer == 0)
        return 1;
    return 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
