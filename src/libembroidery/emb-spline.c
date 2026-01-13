#include "emb-spline.h"

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

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
