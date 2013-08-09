#include "emb-polyline.h"

EmbPolylineObjectList* embPolylineObjectList_create(EmbPolylineObject* data)
{
    EmbPolylineObjectList* heapPointObj = (EmbPolylineObjectList*)malloc(sizeof(EmbPolylineObjectList));
    heapPointObj->polylineObj = data;
    heapPointObj->next = 0;
    return heapPointObj;
}

EmbPointObjectList* embPolylineObjectList_add(EmbPolylineObjectList* pointer, EmbPolylineObject* data)
{
    pointer->next = (EmbPointObjectList*)malloc(sizeof(EmbPolylineObjectList));
    pointer = pointer->next;
    pointer->polylineObj = data;
    pointer->next = 0;

    return pointer;
}

int embPolylineObjectList_count(EmbPolylineObjectList* pointer)
{
    int i = 0;
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
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
