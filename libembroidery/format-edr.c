#include "format-edr.h"
#include "helpers-binary.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>

int readEdr(EmbPattern* pattern, const char* fileName)
{
    int numberOfColors;
    int i;
    FILE* file = 0;

    file = fopen(fileName, "rb");
    if(!file)
    {
        /*TODO: set messages here "Error opening EDR file for read:" */
        return 0;
    }
    fseek(file, 0x00, SEEK_END);
    numberOfColors = ftell(file) / 4;
    fseek(file, 0x00, SEEK_SET);

    embThreadList_free(pattern->threadList);

    for(i = 0; i < numberOfColors; i++)
    {
        EmbThread t;
        t.color.r = binaryReadByte(file);
        t.color.g = binaryReadByte(file);
        t.color.b = binaryReadByte(file);
        binaryReadByte(file);
        embPattern_addThread(pattern, t);
    }
    fclose(file);
    return 1;
}

int writeEdr(EmbPattern* pattern, const char* fileName)
{
    EmbThreadList* pointer = 0;
    FILE* file = 0;

    file = fopen(fileName, "wb");
    if(!file)
    {
        /*TODO: set messages here "Error creating/editing EDR file for read:" */
        return 0;
    }
    pointer = pattern->threadList;
    while(pointer)
    {
        EmbColor c;
        c = pointer->thread.color;
        binaryWriteByte(file, c.r);
        binaryWriteByte(file, c.g);
        binaryWriteByte(file, c.b);
        binaryWriteByte(file, 0);
        pointer = pointer->next;
    }
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
