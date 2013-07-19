#include "emb-circle.h"
#include <stdlib.h>

double embCircle_centerX(EmbCircle circle)
{
    return circle.centerX;
}

double embCircle_centerY(EmbCircle circle)
{
    return circle.centerY;
}

double embCircle_radius(EmbCircle circle)
{
    return circle.radius;
}

/* Returns an EmbCircleObject. It is created on the stack. */
EmbCircleObject embCircleObj_make(double cx, double cy, double r)
{
    EmbCircleObject stackCircleObj;
    stackCircleObj.circle.centerX = cx;
    stackCircleObj.circle.centerY = cy;
    stackCircleObj.circle.radius  = r;
    return stackCircleObj;
}

/* Returns an EmbCircleObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbCircleObject* embCircleObj_create(double cx, double cy, double r)
{
    EmbCircleObject* heapCircleObj = (EmbCircleObject*)malloc(sizeof(EmbCircleObject));
    heapCircleObj->circle.centerX = cx;
    heapCircleObj->circle.centerY = cy;
    heapCircleObj->circle.radius  = r;
    return heapCircleObj;
}

void embCircle_add(EmbCircleObjectList* pointer, EmbCircleObject data)
{
    pointer->next = (EmbCircleObjectList*)malloc(sizeof(EmbCircleObjectList));
    pointer = pointer->next;
    pointer->circleObj = data;
    pointer->next = 0;
}

int embCircle_count(EmbCircleObjectList* pointer)
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

int embCircle_empty(EmbCircleObjectList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
