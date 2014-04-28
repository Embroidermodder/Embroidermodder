#include "format-new.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"

static int decodeNewStitch(unsigned char value)
{
    return (int)value;
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readNew(EmbPattern* pattern, const char* fileName)
{
    unsigned int stitchCount;
    unsigned char data[3];
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-new.c readNew(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-new.c readNew(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-new.c readNew(), cannot open %s for reading\n", fileName);
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

    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeNew(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-new.c writeNew(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-new.c writeNew(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-new.c writeNew(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeNew */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
