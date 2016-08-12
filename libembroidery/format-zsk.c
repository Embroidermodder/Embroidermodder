#include "format-zsk.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include "emb-file.h"
#include "emb-logging.h"

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readZsk(EmbPattern* pattern, const char* fileName)
{
    char b[3];
    EmbFile* file = 0;
    int stitchType;
    unsigned char colorNumber;
    if(!pattern) { embLog_error("format-zsk.c readZsk(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-zsk.c readZsk(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-zsk.c readZsk(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 0x230, SEEK_SET);
    colorNumber = binaryReadUInt8(file);
    while(colorNumber != 0)
    {
        EmbThread t;
        t.color.r = binaryReadUInt8(file);
        t.color.g = binaryReadUInt8(file);
        t.color.b = binaryReadUInt8(file);
        t.catalogNumber = "";
        t.description = "";
        embPattern_addThread(pattern, t);
        embFile_seek(file, 0x48, SEEK_CUR);
        colorNumber = binaryReadUInt8(file);
    }
    embFile_seek(file, 0x2E, SEEK_CUR);

    while(embFile_read(b, 1, 3, file) == 3)
    {
        stitchType = NORMAL;
		if (b[0] & 0x4)
		{
			b[2] = -b[2];
		}
		if (b[0] & 0x8)
		{
			b[1] = -b[1];
		}
		if (b[0] & 0x02)
		{
			stitchType = JUMP;
		}
        if(b[0] & 0x20)
        {
			if (b[1] == 2)
			{
				stitchType = TRIM;
			}
			else if (b[1] == -1)
			{
				break;
			}
            else
            {
				if (b[2] != 0)
				{
					colorNumber = b[2];
				}
                stitchType = STOP; /* TODO: need to determine what b[1] is used for.*/
                embPattern_changeColor(pattern, colorNumber - 1);
            }
            b[1] = 0;
            b[2] = 0;
        }
        embPattern_addStitchRel(pattern, b[1] / 10.0, b[2] / 10.0, stitchType, 0);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
	if (pattern->lastStitch->stitch.flags != END)
	{
		embPattern_addStitchRel(pattern, 0, 0, END, 1);
	}
    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeZsk(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-zsk.c writeZsk(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-zsk.c writeZsk(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-zsk.c writeZsk(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeZsk */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
