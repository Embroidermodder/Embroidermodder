#include "emb-line.h"
#include <stdlib.h>

double embLine_x1(EmbLine line)
{
    return line.x1;
}

double embLine_y1(EmbLine line)
{
    return line.y1;
}

double embLine_x2(EmbLine line)
{
    return line.x2;
}

double embLine_y2(EmbLine line)
{
    return line.y2;
}

/* Returns an EmbLineObject. It is created on the stack. */
EmbLineObject embLineObj_make(double x1, double y1, double x2, double y2)
{
    EmbLineObject stackLineObj;
    stackLineObj.line.x1 = x1;
    stackLineObj.line.y1 = y1;
    stackLineObj.line.x2 = x2;
    stackLineObj.line.y2 = y2;
    return stackLineObj;
}

/* Returns an EmbLineObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbLineObject* embLineObj_create(double x1, double y1, double x2, double y2)
{
    EmbLineObject* heapLineObj = (EmbLineObject*)malloc(sizeof(EmbLineObject));
    heapLineObj->line.x1 = x1;
    heapLineObj->line.y1 = y1;
    heapLineObj->line.x2 = x2;
    heapLineObj->line.y2 = y2;
    return heapLineObj;
}

void embLine_add(EmbLineObjectList* pointer, EmbLineObject data)
{
    pointer->next = (EmbLineObjectList*)malloc(sizeof(EmbLineObjectList));
    pointer = pointer->next;
    pointer->lineObj = data;
    pointer->next = 0;
}

int embLine_count(EmbLineObjectList* pointer)
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

int embLine_empty(EmbLineObjectList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
