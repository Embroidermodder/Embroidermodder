#include "emb-line.h"
#include "emb-logging.h"
#include "emb-vector.h"
#include <stdlib.h>

/**************************************************/
/* EmbLine                                        */
/**************************************************/

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

/**************************************************/
/* EmbLineObject                                  */
/**************************************************/

/* Returns an EmbLineObject. It is created on the stack. */
EmbLineObject embLineObject_make(double x1, double y1, double x2, double y2)
{
    EmbLineObject stackLineObj;
    stackLineObj.line.x1 = x1;
    stackLineObj.line.y1 = y1;
    stackLineObj.line.x2 = x2;
    stackLineObj.line.y2 = y2;
    return stackLineObj;
}

/* Returns a pointer to an EmbLineObject. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbLineObject* embLineObject_create(double x1, double y1, double x2, double y2)
{
    EmbLineObject* heapLineObj = (EmbLineObject*)malloc(sizeof(EmbLineObject));
    if(!heapLineObj) { embLog_error("emb-line.c embLineObject_create(), cannot allocate memory for heapLineObj\n"); return 0; }
    heapLineObj->line.x1 = x1;
    heapLineObj->line.y1 = y1;
    heapLineObj->line.x2 = x2;
    heapLineObj->line.y2 = y2;
    return heapLineObj;
}

/**************************************************/
/* EmbLineObjectList                              */
/**************************************************/

void embLineObjectList_add(EmbLineObjectList* pointer, EmbLineObject data)
{
    if(!pointer) { embLog_error("emb-line.c embLineObjectList_add(), pointer argument is null\n"); return; }
    pointer->next = (EmbLineObjectList*)malloc(sizeof(EmbLineObjectList));
    if(!pointer->next) { embLog_error("emb-line.c embLineObjectList_add(), cannot allocate memory for pointer->next\n"); return; }
    pointer = pointer->next;
    pointer->lineObj = data;
    pointer->next = 0;
}

int embLineObjectList_count(EmbLineObjectList* pointer)
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

int embLineObjectList_empty(EmbLineObjectList* pointer)
{
    if(pointer == 0)
        return 1;
    return 0;
}

/*! Finds the normalized vector perpendicular (clockwise) to the line given by v1->v2 (normal to the line) */
void embLine_GetPerpendicularCWVector(EmbVector vector1, EmbVector vector2, EmbVector* result)
{
    double temp;
    if(!result) { embLog_error("emb-line.c embLine_GetPerpendicularCWVector(), result argument is null\n"); return; }
    result->X = vector2.X - vector1.X;
    result->Y = vector2.Y - vector1.Y;
    embVector_Normalize(*result, result);
    temp = result->X;
    result->X = result->Y;
    result->Y = -temp;
}

/*! Finds the intersection of two lines given by v1->v2 and v3->v4 and sets the value in the result variable */
void embLine_IntersectionWith(EmbVector v1, EmbVector v2, EmbVector v3, EmbVector v4, EmbVector* result)
{
    double A2 = v2.Y - v1.Y;
    double B2 = v1.X - v2.X;
    double C2 = A2 * v1.X + B2 * v1.Y;

    double A1 = v4.Y - v3.Y;
    double B1 = v3.X - v4.X;
    double C1 = A1 * v3.X + B1 * v3.Y;

    double det = A1 * B2 - A2 * B1;

    if(!result) { embLog_error("emb-line.c embLine_IntersectionWith(), result argument is null\n"); return; }
    if(det < 1e-10 && det > -1e-10)
    {
        result->X = -10000;
        result->Y = -10000;
    }
    result->X = (B2 * C1 - B1 * C2) / det;
    result->Y = (A1 * C2 - A2 * C1) / det;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
