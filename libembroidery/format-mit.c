#include "format-mit.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"

static int mitDecodeStitch(unsigned char value)
{
	if (value & 0x80)
	{
		return -(value & 0x1F);
	}
    return value;
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readMit(EmbPattern* pattern, const char* fileName)
{
    unsigned char data[2];
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-mit.c readMit(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-mit.c readMit(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-mit.c readMit(), cannot open %s for reading\n", fileName);
        return 0;
    }

    /* embPattern_loadExternalColorFile(pattern, fileName); TODO: review this and uncomment or remove it */

    while(binaryReadBytes(file, data, 2) == 2)
    {
        embPattern_addStitchRel(pattern, mitDecodeStitch(data[0]) / 10.0, mitDecodeStitch(data[1]) / 10.0, NORMAL, 1);
    }

    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

static unsigned char mitEncodeStitch(double value)
{
	if (value < 0)
	{
		return 0x80 | (unsigned char)(-value);
	}
	return (unsigned char)value;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeMit(EmbPattern* pattern, const char* fileName)
{
	EmbFile* file = 0;
	EmbStitchList* pointer = 0;
	double xx = 0, yy = 0, dx = 0, dy = 0;
	int flags = 0;

    if(!pattern) { embLog_error("format-mit.c writeMit(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-mit.c writeMit(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-mit.c writeMit(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
	if (pattern->lastStitch->stitch.flags != END)
	{
		embPattern_addStitchRel(pattern, 0, 0, END, 1);
	}
	file = embFile_open(fileName, "wb");
	if (!file)
	{
		embLog_error("format-mit.c writeMit(), cannot open %s for writing\n", fileName);
		return 0;
	}
	embPattern_correctForMaxStitchLength(pattern, 0x1F, 0x1F);
	xx = yy = 0;
	pointer = pattern->stitchList;
	while (pointer)
	{
		dx = pointer->stitch.xx - xx;
		dy = pointer->stitch.yy - yy;
		xx = pointer->stitch.xx;
		yy = pointer->stitch.yy;
		flags = pointer->stitch.flags;
		embFile_putc(mitEncodeStitch(dx), file);
		embFile_putc(mitEncodeStitch(dy), file);
		pointer = pointer->next;
	}
	embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
