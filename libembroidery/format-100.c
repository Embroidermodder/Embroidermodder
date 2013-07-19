#include <stdio.h>
#include "format-100.h"

int read100(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    int x,y;
    int stitchType;
    unsigned char b[4];

    file= fopen(fileName,"rb");
    if(file==0)
    {
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);
    while(fread(b, 1, 4, file) == 4)
    {
        stitchType = NORMAL;
        x = (b[2] > 0x80) ? -(b[2] - 0x80) : b[2];
        y = (b[3] > 0x80) ? -(b[3] - 0x80) : b[3];
        /*if(!(b[0] & 0xFC)) stitchType = JUMP; TODO: review & fix */
        if(!(b[0] & 0x01)) stitchType = STOP;
        if(b[0] == 0x1F) stitchType = END;
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, stitchType, 1);
    }
    fclose(file);
    return 1;
}

int write100(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish write100 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
