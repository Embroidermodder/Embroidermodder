#include "format-exp.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "emb-stitch.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include <stdio.h>

static char expDecode(unsigned char a1)
{
    return (a1 > 0x80) ? ((-~a1) - 1) : a1;
}

static void expEncode(unsigned char* b, char dx, char dy, int flags)
{
    if(!b)
    {
        embLog_error("format-exp.c expEncode(), b argument is null\n");
        return;
    }
    /* TODO: How to encode JUMP stitches? JUMP must be handled. Also check this for the KSM format since it appears to be similar */
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
    else if(flags == TRIM || flags == JUMP)
    {
        b[0] = 0x80;
        b[1] = 2;
        b[2] = dx;
        b[3] = dy;
    }
    else
    {
        b[0] = dx;
        b[1] = dy;
    }
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readExp(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;
    int i = 0;
    unsigned char b0 = 0, b1 = 0;
    char dx = 0, dy = 0;
    int flags = 0;

    if(!pattern) { embLog_error("format-exp.c readExp(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-exp.c readExp(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-exp.c readExp(), cannot open %s for reading\n", fileName);
        return 0;
    }
    embPattern_loadExternalColorFile(pattern, fileName);

    for(i = 0; !embFile_eof(file); i++)
    {
        flags = NORMAL;
        b0 = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
            break;
        b1 = (unsigned char)embFile_getc(file);
        if(embFile_eof(file))
            break;
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
                flags = STOP;
            }
            else if((b1 == 2) || (b1 == 4) || b1 == 6)
            {
                flags = TRIM;
                if(b1 == 2) flags = NORMAL;
                b0 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                b1 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
            }
            else if(b1 == 0x80)
            {
                b0 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                b1 = (unsigned char)embFile_getc(file);
                if(embFile_eof(file))
                    break;
                /* Seems to be b0=0x07 and b1=0x00
                 * Maybe used as extension functions */
                b0 = 0;
                b1 = 0;
                flags = TRIM;
            }
        }
        dx = expDecode(b0);
        dy = expDecode(b1);
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
int writeExp(EmbPattern* pattern, const char* fileName)
{
#ifdef ARDUINO /* ARDUINO TODO: This is temporary. Remove when complete. */
return 0; /* ARDUINO TODO: This is temporary. Remove when complete. */
#else /* ARDUINO TODO: This is temporary. Remove when complete. */

    EmbFile* file = 0;
    EmbStitchList* stitches = 0;
    double dx = 0.0, dy = 0.0;
    double xx = 0.0, yy = 0.0;
    int flags = 0;
    unsigned char b[4];

    if(!pattern) { embLog_error("format-exp.c writeExp(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-exp.c writeExp(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-exp.c writeExp(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-exp.c writeExp(), cannot open %s for writing\n", fileName);
        return 0;
    }

    /* write stitches */
    stitches = pattern->stitchList;
    while(stitches)
    {
        dx = stitches->stitch.xx * 10.0 - xx;
        dy = stitches->stitch.yy * 10.0 - yy;
        xx = stitches->stitch.xx * 10.0;
        yy = stitches->stitch.yy * 10.0;
        flags = stitches->stitch.flags;
        expEncode(b, (char)roundDouble(dx), (char)roundDouble(dy), flags);
        if((b[0] == 0x80) && ((b[1] == 1) || (b[1] == 2) || (b[1] == 4) || (b[1] == 0x10)))
        {
            embFile_printf(file, "%c%c%c%c", b[0], b[1], b[2], b[3]);
        }
        else
        {
            embFile_printf(file, "%c%c", b[0], b[1]);
        }
        stitches = stitches->next;
    }
    embFile_printf(file, "\x1a");
    embFile_close(file);
    return 1;
#endif /* ARDUINO TODO: This is temporary. Remove when complete. */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
