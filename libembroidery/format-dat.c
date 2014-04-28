#include "format-dat.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readDat(EmbPattern* pattern, const char* fileName)
{
    int fileLength, stitchesRemaining;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-dat.c readDat(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dat.c readDat(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-dat.c readDat(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);
    embFile_seek(file, 0x00, SEEK_END);
    fileLength = embFile_tell(file);
    embFile_seek(file, 0x02, SEEK_SET);
    stitchesRemaining = binaryReadUInt16(file);
    embFile_seek(file, 0x100, SEEK_SET);

    while(embFile_tell(file) < fileLength)
    {
        int b1 = (int)binaryReadUInt8(file);
        int b2 = (int)binaryReadUInt8(file);
        unsigned char b0 = binaryReadByte(file);

        int stitchType = NORMAL;
        stitchesRemaining--;

        if((b0 & 0x02) == 0) stitchType = TRIM;

        if(b0 == 0x87)
        {
            stitchType = STOP;
        }
        if(b0 == 0xF8)
        {
            break;
        }
        if(b1 >= 0x80)
        {
            b1 = -(b1 & 0x7F);
        }
        if(b2 >= 0x80)
        {
            b2 = -(b2 & 0x7F);
        }
        embPattern_addStitchRel(pattern, b1 / 10.0, b2 / 10.0, stitchType, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeDat(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-dat.c writeDat(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dat.c writeDat(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-dat.c writeDat(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeDat */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
