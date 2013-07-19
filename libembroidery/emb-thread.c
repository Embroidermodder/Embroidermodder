#include "emb-thread.h"
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


void embThread_add(EmbThreadList* pointer, EmbThread data)
{
    EmbThreadList* tempPointer = pointer;
    while(tempPointer->next)
    {
        tempPointer = tempPointer->next;
    }
    tempPointer->next = (EmbThreadList*)malloc(sizeof(EmbThreadList));
    tempPointer = tempPointer->next;
    tempPointer->thread = data;
    tempPointer->next = (EmbThreadList*)NULL;
}

EmbThread embThread_getRandom()
{
    EmbThread c;
    c.color.r = rand()%256;
    c.color.g = rand()%256;
    c.color.b = rand()%256;
    c.description = "random";
    c.catalogNumber = "";
    return c;
}

EmbThread embThread_getAt(EmbThreadList* pointer, int num)
{
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

int embThread_count(EmbThreadList* pointer)
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

int embThread_empty(EmbThreadList* pointer)
{
    return pointer == NULL;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
