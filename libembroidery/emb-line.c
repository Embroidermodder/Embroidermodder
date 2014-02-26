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

EmbLineObjectList* embLineObjectList_create(EmbLineObject data)
{
    EmbLineObjectList* heapLineObjList = (EmbLineObjectList*)malloc(sizeof(EmbLineObjectList));
    if(!heapLineObjList) { embLog_error("emb-line.c embLineObjectList_create(), cannot allocate memory for heapLineObjList\n"); return 0; }
    heapLineObjList->lineObj = data;
    heapLineObjList->next = 0;
    return heapLineObjList;
}

EmbLineObjectList* embLineObjectList_add(EmbLineObjectList* pointer, EmbLineObject data)
{
    if(!pointer) { embLog_error("emb-line.c embLineObjectList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-line.c embLineObjectList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbLineObjectList*)malloc(sizeof(EmbLineObjectList));
    if(!pointer->next) { embLog_error("emb-line.c embLineObjectList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->lineObj = data;
    pointer->next = 0;
    return pointer;
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
    if(!pointer)
        return 1;
    return 0;
}

void embLineObjectList_free(EmbLineObjectList* pointer)
{
    EmbLineObjectList* tempPointer = pointer;
    EmbLineObjectList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* TODO: API Cleanup: This function should use an embLine parameter, not vector1/vector2, and look like this:
                      embLine_normalVector(EmbLine line, EmbVector* result, int clockwise) */
/*! Finds the normalized vector perpendicular (clockwise) to the line given by v1->v2 (normal to the line) */
void embLine_normalVector(EmbVector vector1, EmbVector vector2, EmbVector* result, int clockwise)
{
    double temp;
    if(!result) { embLog_error("emb-line.c embLine_normalVector(), result argument is null\n"); return; }
    result->X = vector2.X - vector1.X;
    result->Y = vector2.Y - vector1.Y;
    embVector_normalize(*result, result);
    temp = result->X;
    result->X = result->Y;
    result->Y = -temp;
    if(!clockwise)
    {
        result->X = -result->X;
        result->Y = -result->Y;
    }
}

/* TODO: API Cleanup: This is similar to the code we already have in geom-line.c, but may work well here.
         Also the result has NOTHING to do with vectors. It sets a point, not a vector, even though
         vector math is used internally inside this function. The function should look like this:
         embLine_intersectionPoint(EmbLine line1, EmbLine line2, EmbPoint* result)
         */
/*! Finds the intersection of two lines given by v1->v2 and v3->v4 and sets the value in the result variable */
void embLine_intersectionPoint(EmbVector v1, EmbVector v2, EmbVector v3, EmbVector v4, EmbVector* result)
{
    double A2 = v2.Y - v1.Y;
    double B2 = v1.X - v2.X;
    double C2 = A2 * v1.X + B2 * v1.Y;

    double A1 = v4.Y - v3.Y;
    double B1 = v3.X - v4.X;
    double C1 = A1 * v3.X + B1 * v3.Y;

    double det = A1 * B2 - A2 * B1;

    if(!result) { embLog_error("emb-line.c embLine_intersectionPoint(), result argument is null\n"); return; }
    /*TODO: The code below needs revised since division by zero can still occur */
    if(det < 1e-10 && det > -1e-10)
    {
        result->X = -10000; /* TODO: What is significant about these numbers? Leave the point, undefined */
        result->Y = -10000; /* TODO: A better solution would be to return an unsigned char(0 parallel, 1 intersecting, 2 not-intersecting) */
    }
    result->X = (B2 * C1 - B1 * C2) / det;
    result->Y = (A1 * C2 - A2 * C1) / det;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
