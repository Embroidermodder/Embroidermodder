#include "embroidery.h"
#include <stdio.h>

#define HOOP_110X110 0
#define HOOP_50X50   1
#define HOOP_140X200 2
#define HOOP_126X110 3
#define HOOP_200X200 4

const EmbThread jefThreads[] = {
    {{0, 0 ,0}, "Black", ""},
    {{0, 0, 0}, "Black", ""},
    {{255, 255, 255}, "White", ""},
    {{255, 255, 23}, "Yellow", ""},
    {{250, 160, 96}, "Orange", ""},
    {{92, 118, 73}, "Olive Green", ""},
    {{64, 192, 48}, "Green", ""},
    {{101, 194, 200}, "Sky", ""},
    {{172, 128, 190}, "Purple", ""},
    {{245, 188, 203}, "Pink", ""},
    {{255, 0, 0}, "Red", ""},
    {{192, 128, 0}, "Brown", ""},
    {{0, 0, 240}, "Blue", ""},
    {{228, 195, 93}, "Gold", ""},
    {{165, 42, 42}, "Dark Brown", ""},
    {{213, 176, 212}, "Pale Violet", ""},
    {{252, 242, 148}, "Pale Yellow", ""},
    {{240, 208, 192}, "Pale Pink", ""},
    {{255, 192, 0}, "Peach", ""},
    {{201, 164, 128}, "Beige", ""},
    {{155, 61, 75}, "Wine Red", ""},
    {{160, 184, 204}, "Pale Sky", ""},
    {{127, 194, 28}, "Yellow Green", ""},
    {{185, 185, 185}, "Silver Grey", ""},
    {{160, 160, 160}, "Grey", ""},
    {{152, 214, 189}, "Pale Aqua", ""},
    {{184, 240, 240}, "Baby Blue", ""},
    {{54, 139, 160}, "Powder Blue", ""},
    {{79, 131, 171}, "Bright Blue", ""},
    {{56, 106, 145}, "Slate Blue", ""},
    {{0, 32, 107}, "Nave Blue", ""},
    {{229, 197, 202}, "Salmon Pink", ""},
    {{249, 103, 107}, "Coral", ""},
    {{227, 49, 31}, "Burnt Orange", ""},
    {{226, 161, 136}, "Cinnamon", ""},
    {{181, 148, 116}, "Umber", ""},
    {{228, 207, 153}, "Blonde", ""},
    {{225, 203, 0}, "Sunflower", ""},
    {{225, 173, 212}, "Orchid Pink", ""},
    {{195, 0, 126}, "Peony Purple", ""},
    {{128, 0, 75}, "Burgundy", ""},
    {{160, 96, 176}, "Royal Purple", ""},
    {{192, 64, 32}, "Cardinal Red", ""},
    {{202, 224, 192}, "Opal Green", ""},
    {{137, 152, 86}, "Moss Green", ""},
    {{0, 170, 0}, "Meadow Green", ""},
    {{33, 138, 33}, "Dark Green", ""},
    {{93, 174, 148}, "Aquamarine", ""},
    {{76, 191, 143}, "Emerald Green", ""},
    {{0, 119, 114}, "Peacock Green", ""},
    {{112, 112, 112}, "Dark Grey", ""},
    {{242, 255, 255}, "Ivory White", ""},
    {{177, 88, 24}, "Hazel", ""},
    {{203, 138, 7}, "Toast", ""},
    {{247, 146, 123}, "Salmon", ""},
    {{152, 105, 45}, "Cocoa Brown", ""},
    {{162, 113, 72}, "Sienna", ""},
    {{123, 85, 74}, "Sepia", ""},
    {{79, 57, 70}, "Dark Sepia", ""},
    {{82, 58, 151}, "Violet Blue", ""},
    {{0, 0, 160}, "Blue Ink", ""},
    {{0, 150, 222}, "Solar Blue", ""},
    {{178, 221, 83}, "Green Dust", ""},
    {{250, 143, 187}, "Crimson", ""},
    {{222, 100, 158}, "Floral Pink", ""},
    {{181, 80, 102}, "Wine", ""},
    {{94, 87, 71}, "Olive Drab", ""},
    {{76, 136, 31}, "Meadow", ""},
    {{228, 220, 121}, "Mustard", ""},
    {{203, 138, 26}, "Yellow Ochre", ""},
    {{198, 170, 66}, "Old Gold", ""},
    {{236, 176, 44}, "Honeydew", ""},
    {{248, 128, 64}, "Tangerine", ""},
    {{255, 229, 5}, "Canary Yellow", ""},
    {{250, 122, 122}, "Vermillion", ""},
    {{107, 224, 0}, "Bright Green", ""},
    {{56, 108, 174}, "Ocean Blue", ""},
    {{227, 196, 180}, "Beige Grey", ""},
    {{227, 172, 129}, "Bamboo", ""}};

