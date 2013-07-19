#include "emb-arc.h"
#include <stdlib.h>

/* Returns an EmbArcObject. It is created on the stack. */
EmbArcObject embArcObj_make(double sx, double sy, double mx, double my, double ex, double ey)
{
    EmbArcObject stackArcObj;
    stackArcObj.arc.startX = sx;
    stackArcObj.arc.startY = sy;
    stackArcObj.arc.midX   = mx;
    stackArcObj.arc.midY   = my;
    stackArcObj.arc.endX   = ex;
    stackArcObj.arc.endY   = ey;
    return stackArcObj;
}

/* Returns an EmbArcObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbArcObject* embArcObj_create(double sx, double sy, double mx, double my, double ex, double ey)
{
    EmbArcObject* heapArcObj = (EmbArcObject*)malloc(sizeof(EmbArcObject));
    heapArcObj->arc.startX = sx;
    heapArcObj->arc.startY = sy;
    heapArcObj->arc.midX   = mx;
    heapArcObj->arc.midY   = my;
    heapArcObj->arc.endX   = ex;
    heapArcObj->arc.endY   = ey;
    return heapArcObj;
}

void embArc_add(EmbArcObjectList* pointer, EmbArcObject data)
{
    while(pointer->next != 0)
    {
        pointer = pointer->next;
    }
    pointer->next = (EmbArcObjectList*)malloc(sizeof(EmbArcObjectList));
    pointer = pointer->next;
    pointer->arcObj = data;
    pointer->next = 0;
}

int embArc_count(EmbArcObjectList* pointer)
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

int embArc_empty(EmbArcObjectList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
