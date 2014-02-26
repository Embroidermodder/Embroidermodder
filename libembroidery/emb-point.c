#include "emb-point.h"
#include "emb-logging.h"
#include <stdlib.h>

/**************************************************/
/* EmbPoint                                       */
/**************************************************/

double embPoint_x(EmbPoint point)
{
    return point.xx;
}

double embPoint_y(EmbPoint point)
{
    return point.yy;
}

/* Returns an EmbPointObject. It is created on the stack. */
EmbPoint embPoint_make(double x, double y)
{
    EmbPoint stackPoint;
    stackPoint.xx = x;
    stackPoint.yy = y;
    return stackPoint;
}

/**************************************************/
/* EmbPointList                                   */
/**************************************************/

EmbPointList* embPointList_create(double x, double y)
{
    EmbPointList* heapPointList = (EmbPointList*)malloc(sizeof(EmbPointList));
    if(!heapPointList) { embLog_error("emb-point.c embPointList_create(), cannot allocate memory for heapPointList\n"); return 0; }
    heapPointList->point.xx = x;
    heapPointList->point.yy = y;
    heapPointList->next = 0;
    return heapPointList;
}

EmbPointList* embPointList_add(EmbPointList* pointer, EmbPoint data)
{
    if(!pointer) { embLog_error("emb-point.c embPointList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-point.c embPointList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbPointList*)malloc(sizeof(EmbPointList));
    if(!pointer->next) { embLog_error("emb-point.c embPointList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->point = data;
    pointer->next = 0;
    return pointer;
}

int embPointList_count(EmbPointList* pointer)
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

int embPointList_empty(EmbPointList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embPointList_free(EmbPointList* pointer)
{
    EmbPointList* tempPointer = pointer;
    EmbPointList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/**************************************************/
/* EmbPointObject                                 */
/**************************************************/

/* Returns an EmbPointObject. It is created on the stack. */
EmbPointObject embPointObject_make(double x, double y)
{
    EmbPointObject stackPointObj;
    stackPointObj.point.xx = x;
    stackPointObj.point.yy = y;
    return stackPointObj;
}

/* Returns a pointer to an EmbPointObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbPointObject* embPointObject_create(double x, double y)
{
    EmbPointObject* heapPointObj = (EmbPointObject*)malloc(sizeof(EmbPointObject));
    if(!heapPointObj) { embLog_error("emb-point.c embPointObject_create(), cannot allocate memory for heapPointObj\n"); return 0; }
    heapPointObj->point.xx = x;
    heapPointObj->point.yy = y;
    return heapPointObj;
}

/**************************************************/
/* EmbPointObjectList                             */
/**************************************************/

EmbPointObjectList* embPointObjectList_create(EmbPointObject data)
{
    EmbPointObjectList* heapPointObjList = (EmbPointObjectList*)malloc(sizeof(EmbPointObjectList));
    if(!heapPointObjList) { embLog_error("emb-point.c embPointObjectList_create(), cannot allocate memory for heapPointObjList\n"); return 0; }
    heapPointObjList->pointObj = data;
    heapPointObjList->next = 0;
    return heapPointObjList;
}

EmbPointObjectList* embPointObjectList_add(EmbPointObjectList* pointer, EmbPointObject data)
{
    if(!pointer) { embLog_error("emb-point.c embPointObjectList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-point.c embPointObjectList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbPointObjectList*)malloc(sizeof(EmbPointObjectList));
    if(!pointer->next) { embLog_error("emb-point.c embPointObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->pointObj = data;
    pointer->next = 0;
    return pointer;
}

int embPointObjectList_count(EmbPointObjectList* pointer)
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

int embPointObjectList_empty(EmbPointObjectList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embPointObjectList_free(EmbPointObjectList* pointer)
{
    EmbPointObjectList* tempPointer = pointer;
    EmbPointObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
