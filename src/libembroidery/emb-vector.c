#include "embroidery.h"

#include <math.h>
#include <stdlib.h>

EmbVector embVector(double x, double y)
{
    EmbVector v;
    v.x = x;
    v.y = y;
    return v;
}

/**
 * This is used to test the correctness of other code in this file, so
 * we avoid calling other functions here.
 */
int embVector_approxEqual(EmbVector a, EmbVector b, double tolerance)
{
    double cx, cy;
    cx = b.x - a.x;
    cy = b.y - a.y;
    return sqrt(cx * cx + cy * cy) < tolerance;
}

EmbVector embVector_normalize(EmbVector vector)
{
    double length = embVector_getLength(vector);
    return embVector_scale(vector, 1.0 / length);
}

EmbVector embVector_scale(EmbVector vector, double magnitude)
{
    return embVector(vector.x * magnitude, vector.y * magnitude);
}

EmbVector embVector_add(EmbVector a, EmbVector b)
{
    return embVector(a.x + b.x, a.y + b.y);
}

EmbVector embVector_subtract(EmbVector a, EmbVector b)
{
    return embVector(a.x - b.x, a.y - b.y);
}

EmbVector embVector_average(EmbVector a, EmbVector b)
{
    return embVector((a.x + b.x) / 2.0, (a.y + b.y) / 2.0);
}

double embVector_getLength(EmbVector vector)
{
    return sqrt(vector.x * vector.x + vector.y * vector.y);
}

EmbVectorList* embVectorList_create(EmbVector data)
{
    EmbVectorList* pointer = (EmbVectorList*)malloc(sizeof(EmbVectorList));
    if (!pointer) {
        embLog_error("emb-vector.c embVectorList_create(), cannot allocate memory for pointer\n");
        return 0;
    }
    pointer->vector = data;
    pointer->next = 0;
    return pointer;
}

EmbVectorList* embVectorList_add(EmbVectorList* pointer, EmbVector data)
{
    if (!pointer) { embLog_error("emb-vector.c embVectorList_add(), pointer argument is null\n"); return 0; }
    if (pointer->next) { embLog_error("emb-vector.c embVectorList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbVectorList*)malloc(sizeof(EmbVectorList));
    if (!pointer->next) { embLog_error("emb-vector.c embVectorList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->vector = data;
    pointer->next = 0;
    return pointer;
}

int embVectorList_count(EmbVectorList* pointer)
{
    int i = 1;
    if (!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embVectorList_empty(EmbVectorList* pointer)
{
    if (!pointer)
        return 1;
    return 0;
}

void embVectorList_free(EmbVectorList* pointer)
{
    while(pointer)
    {
        EmbVectorList* temp = pointer;
        pointer = pointer->next;
        free(temp);
        temp = 0;
    }
}