static int jefGetHoopSize(int width, int height)
{
    if(width <  500 && height <  500) { return HOOP_50X50; }
    if(width < 1260 && height < 1100) { return HOOP_126X110; }
    if(width < 1400 && height < 2000) { return HOOP_140X200; }
	if(width < 2000 && height < 2000) { return HOOP_200X200; }
    return ((int) HOOP_110X110);
}

static char jefDecode(unsigned char inputByte)
{
    if(inputByte >= 0x80)
        return (char) ((-~inputByte) - 1);
    return ((char) inputByte);
}

static void jefSetHoopFromId(EmbPattern* pattern, int hoopCode)
{
    if(!pattern) { embLog_error("format-jef.c jefSetHoopFromId(), pattern argument is null\n"); return; }

    switch(hoopCode)
    {
        case HOOP_126X110:
            pattern->hoop.height = 126.0;
            pattern->hoop.width = 110.0;
            break;
        case HOOP_50X50:
            pattern->hoop.height = 50.0;
            pattern->hoop.width = 50.0;
            break;
        case HOOP_110X110:
           pattern->hoop.height = 110.0;
            pattern->hoop.width = 110.0;
            break;
        case HOOP_140X200:
            pattern->hoop.height = 140.0;
            pattern->hoop.width = 200.0;
            break;
        case HOOP_200X200:
            pattern->hoop.height = 200.0;
            pattern->hoop.width = 200.0;
            break;
    }
}

struct hoop_padding
{
    int left;
    int right;
    int top;
    int bottom;
};

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readJef(EmbPattern* pattern, const char* fileName)
{
    int stitchOffset, formatFlags, numberOfColors, numberOfStitchBytes;
    int hoopSize, i;
    struct hoop_padding bounds, rectFrom110x110, rectFrom50x50, rectFrom200x140, rect_from_custom;
    int stitchBytes;
    char date[8], time[8];
  
    EmbFile* file = 0;
 
    unsigned char b0 = 0, b1 = 0;
    char dx = 0, dy = 0;
    int flags = 0;


    if(!pattern) { embLog_error("format-jef.c readJef(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-jef.c readJef(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-jef.c readJef(), cannot open %s for reading\n", fileName);
        return 0;
    }

    stitchOffset = binaryReadInt32(file);
    formatFlags = binaryReadInt32(file); /* TODO: find out what this means */

    binaryReadBytes(file, (unsigned char*) date, 8); /* TODO: check return value */
    binaryReadBytes(file, (unsigned char*) time, 8); /* TODO: check return value */
    numberOfColors = binaryReadInt32(file);
    numberOfStitchBytes = binaryReadInt32(file) * 2;
    hoopSize = binaryReadInt32(file);
    jefSetHoopFromId(pattern, hoopSize);

    bounds.left = binaryReadInt32(file);
    bounds.top = binaryReadInt32(file);
    bounds.right = binaryReadInt32(file);
    bounds.bottom = binaryReadInt32(file);

    rectFrom110x110.left = binaryReadInt32(file);
    rectFrom110x110.top = binaryReadInt32(file);
    rectFrom110x110.right = binaryReadInt32(file);
    rectFrom110x110.bottom = binaryReadInt32(file);

    rectFrom50x50.left = binaryReadInt32(file);
    rectFrom50x50.top = binaryReadInt32(file);
    rectFrom50x50.right = binaryReadInt32(file);
    rectFrom50x50.bottom = binaryReadInt32(file);

    rectFrom200x140.left = binaryReadInt32(file);
    rectFrom200x140.top = binaryReadInt32(file);
    rectFrom200x140.right = binaryReadInt32(file);
    rectFrom200x140.bottom = binaryReadInt32(file);

    rect_from_custom.left = binaryReadInt32(file);
    rect_from_custom.top = binaryReadInt32(file);
    rect_from_custom.right = binaryReadInt32(file);
    rect_from_custom.bottom = binaryReadInt32(file);

    for(i = 0; i < numberOfColors; i++)
    {
        embPattern_addThread(pattern, jefThreads[binaryReadInt32(file) % 79]);
    }
    embFile_seek(file, stitchOffset, SEEK_SET);
	stitchBytes = 0;
    while(stitchBytes < numberOfStitchBytes)
    {
        flags = NORMAL;
        b0 = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
        {
            break;
        }
        b1 = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
        {
            break;
        }
		stitchBytes += 2;
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
				stitchBytes += 2;
                flags = STOP;
            }
            else if((b1 == 2) || (b1 == 4) || b1 == 6)
            {
                flags = TRIM;
                b0 = (unsigned char)embFile_getc(file);
                if (embFile_eof(file))
                {
                    break;
                }
                b1 = (unsigned char)embFile_getc(file);
                if (embFile_eof(file))
                {
                    break;
                }
				stitchBytes += 2;
            }
            else if(b1 == 0x10)
            {
                embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
				stitchBytes += 1;
                break;
            }
        }
        dx = jefDecode(b0);
        dy = jefDecode(b1);
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if (pattern->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchRel(pattern, 0, 0, END, 1);
    }
    return 1;
}

