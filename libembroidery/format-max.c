#include "format-max.h"
#include "format-pcd.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include "helpers-misc.h"

/* Pfaff MAX embroidery file format */

static double maxDecode(unsigned char a1, unsigned char a2, unsigned char a3)
{
    int res = a1 + (a2 << 8) + (a3 << 16);
    if(res > 0x7FFFFF)
    {
        return (-((~(res) & 0x7FFFFF) - 1));
    }
    return res;
}

static void maxEncode(EmbFile* file, int x, int y)
{
    if(!file) { embLog_error("format-max.c maxEncode(), file argument is null\n"); return; }

    binaryWriteByte(file, (unsigned char)0);
    binaryWriteByte(file, (unsigned char)(x & 0xFF));
    binaryWriteByte(file, (unsigned char)((x >> 8) & 0xFF));
    binaryWriteByte(file, (unsigned char)((x >> 16) & 0xFF));

    binaryWriteByte(file, (unsigned char)0);
    binaryWriteByte(file, (unsigned char)(y & 0xFF));
    binaryWriteByte(file, (unsigned char)((y >> 8) & 0xFF));
    binaryWriteByte(file, (unsigned char)((y >> 16) & 0xFF));
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readMax(EmbPattern* pattern, const char* fileName)
{
    int i = 0;
    unsigned char b[8];
    double dx = 0, dy = 0;
    int flags = 0;
    int stitchCount;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-max.c readMax(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-max.c readMax(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-max.c readMax(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, 0xD5, SEEK_SET);
    stitchCount = binaryReadUInt32(file);

    /* READ STITCH RECORDS */
    for(i = 0; i < stitchCount; i++)
    {
        flags = NORMAL;
        if(embFile_read(b, 1, 8, file) != 8)
            break;

        dx = maxDecode(b[0], b[1], b[2]);
        dy = maxDecode(b[4], b[5], b[6]);
        embPattern_addStitchAbs(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    embPattern_flipVertical(pattern);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeMax(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;
    EmbStitchList* pointer = 0;
    char header[] = {
        0x56,0x43,0x53,0x4D,0xFC,0x03,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
        0xF6,0x25,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x05,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x31,0x33,0x37,0x38,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4D,0x61,0x64,0x65,0x69,0x72,0x61,0x20,
        0x52,0x61,0x79,0x6F,0x6E,0x20,0x34,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x01,0x38,0x09,0x31,0x33,0x30,0x2F,0x37,0x30,0x35,0x20,0x48,0xFA,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00 };

    if(!pattern) { embLog_error("format-max.c writeMax(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-max.c writeMax(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-max.c writeMax(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-max.c writeMax(), cannot open %s for writing\n", fileName);
        return 0;
    }

    binaryWriteBytes(file, header, 0xD5);
    pointer = pattern->stitchList;
    while(pointer)
    {
        maxEncode(file, roundDouble(pointer->stitch.xx * 10.0), roundDouble(pointer->stitch.yy * 10.0));
        pointer = pointer->next;
    }
    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
