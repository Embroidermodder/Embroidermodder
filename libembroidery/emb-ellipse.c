#include "emb-ellipse.h"
#include <stdlib.h>

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

/* Returns an EmbEllipseObject. It is created on the stack. */
EmbEllipseObject embEllipseObj_make(double cx, double cy, double rx, double ry)
{
    EmbEllipseObject stackEllipseObj;
    stackEllipseObj.ellipse.centerX = cx;
    stackEllipseObj.ellipse.centerY = cy;
    stackEllipseObj.ellipse.radiusX = rx;
    stackEllipseObj.ellipse.radiusY = ry;
    return stackEllipseObj;
}

/* Returns an EmbEllipseObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbEllipseObject* embEllipseObj_create(double cx, double cy, double rx, double ry)
{
    EmbEllipseObject* heapEllipseObj = (EmbEllipseObject*)malloc(sizeof(EmbEllipseObject));
    heapEllipseObj->ellipse.centerX = cx;
    heapEllipseObj->ellipse.centerY = cy;
    heapEllipseObj->ellipse.radiusX = rx;
    heapEllipseObj->ellipse.radiusY = ry;
    return heapEllipseObj;
}

void embEllipse_add(EmbEllipseObjectList* pointer, EmbEllipseObject data)
{
    pointer->next = (EmbEllipseObjectList*)malloc(sizeof(EmbEllipseObjectList));
    pointer = pointer->next;
    pointer->ellipseObj = data;
    pointer->next = 0;
}

int embEllipse_count(EmbEllipseObjectList* pointer)
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

int embEllipse_empty(EmbEllipseObjectList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
