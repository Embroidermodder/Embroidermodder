#include <stdio.h>
#include "format-txt.h"
#include "helpers-misc.h"

int readTxt(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish readTxt */
}

int writeTxt(EmbPattern* pattern, const char* fileName)
{
    EmbStitchList* pointer;
    FILE* file;
    file = fopen(fileName, "w");
    if(file == 0)
    {
        return 0;
    }
    pointer = pattern->stitchList;
    fprintf(file, "%u\n", embStitch_count(pointer));

    while(pointer)
    {
        EmbStitch s = pointer->stitch;
        fprintf(file, "%.1f,%.1f color:%i flags:%i\n", s.xx, s.yy, s.color, s.flags);
        pointer = pointer->next;
    }

    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