static void jefEncode(unsigned char* b, char dx, char dy, int flags)
{
    if(!b)
    {
        embLog_error("format-jef.c expEncode(), b argument is null\n");
        return;
    }
    if(flags == STOP)
    {
        b[0] = 0x80;
        b[1] = 1;
        b[2] = dx;
        b[3] = dy;
    }
    else if (flags == END)
    {
        b[0] = 0x80;
        b[1] = 0x10;
        b[2] = 0;
        b[3] = 0;
    }
    else if(flags == TRIM)
    {
        b[0] = 0x80;
        b[1] = 2;
        b[2] = dx;
        b[3] = dy;
    }
	else if (flags == JUMP)
	{
		b[0] = 0x80;
		b[1] = 4;
		b[2] = dx;
		b[3] = dy;
	}
    else
    {
        b[0] = dx;
        b[1] = dy;
    }
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeJef(EmbPattern* pattern, const char* fileName)
{
    int colorlistSize, designWidth, designHeight, i, jumpAndStopCount;
    EmbRect boundingRect;
    EmbFile* file = 0;
    EmbTime time;
    EmbThreadList* threadPointer = 0;
    EmbStitchList* stitches = 0;
    double dx = 0.0, dy = 0.0;
    double xx = 0.0, yy = 0.0;
    int flags = 0;
    unsigned char b[4];

    if(!pattern) { embLog_error("format-jef.c writeJef(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-jef.c writeJef(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-jef.c writeJef(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if (pattern->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchRel(pattern, 0, 0, END, 1);
    }
    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-jef.c writeJef(), cannot open %s for writing\n", fileName);
        return 0;
    }

    embPattern_correctForMaxStitchLength(pattern, 12.7, 12.7);

    colorlistSize = embThreadList_count(pattern->threadList);
    binaryWriteInt(file, 0x74 + (colorlistSize * 8));
    binaryWriteInt(file, 0x14);

    embTime_initNow(&time);

    embFile_printf(file, "%04d%02d%02d%02d%02d%02d", (int)(time.year + 1900),
            (int)(time.month + 1), (int)(time.day), (int)(time.hour),
            (int)(time.minute), (int)(time.second));
    binaryWriteByte(file, 0x00);
    binaryWriteByte(file, 0x00);
    binaryWriteInt(file, embThreadList_count(pattern->threadList));

	stitches = pattern->stitchList;
	jumpAndStopCount = 0;
	while (stitches)
	{
		flags = stitches->stitch.flags;
		if ((flags & (STOP | TRIM | JUMP)) > 0) {
			jumpAndStopCount++;
		}
		stitches = stitches->next;
	}
	binaryWriteInt(file, embStitchList_count(pattern->stitchList) + jumpAndStopCount);

    boundingRect = embPattern_calcBoundingBox(pattern);

    designWidth = (int)(embRect_width(boundingRect) * 10.0);
    designHeight = (int)(embRect_height(boundingRect) * 10.0);

    binaryWriteInt(file, jefGetHoopSize(designWidth, designHeight));

    /* Distance from center of Hoop */
    binaryWriteInt(file, (int) (designWidth / 2));  /* left */
    binaryWriteInt(file, (int) (designHeight / 2)); /* top */
    binaryWriteInt(file, (int) (designWidth / 2));  /* right */
    binaryWriteInt(file, (int) (designHeight / 2)); /* bottom */

    /* Distance from default 110 x 110 Hoop */
    if(min(550 - designWidth / 2, 550 - designHeight / 2) >= 0)
    {
        binaryWriteInt(file, (int) max(-1, (1100 - designWidth) / 2));  /* left */
		binaryWriteInt(file, (int)max(-1, (1100 - designWidth) / 2));  /* right */
        binaryWriteInt(file, (int) max(-1, (1100 - designHeight) / 2)); /* top */
        binaryWriteInt(file, (int) max(-1, (1100 - designHeight) / 2)); /* bottom */
    }
    else
    {
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
    }

    /* Distance from default 50 x 50 Hoop */
    if(min((500 - designWidth) / 2, (500 - designHeight) / 2) >= 0)
    {
        binaryWriteInt(file, (int) max(-1, (500 - designWidth) / 2));  /* left */
		binaryWriteInt(file, (int)max(-1, (500 - designWidth) / 2));  /* right */
        binaryWriteInt(file, (int) max(-1, (500 - designHeight) / 2)); /* top */
        binaryWriteInt(file, (int) max(-1, (500 - designHeight) / 2)); /* bottom */
    }
    else
    {
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
        binaryWriteInt(file, -1);
    }

    /* Distance from default 140 x 200 Hoop */
    binaryWriteInt(file, (int)max(-1, ((1400 - designWidth) / 2)));   /* left */
    binaryWriteInt(file, (int)max(-1, ((2000 - designHeight) / 2))); /* top */
	binaryWriteInt(file, (int)max(-1, ((1400 - designWidth) / 2)));   /* right */
    binaryWriteInt(file, (int)max(-1, ((2000 - designHeight) / 2))); /* bottom */

    /* repeated Distance from default 140 x 200 Hoop */
    /* TODO: Actually should be distance to custom hoop */
    binaryWriteInt(file, (int)max(-1, ((1400 - designWidth) / 2)));  /* left */
    binaryWriteInt(file, (int)max(-1, ((2000 - designHeight) / 2))); /* top */
	binaryWriteInt(file, (int)max(-1, ((1400 - designWidth) / 2)));  /* right */
    binaryWriteInt(file, (int)max(-1, ((2000 - designHeight) / 2))); /* bottom */

    threadPointer = pattern->threadList;

    while(threadPointer)
    {
        binaryWriteInt(file, embThread_findNearestColorInArray(threadPointer->thread.color, (EmbThread*)jefThreads, 79));
        threadPointer = threadPointer->next;
    }
    for(i = 0; i < colorlistSize; i++)
    {
        binaryWriteInt(file, 0x0D);
    }
    stitches = pattern->stitchList;
    while(stitches)
    {
        dx = stitches->stitch.xx * 10.0 - xx;
        dy = stitches->stitch.yy * 10.0 - yy;
        xx = stitches->stitch.xx * 10.0;
        yy = stitches->stitch.yy * 10.0;
        flags = stitches->stitch.flags;
        jefEncode(b, (char)roundDouble(dx), (char)roundDouble(dy), flags);
        if((b[0] == 0x80) && ((b[1] == 1) || (b[1] == 2) || (b[1] == 4)))
        {
            embFile_printf(file, "%c%c%c%c", b[0], b[1], b[2], b[3]);
        }
        else
        {
            embFile_printf(file, "%c%c", b[0], b[1]);
        }
		if (flags & END) {
			break;
		}
        stitches = stitches->next;
    }
    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
