#include <math.h>
#include <stdlib.h>
#include "emb-vector.h"

void embVector_Normalize(EmbVector vector, EmbVector* result)
{
    /* TODO: pointer safety */
    double length = embVector_GetLength(vector);
    result->X = vector.X / length;
    result->Y = vector.Y / length;
}

void embVector_Multiply(EmbVector vector, double magnitude, EmbVector* result)
{
    /* TODO: pointer safety */
    result->X = vector.X * magnitude;
    result->Y = vector.Y * magnitude;
}

void embVector_Add(EmbVector v1, EmbVector v2, EmbVector* result)
{
    /* TODO: pointer safety */
    result->X = v1.X + v2.X;
    result->Y = v1.Y + v2.Y;
}

double embVector_GetLength(EmbVector vector)
{
    return sqrt(vector.X * vector.X + vector.Y * vector.Y);
}

EmbVectorList* embVectorList_create(EmbVector data)
{
    EmbVectorList* pointer = (EmbVectorList*)malloc(sizeof(EmbVectorList));
    /* TODO: malloc fail error */
    pointer->vector = data;
    pointer->next = 0;
    return pointer;
}

EmbVectorList* embVectorList_add(EmbVectorList* pointer, EmbVector data)
{
    if(!pointer) return embVectorList_create(data);
    pointer->next = (EmbVectorList*)malloc(sizeof(EmbVectorList));
    if(!pointer->next) return 0;
    pointer = pointer->next;
    pointer->vector = data;
    pointer->next = 0;
    return pointer;
}

int embVectorList_count(EmbVectorList* pointer)
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

int embVectorList_empty(EmbVectorList* pointer)
{
    if(pointer == 0)
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
    }
}
