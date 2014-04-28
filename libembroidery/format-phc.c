#include "format-phc.h"
#include "format-pec.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPhc(EmbPattern* pattern, const char* fileName)
{
    int colorChanges, version, bytesInSection2;
    unsigned short pecOffset, bytesInSection, bytesInSection3;
    char pecAdd;
    EmbFile* file = 0;
    int i;

    if(!pattern) { embLog_error("format-phc.c readPhc(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-phc.c readPhc(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-phc.c readPhc(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 0x07, SEEK_SET);
    version = binaryReadByte(file) - 0x30; /* converting from ansi number */
    embFile_seek(file, 0x4D, SEEK_SET);
    colorChanges = binaryReadUInt16(file);

    for(i = 0; i < colorChanges; i++)
    {
        EmbThread t = pecThreads[(int)binaryReadByte(file)];
        embPattern_addThread(pattern, t);
    }
    embFile_seek(file, 0x2B, SEEK_SET);
    pecAdd = binaryReadByte(file);
    binaryReadUInt32(file); /* file length */
    pecOffset = binaryReadUInt16(file);
    embFile_seek(file, pecOffset + pecAdd, SEEK_SET);
    bytesInSection = binaryReadUInt16(file);
    embFile_seek(file, bytesInSection, SEEK_CUR);
    bytesInSection2 = binaryReadUInt32(file);
    embFile_seek(file, bytesInSection2, SEEK_CUR);
    bytesInSection3 = binaryReadUInt16(file);
    embFile_seek(file, bytesInSection3 + 0x12, SEEK_CUR);

    readPecStitches(pattern, file);
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    embPattern_flipVertical(pattern);
    return 1; /*TODO: finish ReadPhc */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePhc(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-phc.c writePhc(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-phc.c writePhc(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-phc.c writePhc(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writePhc */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
