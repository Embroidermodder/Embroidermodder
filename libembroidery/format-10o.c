#include "format-10o.h"
#include "emb-logging.h"
#include <stdio.h>

int read10o(EmbPattern* pattern, const char* fileName)
{
    FILE* file = 0;

    if(!pattern) { embLog_error("format-10o.c read10o(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-10o.c read10o(), fileName argument is null\n"); return 0; }

    file = fopen(fileName,"rb");
    if(!file)
    {
        embLog_error("format-10o.c read10o(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);

    while(1)
    {
        int x, y;
        int stitchType = NORMAL;
        unsigned char ctrl = (unsigned char)fgetc(file);
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
    if(!pattern) { embLog_error("format-10o.c write10o(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-10o.c write10o(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish write10o */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
