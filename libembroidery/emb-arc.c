#include "emb-arc.h"
#include "emb-logging.h"
#include <stdlib.h>

/* Returns an EmbArcObject. It is created on the stack. */
EmbArcObject embArcObject_make(double sx, double sy, double mx, double my, double ex, double ey)
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

/* Returns a pointer to an EmbArcObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbArcObject* embArcObject_create(double sx, double sy, double mx, double my, double ex, double ey)
{
    EmbArcObject* heapArcObj = (EmbArcObject*)malloc(sizeof(EmbArcObject));
    if(!heapArcObj) return 0;
    heapArcObj->arc.startX = sx;
    heapArcObj->arc.startY = sy;
    heapArcObj->arc.midX   = mx;
    heapArcObj->arc.midY   = my;
    heapArcObj->arc.endX   = ex;
    heapArcObj->arc.endY   = ey;
    return heapArcObj;
}

EmbArcObjectList* embArcObjectList_add(EmbArcObjectList* pointer, EmbArcObject data)
{
    if(!pointer) { embLog_error("emb-arc.c embArcObjectList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-arc.c embArcObjectList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbArcObjectList*)malloc(sizeof(EmbArcObjectList));
    if(!pointer->next) { embLog_error("emb-arc.c embArcObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->arcObj = data;
    pointer->next = 0;
    return pointer;
}

int embArcObjectList_count(EmbArcObjectList* pointer)
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

int embArcObjectList_empty(EmbArcObjectList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embArcObjectList_free(EmbArcObjectList* pointer)
{
    EmbArcObjectList* tempPointer = pointer;
    EmbArcObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
