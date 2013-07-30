#include <stdio.h>
#include "format-new.h"
#include "helpers-binary.h"

int decodeNewStitch(unsigned char value)
{
	return (int)value;
}
int readNew(EmbPattern* pattern, const char* fileName)
{
	unsigned int stitchCount;
	unsigned char data[3];
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);
	stitchCount = binaryReadUInt16(file);
    while(binaryReadBytes(file, data, 3) == 3)
    {
		int x = decodeNewStitch(data[0]);
		int y = decodeNewStitch(data[1]);
		int flag = NORMAL;
		char val = data[2];
		if(data[2] & 0x40)
		{
			x = -x;
		}
		if(data[2] & 0x20)
		{
			y = -y;
		}
		if(data[2] & 0x10)
		{
			flag = TRIM;
		}
		if(data[2] & 0x01)
		{
			flag = JUMP;
		}
		if((val & 0x1E) == 0x02)
		{
			flag = STOP;
		}
		/* Unknown values, possibly TRIM 
		155 = 1001 1011 = 0x9B
		145 = 1001 0001 = 0x91
        */
		/*val = (data[2] & 0x1C);
		if(val != 0 && data[2] != 0x9B && data[2] != 0x91)
		{
			int z = 1;
		}*/
		embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, flag, 1);
    }
	embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
    return 1;
}

int writeNew(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish writeNew */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
