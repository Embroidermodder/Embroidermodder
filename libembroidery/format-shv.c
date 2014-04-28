#include "format-shv.h"
#include "format-jef.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include <string.h>

static char shvDecode(unsigned char inputByte)
{
    if(inputByte >= 0x80)
    {
        return (char)-((unsigned char)((~inputByte) + 1));
    }
    return ((char)inputByte);
}

static short shvDecodeShort(unsigned short inputByte)
{
    if(inputByte > 0x8000)
    {
        return (short)-((unsigned short)((~inputByte) + 1));
    }
    return ((short)inputByte);
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readShv(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;
    int i;
    char inJump = 0;
    unsigned char fileNameLength, designWidth, designHeight;
    char halfDesignWidth, halfDesignHeight, halfDesignWidth2, halfDesignHeight2;
    char* headerText = "Embroidery disk created using software licensed from Viking Sewing Machines AB, Sweden";
    char dx = 0, dy = 0;
    char numberOfColors;
    unsigned short magicCode;
    int something;
    short left,top,right,bottom;
    char something2, numberOfSections, something3;

    if(!pattern) { embLog_error("format-shv.c readShv(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-shv.c readShv(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-shv.c readShv(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embFile_seek(file, strlen(headerText), SEEK_SET);
    fileNameLength = binaryReadUInt8(file);
    embFile_seek(file, fileNameLength, SEEK_CUR);
    designWidth = binaryReadUInt8(file);
    designHeight = binaryReadUInt8(file);
    halfDesignWidth = binaryReadUInt8(file);
    halfDesignHeight = binaryReadUInt8(file);
    halfDesignWidth2 = binaryReadUInt8(file);
    halfDesignHeight2 = binaryReadUInt8(file);
    if((designHeight % 2) == 1)
        embFile_seek(file, ((designHeight + 1)*designWidth)/2, SEEK_CUR);
    else
        embFile_seek(file, (designHeight*designWidth)/2, SEEK_CUR);

    numberOfColors = binaryReadUInt8(file);
    magicCode = binaryReadUInt16(file);
    binaryReadByte(file);
    something = binaryReadInt32(file);
    left = binaryReadInt16(file);
    top = binaryReadInt16(file);
    right = binaryReadInt16(file);
    bottom = binaryReadInt16(file);

    something2 = binaryReadByte(file);
    numberOfSections = binaryReadUInt8(file);
    something3 = binaryReadByte(file);

    for(i = 0; i < numberOfColors; i++)
    {
        unsigned char colorNumber;
        binaryReadUInt16(file);
        binaryReadUInt16(file);
        colorNumber = binaryReadUInt8(file);
        embPattern_addThread(pattern, jefThreads[colorNumber]);
        embFile_seek(file, 9, SEEK_CUR);
    }

    embFile_seek(file, -2, SEEK_CUR);
    for(i = 0; !embFile_eof(file); i++)
    {
        unsigned char b0, b1;
        int flags;
        if(inJump)
            flags = JUMP;
        else
            flags = NORMAL;

        b0 = binaryReadUInt8(file);
        b1 = binaryReadUInt8(file);
        if(b0 == 0 && b1 == 0 && i > 0) flags = STOP;
        if(b0 == 0x80)
        {
            if(b1 == 3)
            {
                /*embPattern_addStitchRel(pattern, 0, 0, STOP); TODO: review this */
                continue;
            }
            else if(b1 == 0x02)
            {
                inJump = 0;
                continue;
            }
            else if(b1 == 0x01)
            {
                unsigned short sx, sy;
                sx = binaryReadUInt8(file);
                sx = (unsigned short)(sx << 8 | binaryReadUInt8(file));
                sy = binaryReadUInt8(file);
                sy = (unsigned short)(sy << 8 | binaryReadUInt8(file));
                flags = TRIM;
                inJump = 1;
                embPattern_addStitchRel(pattern, shvDecodeShort(sx) / 10.0, shvDecodeShort(sy) / 10.0, flags, 1);
                continue;
            }
        }
        dx = shvDecode(b0);
        dy = shvDecode(b1);
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
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
int writeShv(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-shv.c writeShv(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-shv.c writeShv(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-shv.c writeShv(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeShv */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
