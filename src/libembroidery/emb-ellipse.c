#include "emb-ellipse.h"
#include "emb-logging.h"
#include <stdlib.h>

/**************************************************/
/* EmbEllipse                                     */
/**************************************************/

double embEllipse_centerX(EmbEllipse ellipse)
{
    return ellipse.centerX;
}

double embEllipse_centerY(EmbEllipse ellipse)
{
    return ellipse.centerY;
}

double embEllipse_radiusX(EmbEllipse ellipse)
{
    return ellipse.radiusX;
}

double embEllipse_radiusY(EmbEllipse ellipse)
{
    return ellipse.radiusY;
}

double embEllipse_diameterX(EmbEllipse ellipse)
{
    return ellipse.radiusX * 2.0;
}

double embEllipse_diameterY(EmbEllipse ellipse)
{
    return ellipse.radiusY * 2.0;
}

double embEllipse_width(EmbEllipse ellipse)
{
    return ellipse.radiusX * 2.0;
}

double embEllipse_height(EmbEllipse ellipse)
{
    return ellipse.radiusY * 2.0;
}

/**************************************************/
/* EmbEllipseObject                               */
/**************************************************/

/* Returns an EmbEllipseObject. It is created on the stack. */
EmbEllipseObject embEllipseObject_make(double cx, double cy, double rx, double ry)
{
    EmbEllipseObject stackEllipseObj;
    stackEllipseObj.ellipse.centerX = cx;
    stackEllipseObj.ellipse.centerY = cy;
    stackEllipseObj.ellipse.radiusX = rx;
    stackEllipseObj.ellipse.radiusY = ry;
    return stackEllipseObj;
}

/* Returns a pointer to an EmbEllipseObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbEllipseObject* embEllipseObject_create(double cx, double cy, double rx, double ry)
{
    EmbEllipseObject* heapEllipseObj = (EmbEllipseObject*)malloc(sizeof(EmbEllipseObject));
    if(!heapEllipseObj) { embLog_error("emb-ellipse.c embEllipseObject_create(), cannot allocate memory for heapEllipseObj\n"); return 0; }
    heapEllipseObj->ellipse.centerX = cx;
    heapEllipseObj->ellipse.centerY = cy;
    heapEllipseObj->ellipse.radiusX = rx;
    heapEllipseObj->ellipse.radiusY = ry;
    return heapEllipseObj;
}

/**************************************************/
/* EmbEllipseObjectList                           */
/**************************************************/

EmbEllipseObjectList* embEllipseObjectList_create(EmbEllipseObject data)
{
    EmbEllipseObjectList* heapEllipseObjList = (EmbEllipseObjectList*)malloc(sizeof(EmbEllipseObjectList));
    if(!heapEllipseObjList) { embLog_error("emb-ellipse.c embEllipseObjectList_create(), cannot allocate memory for heapEllipseObjList\n"); return 0; }
    heapEllipseObjList->ellipseObj = data;
    heapEllipseObjList->next = 0;
    return heapEllipseObjList;
}

EmbEllipseObjectList* embEllipseObjectList_add(EmbEllipseObjectList* pointer, EmbEllipseObject data)
{
    if(!pointer) { embLog_error("emb-ellipse.c embEllipseObjectList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-ellipse.c embEllipseObjectList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbEllipseObjectList*)malloc(sizeof(EmbEllipseObjectList));
    if(!pointer->next) { embLog_error("emb-ellipse.c embEllipseObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->ellipseObj = data;
    pointer->next = 0;
    return pointer;
}

int embEllipseObjectList_count(EmbEllipseObjectList* pointer)
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

int embEllipseObjectList_empty(EmbEllipseObjectList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embEllipseObjectList_free(EmbEllipseObjectList* pointer)
{
    EmbEllipseObjectList* tempPointer = pointer;
    EmbEllipseObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
