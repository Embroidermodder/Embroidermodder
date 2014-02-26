#include "emb-stitch.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>

EmbStitchList* embStitchList_create(EmbStitch data)
{
    EmbStitchList* heapStitchList = (EmbStitchList*)malloc(sizeof(EmbStitchList));
    if(!heapStitchList) { embLog_error("emb-stitch.c embStitchList_create(), cannot allocate memory for heapStitchList\n"); return 0; }
    heapStitchList->stitch = data;
    heapStitchList->next = 0;
    return heapStitchList;
}

EmbStitchList* embStitchList_add(EmbStitchList* pointer, EmbStitch data)
{
    if(!pointer) { embLog_error("emb-stitch.c embStitchList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-stitch.c embStitchList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbStitchList*)malloc(sizeof(EmbStitchList));
    if(!pointer->next) { embLog_error("emb-stitch.c embStitchList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->stitch = data;
    pointer->next = 0;
    return pointer;
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
    int i = 1;
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
    if(!pointer)
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
