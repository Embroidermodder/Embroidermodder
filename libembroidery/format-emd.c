#include "format-emd.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"

static char emdDecode(unsigned char inputByte)
{
    return (inputByte >= 0x80) ? ((-~inputByte) - 1) : inputByte; /* TODO: eliminate ternary return statement */
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readEmd(EmbPattern* pattern, const char* fileName)
{
    unsigned char b0 = 0, b1 = 0;
    char dx = 0, dy = 0;
    int flags = NORMAL;
    char endOfStream = 0;
    unsigned char jemd0[6]; /* TODO: more descriptive name */
    int width, height, colors;
    int i;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-emd.c readEmd(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-emd.c readEmd(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-emd.c readEmd(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);

    binaryReadBytes(file, jemd0, 6); /* TODO: check return value */
    width = binaryReadInt16(file);
    height = binaryReadInt16(file);
    colors = binaryReadInt16(file);

    embFile_seek(file, 0x30, SEEK_SET);

    for(i = 0; !endOfStream; i++)
    {
            flags = NORMAL;
            b0 = binaryReadUInt8(file);
            b1 = binaryReadUInt8(file);

            if(b0 == 0x80)
            {
                if(b1 == 0x2A)
                {
                    embPattern_addStitchRel(pattern, 0, 0, STOP, 1);
                    continue;
                }
                else if(b1 == 0x80)
                {
                    b0 = binaryReadUInt8(file);
                    b1 = binaryReadUInt8(file);
                    flags = TRIM;
                }
                else if(b1 == 0xFD)
                {
                    embPattern_addStitchRel(pattern, 0, 0, END, 1);
                    break;
                }
                else
                {
                    continue;
                }
            }
            dx = emdDecode(b0);
            dy = emdDecode(b1);
            embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeEmd(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-emd.c writeEmd(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-emd.c writeEmd(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-emd.c writeEmd(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeEmd */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
