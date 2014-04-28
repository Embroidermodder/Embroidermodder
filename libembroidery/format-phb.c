#include "format-phb.h"
#include "format-pec.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPhb(EmbPattern* pattern, const char* fileName)
{
    unsigned int fileOffset;
    short colorCount;
    EmbFile* file = 0;
    int i;

    if(!pattern) { embLog_error("format-phb.c readPhb(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-phb.c readPhb(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-phb.c readPhb(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 0x71, SEEK_SET);
    colorCount = binaryReadInt16(file);

    for(i = 0; i < colorCount; i++)
    {
        EmbThread t = pecThreads[(int)binaryReadByte(file)];
        embPattern_addThread(pattern, t);
    }

    /* TODO: check that file begins with #PHB */
    embFile_seek(file, 0x54, SEEK_SET);
    fileOffset = 0x52;
    fileOffset += binaryReadUInt32(file);

    embFile_seek(file, fileOffset, SEEK_SET);
    fileOffset += binaryReadUInt32(file) + 2;

    embFile_seek(file, fileOffset, SEEK_SET);
    fileOffset += binaryReadUInt32(file);

    embFile_seek(file, fileOffset + 14, SEEK_SET); /* 28 */

    colorCount = (short)binaryReadByte(file);
    for(i = 0; i< colorCount; i++)
    {
        binaryReadByte(file);
    }
    binaryReadInt32(file); /* bytes to end of file */
    binaryReadInt32(file);
    binaryReadByte(file);

    binaryReadInt16(file);
    binaryReadInt16(file);
    binaryReadInt16(file);
    binaryReadInt16(file);
    binaryReadInt16(file);
    binaryReadInt16(file);
    readPecStitches(pattern, file);
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    embPattern_flipVertical(pattern);
    return 1; /*TODO: finish ReadPhb */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePhb(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-phb.c writePhb(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-phb.c writePhb(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-phb.c writePhb(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writePhb */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
