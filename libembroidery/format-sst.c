#include <stdio.h>
#include "format-sst.h"
#include "helpers-binary.h"

int readSst(EmbPattern* pattern, const char* fileName)
{
	int fileLength;
	FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }

	fseek(file, 0, SEEK_END);
	fileLength = ftell(file);
    fseek(file, 0xA0, SEEK_SET); /* skip the all zero header */
	while(ftell(file) < fileLength)
    {
        int stitchType = NORMAL;
        
		int b1 = (int) binaryReadByte(file);
        int b2 = (int) binaryReadByte(file);
        unsigned char commandByte = binaryReadByte(file);

        if(commandByte == 0x04)
        {
            embPattern_addStitchRel(pattern, 0, 0, END, 1);
            break;
        }

        if((commandByte & 0x01) == 0x01)
            stitchType = STOP;
        if((commandByte & 0x02) == 0x02)
            stitchType = JUMP;
        if((commandByte & 0x10) != 0x10)
            b2 = -b2;
        if((commandByte & 0x40) == 0x40)
            b1 = -b1;
        embPattern_addStitchRel(pattern, b1 / 10.0, b2 / 10.0, stitchType, 1);
    }
    return 1; /*TODO: finish readSst */
}

int writeSst(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeSst */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
