#include "emb-thread.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>

int embThread_findNearestColor(EmbColor color, EmbThreadList* colors)
{
    double currentClosestValue = 9999999;
    int closestIndex = -1;
    int red = color.r;
    int green = color.g;
    int blue = color.b;
    int i = 0;
    EmbThreadList* currentThreadItem = colors;

    while(currentThreadItem != NULL)
    {
        int deltaRed;
        int deltaBlue;
        int deltaGreen;
        double dist;
        EmbColor c;
        c = currentThreadItem->thread.color;

        deltaRed = red - c.r;
        deltaBlue = green - c.g;
        deltaGreen = blue - c.b;

        dist = sqrt((double)(deltaRed * deltaRed) + (deltaBlue * deltaBlue) + (deltaGreen * deltaGreen));
        if(dist <= currentClosestValue)
        {
            currentClosestValue = dist;
            closestIndex = i;
        }
        currentThreadItem = currentThreadItem->next;
        i++;
    }
    return closestIndex;
}

int embThread_findNearestColorInArray(EmbColor color, EmbThread* colorArray, int count)
{
    double currentClosestValue = 9999999;
    int closestIndex = -1;
    int red = color.r;
    int green = color.g;
    int blue = color.b;
    int i = 0;
    for(i = 0; i < count; i++)
    {
        int deltaRed;
        int deltaBlue;
        int deltaGreen;
        double dist;
        EmbColor c;
        c = colorArray[i].color;

        deltaRed = red - c.r;
        deltaBlue = green - c.g;
        deltaGreen = blue - c.b;

        dist = sqrt((double)(deltaRed * deltaRed) + (deltaBlue * deltaBlue) + (deltaGreen * deltaGreen));
        if(dist <= currentClosestValue)
        {
            currentClosestValue = dist;
            closestIndex = i;
        }
    }
    return closestIndex;
}

EmbThread embThread_getRandom(void)
{
    EmbThread c;
    c.color.r = rand()%256;
    c.color.g = rand()%256;
    c.color.b = rand()%256;
    c.description = "random";
    c.catalogNumber = "";
    return c;
}

EmbThreadList* embThreadList_create(EmbThread data)
{
    EmbThreadList* heapThreadList = (EmbThreadList*)malloc(sizeof(EmbThreadList));
    if(!heapThreadList) { embLog_error("emb-thread.c embThreadList_create(), cannot allocate memory for heapThreadList\n"); return 0; }
    heapThreadList->thread = data;
    heapThreadList->next = 0;
    return heapThreadList;
}

EmbThreadList* embThreadList_add(EmbThreadList* pointer, EmbThread data)
{
    if(!pointer) { embLog_error("emb-thread.c embThreadList_add(), pointer argument is null\n"); return 0; }
    if(pointer->next) { embLog_error("emb-thread.c embThreadList_add(), pointer->next should be null\n"); return 0; }
    pointer->next = (EmbThreadList*)malloc(sizeof(EmbThreadList));
    if(!pointer->next) { embLog_error("emb-thread.c embThreadList_add(), cannot allocate memory for pointer->next\n"); return 0; }
    pointer = pointer->next;
    pointer->thread = data;
    pointer->next = 0;
    return pointer;
}

EmbThread embThreadList_getAt(EmbThreadList* pointer, int num)
{
    /* TODO: pointer safety */
    int i = 0;
    for(i = 0; i < num; i++)
    {
        if(pointer->next)
        {
            pointer = pointer->next;
        }
    }
    return pointer->thread;
}

int embThreadList_count(EmbThreadList* pointer)
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

int embThreadList_empty(EmbThreadList* pointer)
{
    if(!pointer)
        return 1;
    return 0;
}

void embThreadList_free(EmbThreadList* pointer)
{
    EmbThreadList* tempPointer = pointer;
    EmbThreadList* nextPointer = 0;
    while(tempPointer)
    {
        nextPointer = tempPointer->next;
        free(tempPointer);
        tempPointer = nextPointer;
    }
	pointer = 0;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
