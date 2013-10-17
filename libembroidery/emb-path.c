#include "emb-path.h"
#include <stdlib.h>

#include <stdio.h>

int embPathObjectList_count(EmbPathObjectList* pointer)
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

int embPathObjectList_empty(EmbPathObjectList* pointer)
{
    if(pointer == 0)
        return 1;
    return 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
