#include "emb-point.h"
#include <stdlib.h>

double embPoint_x(EmbPoint point)
{
    return point.xx;
}

double embPoint_y(EmbPoint point)
{
    return point.yy;
}

/* Returns an EmbPointObject. It is created on the stack. */
EmbPointObject embPointObj_make(double x, double y)
{
    EmbPointObject stackPointObj;
    stackPointObj.point.xx = x;
    stackPointObj.point.yy = y;
    return stackPointObj;
}

/* Returns an EmbPointObject. It is created on the stack. */
EmbPoint embPoint_make(double x, double y)
{
    EmbPoint stackPoint;
    stackPoint.xx = x;
    stackPoint.yy = y;
    return stackPoint;
}

/* Returns an EmbPointObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbPointObject* embPointObj_create(double x, double y)
{
    EmbPointObject* heapPointObj = (EmbPointObject*)malloc(sizeof(EmbPointObject));
    heapPointObj->point.xx = x;
    heapPointObj->point.yy = y;
    return heapPointObj;
}

EmbPointObjectList* embPointObj_add(EmbPointObjectList* pointer, EmbPointObject data)
{
    pointer->next = (EmbPointObjectList*)malloc(sizeof(EmbPointObjectList));
    pointer = pointer->next;
    pointer->pointObj = data;
    pointer->next = 0;
    return pointer;
}

EmbPointList* embPoint_add(EmbPointList* pointer, EmbPoint data)
{
    pointer->next = (EmbPointList*)malloc(sizeof(EmbPointList));
    pointer = pointer->next;
    pointer->point = data;
    pointer->next = 0;
    return pointer;
}

int embPoint_count(EmbPointObjectList* pointer)
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

int embPoint_empty(EmbPointObjectList* pointer)
{
    return pointer == 0;
}

EmbPointList* embPointList_create(double x, double y)
{
    EmbPointList* heapPointObj = (EmbPointList*)malloc(sizeof(EmbPointList));
    heapPointObj->point.xx = x;
    heapPointObj->point.yy = y;
    heapPointObj->next = 0;
    return heapPointObj;
}

void embPointObjectList_free(EmbPointObjectList* list)
{
    EmbPointObjectList* current = list;
    EmbPointObjectList* next;
    while(current)
    {
        next= current->next;
        free(current);
        current = next;
    }
}

void embPointList_free(EmbPointList* list)
{
    EmbPointList* current = list;
    EmbPointList* next;
    while(current)
    {
        next= current->next;
        free(current);
        current = next;
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
