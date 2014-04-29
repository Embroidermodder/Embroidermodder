#include "format-pcm.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include "helpers-misc.h"

static double pcmDecode(unsigned char a1, unsigned char a2, unsigned char a3)
{
    int res = a1 + (a2 << 8) + (a3 << 16);
    if(res > 0x7FFFFF)
    {
        return (-((~(res) & 0x7FFFFF) - 1));
    }
    return res;
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPcm(EmbPattern* pattern, const char* fileName)
{
    int i = 0;
    unsigned char b[9];
    double dx = 0, dy = 0;
    int flags = 0, st = 0;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-pcm.c readPcm(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pcm.c readPcm(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-pcm.c readPcm(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 4, SEEK_SET);

    for(i = 0; i < 16; i++)
    {
        int colorNumber;
        (void)embFile_getc(file); /* zero */
        colorNumber = embFile_getc(file);
        embPattern_addThread(pattern, pcmThreads[colorNumber]);
    }
    st = binaryReadUInt16BE(file);
    /* READ STITCH RECORDS */
    for(i = 0; i < st; i++)
    {
        flags = NORMAL;
        if(embFile_read(b, 1, 9, file) != 9)
            break;

        if(b[8] & 0x01)
        {
            flags = STOP;
        }
        else if(b[8] & 0x04)
        {
            flags = TRIM;
        }
        else if(b[8] != 0)
        {
            /* TODO: ONLY INTERESTED IN THIS CASE TO LEARN MORE ABOUT THE FORMAT */
        }
        dx = pcmDecode(b[2], b[1], b[0]);
        dy = pcmDecode(b[6], b[5], b[4]);
        embPattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePcm(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-pcm.c writePcm(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-pcm.c writePcm(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-pcm.c writePcm(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writePcm */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
