#include "emb-path.h"
#include <stdlib.h>

#include <stdio.h>



int embPath_count(EmbPathObjectList* pointer)
{
    int i = 1; /* TODO: This should be set to 1. It's clearly wrong in multiple other places. Needs reviewed. */
    if(!pointer) return 0;
    while(pointer->next)
    {
        pointer = pointer->next;
        i++;
    }
    return i;
}

int embPath_empty(EmbPathObjectList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
