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
    int i = 0; /*TODO: Should start at 1. review this change to ensure it doesn't break any formats. */
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
    if(pointer == 0)
        return 1;
    return 0;
}

void embStitchList_free(EmbStitchList* pointer)
{
    EmbStitchList* tempPointer = pointer;
    EmbStitchList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
    pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
