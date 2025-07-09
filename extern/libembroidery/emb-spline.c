#include "embroidery.h"

int embSplineObjectList_count(EmbSplineObjectList* pointer)
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

int embSplineObjectList_empty(EmbSplineObjectList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

