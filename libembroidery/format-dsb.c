#include "format-dsb.h"
#include "helpers-misc.h"
#include "emb-logging.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readDsb(EmbPattern* pattern, const char* fileName)
{
    FILE* file = 0;

    if(!pattern) { embLog_error("format-dsb.c readDsb(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dsb.c readDsb(), fileName argument is null\n"); return 0; }

    file = fopen(fileName,"rb");
    if(file==0)
    {
        embLog_error("format-dsb.c readDsb(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);
    /*TODO: READ 512 BYTE HEADER INTO header[] */
    /*
    for(i=0;i<512;i++)
    {
        header[i]=fgetc(file);
    }
    */
    fseek(file, 0x200, SEEK_SET);
    while(1)
    {
        int x, y;
        unsigned char ctrl;
        int stitchType = NORMAL;

        ctrl =(unsigned char)fgetc(file);
        if(feof(file)) break;
        y = fgetc(file);
        if(feof(file)) break;
        x = fgetc(file);
        if(feof(file)) break;
        if(ctrl & 0x01) stitchType = TRIM;
        if(ctrl & 0x20) x = -x;
        if(ctrl & 0x40) y = -y;
        /* ctrl & 0x02 - Speed change? */ /* TODO: review this line */
        /* ctrl & 0x04 - Clutch? */       /* TODO: review this line */
        if((ctrl & 0x05) == 0x05)
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

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeDsb(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-dsb.c writeDsb(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dsb.c writeDsb(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-dsb.c writeDsb(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeDsb */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
