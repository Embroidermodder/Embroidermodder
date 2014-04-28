#include "format-sst.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readSst(EmbPattern* pattern, const char* fileName)
{
    int fileLength;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-sst.c readSst(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-sst.c readSst(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-sst.c readSst(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);
    embFile_seek(file, 0, SEEK_END);
    fileLength = embFile_tell(file);
    embFile_seek(file, 0xA0, SEEK_SET); /* skip the all zero header */
    while(embFile_tell(file) < fileLength)
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

    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1; /*TODO: finish readSst */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeSst(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-sst.c writeSst(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-sst.c writeSst(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-sst.c writeSst(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeSst */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
