#include <stdlib.h>
#include "emb-stitch.h"
#include <stdio.h>

void embStitch_add(EmbStitchList* pointer, EmbStitch data)
{
    pointer->next = (EmbStitchList*)malloc(sizeof(EmbStitchList));
    pointer = pointer->next;
    pointer->stitch = data;
    pointer->next = 0;
}

EmbStitch embStitch_getAt(EmbStitchList* pointer, int num)
{
    int i;
    for(i = 0; i < num; i++)
    {
        pointer = pointer->next;
    }
    return pointer->stitch;
}

/* TODO: Add a default parameter to handle returning count based on stitch flags. Currently, it includes JUMP and TRIM stitches, maybe we just want NORMAL stitches only or vice versa */
int embStitch_count(EmbStitchList* pointer)
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

int embStitch_empty(EmbStitchList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
