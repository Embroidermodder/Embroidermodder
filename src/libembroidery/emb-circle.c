#include "emb-circle.h"
#include "emb-logging.h"
#include <stdlib.h>

/**************************************************/
/* EmbCircle                                      */
/**************************************************/

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

/**************************************************/
/* EmbCircleObject                                */
/**************************************************/

/* Returns an EmbCircleObject. It is created on the stack. */
EmbCircleObject embCircleObject_make(double cx, double cy, double r)
{
    EmbCircleObject stackCircleObj;
    stackCircleObj.circle.centerX = cx;
    stackCircleObj.circle.centerY = cy;
    stackCircleObj.circle.radius  = r;
    return stackCircleObj;
}

/* Returns a pointer to an EmbCircleObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbCircleObject* embCircleObject_create(double cx, double cy, double r)
{
    EmbCircleObject* heapCircleObj = (EmbCircleObject*)malloc(sizeof(EmbCircleObject));
    if(!heapCircleObj) { embLog_error("emb-circle.c embCircleObject_create(), cannot allocate memory for heapCircleObj\n"); return 0; }
    heapCircleObj->circle.centerX = cx;
    heapCircleObj->circle.centerY = cy;
    heapCircleObj->circle.radius  = r;
    return heapCircleObj;
}

/**************************************************/
/* EmbCircleObjectList                            */
/**************************************************/

EmbCircleObjectList* embCircleObjectList_create(EmbCircleObject data)
{
    EmbCircleObjectList* heapCircleObjList = (EmbCircleObjectList*)malloc(sizeof(EmbCircleObjectList));
    if(!heapCircleObjList) { embLog_error("emb-circle.c embCircleObjectList_create(), cannot allocate memory for heapCircleObjList\n"); return 0; }
    heapCircleObjList->circleObj = data;
    heapCircleObjList->next = 0;
    return heapCircleObjList;
}

EmbCircleObjectList* embCircleObjectList_add(EmbCircleObjectList* pointer, EmbCircleObject data)
{
    if(!pointer) { embLog_error("emb-circle.c embCircleObjectList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-circle.c embCircleObjectList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbCircleObjectList*)malloc(sizeof(EmbCircleObjectList));
    if(!pointer->next) { embLog_error("emb-circle.c embCircleObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->circleObj = data;
    pointer->next = 0;
    return pointer;
}

int embCircleObjectList_count(EmbCircleObjectList* pointer)
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

int embCircleObjectList_empty(EmbCircleObjectList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embCircleObjectList_free(EmbCircleObjectList* pointer)
{
    EmbCircleObjectList* tempPointer = pointer;
    EmbCircleObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
