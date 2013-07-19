#include <stdlib.h>
#include "format-col.h"
#include "helpers-binary.h"

int readCol(EmbPattern* pattern, const char* fileName)
{
    EmbThreadList* colors;
    char num;
    unsigned char blue, green, red;
    int numberOfColors, i;

    FILE* file;
    file = fopen(fileName, "r");
    if(file==0)
    {
        /*TODO: set messages here "Error opening COL file for read:" */
        return 0;
    }
    colors = pattern->threadList;
    while(colors)
    {
        EmbThreadList* next = colors->next;
        free(colors);
        colors = next;
    }
    pattern->threadList = NULL;

    fscanf(file, "%d\r", &numberOfColors);
    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread t;
        char line[30];
        fscanf(file, "%s\r", line);
        sscanf(line,"%d,%d,%d,%d\n\r", &num, &blue, &green, &red);

        t.catalogNumber = "";
        t.description = "";
        t.color.r = red;
        t.color.g = green;
        t.color.b = blue;
        embPattern_addThread(pattern, t);
    }
    fclose(file);
    return 1;
}

int writeCol(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    int i, colorCount;
    EmbThreadList *colors;

    file = fopen(fileName, "w");
    if(file==0) {
        /*TODO: set messages here "Error creating/editing COL file for read:" */
        return 0;
    }
    colorCount = embThread_count(pattern->threadList);
    fprintf(file, "%d\n\r", colorCount);
    colors = pattern->threadList;
    i = 0;
    while(colors)
    {
        EmbColor c;
        c = colors->thread.color;
        fprintf(file, "%d,%d,%d,%d\n\r", i, c.r, c.g, c.b);
        i++;
        colors = colors->next;
    }
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
