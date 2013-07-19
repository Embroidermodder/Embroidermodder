#include "format-exp.h"
#include <stdio.h>
#include "helpers-misc.h"
#include "emb-stitch.h"

char expDecode(unsigned char a1)
{
    return (a1 > 0x80) ? ((-~a1) - 1) : a1;
}

void expEncode(unsigned char *b, char dx, char dy, int flags)
{
    if(flags == TRIM)
    {
        b[0] = 128;
        b[1] = 2;
        b[2] = dx;
        b[3] = dy;
    }
    else if(flags == STOP)
    {
        b[0] = 128;
        b[1] = 1;
        b[2] = dx;
        b[3] = dy;
    }
    else
    {
        b[0] = dx;
        b[1] = dy;
    }
}

int readExp(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    int i = 0;
    unsigned char b0 = 0, b1 = 0;
    char dx = 0, dy = 0;
    int flags = 0;
    file = fopen(fileName, "rb");
    if(file == 0)
    {
        /*TODO: set status here "Error opening EXP file for read:" */
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

    for(i = 0; !feof(file); i++)
    {
        flags = NORMAL;
        b0 = fgetc(file);
        if(feof(file))
            break;
        b1 = fgetc(file);
        if(feof(file))
            break;
        if(b0 == 0x80)
        {
            if(b1 & 1)
            {
                b0 = fgetc(file);
                if(feof(file))
                    break;
                b1 = fgetc(file);
                if(feof(file))
                    break;
                flags = STOP;
            }
            else if((b1 == 2) || (b1 == 4) || b1 == 6)
            {
                flags = TRIM;
                if(b1 == 2) flags = NORMAL;
                b0 = fgetc(file);
                if(feof(file))
                    break;
                b1 = fgetc(file);
                if(feof(file))
                    break;
            }
            else if(b1 == 0x80)
            {
                b0 = fgetc(file);
                if(feof(file))
                    break;
                b1 = fgetc(file);
                if(feof(file))
                    break;
                /* Seems to be b0=0x07 and b1=0x00
                 * Maybe used as extension functions */
                b0 = 0;
                b1 = 0;
                flags = TRIM;
            }
        }
        dx = expDecode(b0);
        dy = expDecode(b1);
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);
    return 1;
}

int writeExp(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    EmbStitchList *stitches;
    double dx = 0.0, dy = 0.0;
    double xx = 0.0, yy = 0.0;
    int flags = 0;
    int i = 0;
    unsigned char b[4];

    file = fopen(fileName, "wb");
    if(file == 0)
    {
        /*TODO: set status here "Error opening EXP file for write:" */
        return 0;
    }

    /* write stitches */
    stitches = pattern->stitchList;
    while(stitches)
    {
        dx = stitches->stitch.xx * 10.0 - xx;
        dy = stitches->stitch.yy * 10.0 - yy;
        xx = stitches->stitch.xx * 10.0;
        yy = stitches->stitch.yy * 10.0;
        flags = stitches->stitch.flags;
        expEncode(b, roundDouble(dx), roundDouble(dy), flags);
        if((b[0] == 128) && ((b[1] == 1) || (b[1] == 2) || (b[1] == 4)))
        {
            fprintf(file, "%c%c%c%c", b[0], b[1], b[2], b[3]);
        }
        else
        {
            fprintf(file, "%c%c", b[0], b[1]);
        }
        stitches = stitches->next;
    }
    fprintf(file, "\x1a");
    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
