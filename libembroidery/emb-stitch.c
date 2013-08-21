#include <stdlib.h>
#include "emb-stitch.h"
#include <stdio.h>

void embStitchList_add(EmbStitchList* pointer, EmbStitch data)
{
    /* TODO: pointer safety */
    pointer->next = (EmbStitchList*)malloc(sizeof(EmbStitchList));
    /* TODO: malloc fail error */
    pointer = pointer->next;
    pointer->stitch = data;
    pointer->next = 0;
}

EmbStitch embStitchList_getAt(EmbStitchList* pointer, int num)
{
    /* TODO: pointer safety */
    int i;
    for(i = 0; i < num; i++)
    {
        pointer = pointer->next;
    }
    return pointer->stitch;
}

/* TODO: Add a default parameter to handle returning count based on stitch flags. Currently, it includes JUMP and TRIM stitches, maybe we just want NORMAL stitches only or vice versa */
int embStitchList_count(EmbStitchList* pointer)
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

int embStitchList_empty(EmbStitchList* pointer)
{
    return pointer == 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
