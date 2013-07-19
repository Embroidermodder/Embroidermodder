#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include "helpers-misc.h"
#include "format-dsb.h"

int readDsb(EmbPattern* pattern, const char* fileName)
{
    FILE* file= fopen(fileName,"rb");
    if(file==0)
    {
        /*TODO: set messages here "Error opening DSB file for read:" */
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
        int stitchType = NORMAL;
        int x, y;
        unsigned char ctrl =fgetc(file);
        if(feof(file)) break;
        y = fgetc(file);
        if(feof(file)) break;
        x = fgetc(file);
        if(feof(file)) break;
        if(ctrl & 0x20)
            x = -x;
        if(ctrl & 0x40)
            y = -y;
        if(ctrl & 0x01)
            stitchType = TRIM;
		/* ctrl & 0x02 - Speed change? */
		/* ctrl & 0x04 - Clutch? */
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

int writeDsb(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeDsb */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
