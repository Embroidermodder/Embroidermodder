#include "format-zsk.h"
#include <stdio.h>

int readZsk(EmbPattern* pattern, const char* fileName)
{
    int b[3];
    FILE* file = fopen(fileName, "rb");

    if(file == 0)
        return 0;

    fseek(file, 512, SEEK_SET);

    while(fread(b, 1, 3, file) == 3)
    {
        if((b[2] & 25) == 0)
        {
            if(b[2] & 0x40)
                b[1] = -b[1];
            if(b[2] & 0x20)
                b[0] = -b[0];
            embPattern_addStitchRel(pattern, b[1] / 10.0, b[0] / 10.0, NORMAL, 1);
        }
        else
        {
            embPattern_addStitchRel(pattern, b[1] / 10.0, b[0] / 10.0, STOP, 1);
        }
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);
    return 1;
}

int writeZsk(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WriteZsk */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
