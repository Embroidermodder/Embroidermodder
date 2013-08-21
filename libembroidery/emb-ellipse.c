#include "emb-ellipse.h"
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
    /* TODO: malloc fail error */
    heapEllipseObj->ellipse.centerX = cx;
    heapEllipseObj->ellipse.centerY = cy;
    heapEllipseObj->ellipse.radiusX = rx;
    heapEllipseObj->ellipse.radiusY = ry;
    return heapEllipseObj;
}

/**************************************************/
/* EmbEllipseObjectList                           */
/**************************************************/

void embEllipseObjectList_add(EmbEllipseObjectList* pointer, EmbEllipseObject data)
{
    /* TODO: pointer safety */
    pointer->next = (EmbEllipseObjectList*)malloc(sizeof(EmbEllipseObjectList));
    /* TODO: malloc fail error */
    pointer = pointer->next;
    pointer->ellipseObj = data;
    pointer->next = 0;
}

int embEllipseObjectList_count(EmbEllipseObjectList* pointer)
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

int embEllipseObjectList_empty(EmbEllipseObjectList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
