#include "format-10o.h"
#include <stdio.h>

int read10o(EmbPattern* pattern, const char* fileName)
{
    FILE* file= fopen(fileName,"rb");
    if(file == 0)
    {
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

    while(1)
    {
        int x, y;
        int stitchType = NORMAL;
        unsigned char ctrl = fgetc(file);
        if(feof(file))
            break;
        y = fgetc(file);
        if(feof(file))
            break;
        x = fgetc(file);
        if(feof(file))
            break;
        if(ctrl & 0x20)
            x = -x;
        if(ctrl & 0x40)
            y = -y;
        if(ctrl & 0x01)
            stitchType = TRIM;
        if((ctrl & 0x5) == 5)
        {
            stitchType = STOP;
        }
        if(ctrl == 0xF8 || ctrl == 0x91 || ctrl == 0x87)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, stitchType, 1);
    }
    fclose(file);
    return 1;
}

int write10o(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish write10o */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
