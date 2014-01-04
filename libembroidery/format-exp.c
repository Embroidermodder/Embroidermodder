#include <stdio.h>
#include "emb-stitch.h"
#include "format-exp.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include "emb-file.h"

static char expDecode(unsigned char a1)
{
    return (a1 > 0x80) ? ((-~a1) - 1) : a1;
}

static void expEncode(unsigned char *b, char dx, char dy, int flags)
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
    EmbFile* file = 0;
    int i = 0;
    unsigned char b0 = 0, b1 = 0;
    char dx = 0, dy = 0;
    int flags = 0;

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        /*TODO: set status here "Error opening EXP file for read:" */
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

    for(i = 0; !embFile_eof(file); i++)
    {
        flags = NORMAL;
        b0 = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
            break;
        b1 = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
            break;
        if(b0 == 0x80)
        {
            if(b1 & 1)
            {
                b0 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                b1 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                flags = STOP;
            }
            else if((b1 == 2) || (b1 == 4) || b1 == 6)
            {
                flags = TRIM;
                if(b1 == 2) flags = NORMAL;
                b0 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                b1 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
            }
            else if(b1 == 0x80)
            {
                b0 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                b1 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
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
    embFile_close(file);
    return 1;
}

int writeExp(EmbPattern* pattern, const char* fileName)
{
#ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
#else /* ARDUINO TODO: This is temporary. Remove when complete. */

    EmbFile* file = 0;
    EmbStitchList* stitches = 0;
    double dx = 0.0, dy = 0.0;
    double xx = 0.0, yy = 0.0;
    int flags = 0;
    unsigned char b[4];

    file = embFile_open(fileName, "wb");
    if(!file)
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
        expEncode(b, (char)roundDouble(dx), (char)roundDouble(dy), flags);
        if((b[0] == 128) && ((b[1] == 1) || (b[1] == 2) || (b[1] == 4)))
        {
            /* fprintf(file, "%c%c%c%c", b[0], b[1], b[2], b[3]); TODO: finish embFile_printf */
        }
        else
        {
            /* fprintf(file, "%c%c", b[0], b[1]); TODO: finish embFile_printf */
        }
        stitches = stitches->next;
    }
    /* fprintf(file, "\x1a"); TODO: finish embFile_printf */
    embFile_close(file);
    return 1;
#endif /* ARDUINO TODO: This is temporary. Remove when complete. */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
