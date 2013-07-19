#include "format-dsz.h"
#include "helpers-binary.h"
#include "helpers-misc.h"

int readDsz(EmbPattern* pattern, const char* fileName)
{
    FILE* file = fopen(fileName,"rb");
    if(file==0)
    {
            /*TODO: set messages here "Error opening DSZ file for read:" */
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

    fseek(file, 0x200, SEEK_SET);
    while(1)
    {
        int x, y;
        unsigned char ctrl;
        int stitchType = NORMAL;

        y = fgetc(file);
        if(feof(file)) break;
        x = fgetc(file);
        if(feof(file)) break;
        ctrl =fgetc(file);
        if(feof(file)) break;
        if(ctrl & 0x01) stitchType = TRIM;
        if(ctrl & 0x20) y = -y;
        if(ctrl & 0x40) x = -x;

        if(ctrl & 0x0E)
        {
            int headNumber = (ctrl & 0x0E) >> 1;
            stitchType = STOP;
        }
        if(ctrl & 0x10)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }
        embPattern_addStitchRel(pattern, x  / 10.0, y  / 10.0, stitchType, 1);
    }
    fclose(file);
    return 1;
}

int writeDsz(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeDsz */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
