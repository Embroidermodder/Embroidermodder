#include "format-csd.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include "emb-file.h"
#include "emb-logging.h"
#include <stdlib.h>

#define CsdSubMaskSize  479
#define CsdXorMaskSize  501

static char _subMask[CsdSubMaskSize];
static char _xorMask[CsdXorMaskSize];

static void BuildDecryptionTable(int seed)
{
    int i;
    const int mul1 = 0x41C64E6D;
    const int add1 = 0x3039;

    for(i = 0; i < CsdSubMaskSize; i++)
    {
        seed *= mul1;
        seed += add1;
        _subMask[i] = (char) ((seed >> 16) & 0xFF);
    }
    for(i = 0; i < CsdXorMaskSize; i++)
    {
        seed *= mul1;
        seed += add1;
        _xorMask[i] = (char) ((seed >> 16) & 0xFF);
    }
}

static unsigned char DecodeCsdByte(long fileOffset, unsigned char val, int type)
{
    static const unsigned char _decryptArray[] =
    {
          0x43, 0x6E, 0x72, 0x7A, 0x76, 0x6C, 0x61, 0x6F, 0x7C, 0x29, 0x5D, 0x62, 0x60, 0x6E, 0x61, 0x62, 0x20
        , 0x41, 0x66, 0x6A, 0x3A, 0x35, 0x5A, 0x63, 0x7C, 0x37, 0x3A, 0x2A, 0x25, 0x24, 0x2A, 0x33, 0x00, 0x10
        , 0x14, 0x03, 0x72, 0x4C, 0x48, 0x42, 0x08, 0x7A, 0x5E, 0x0B, 0x6F, 0x45, 0x47, 0x5F, 0x40, 0x54, 0x5C
        , 0x57, 0x55, 0x59, 0x53, 0x3A, 0x32, 0x6F, 0x53, 0x54, 0x50, 0x5C, 0x4A, 0x56, 0x2F, 0x2F, 0x62, 0x2C
        , 0x22, 0x65, 0x25, 0x28, 0x38, 0x30, 0x38, 0x22, 0x2B, 0x25, 0x3A, 0x6F, 0x27, 0x38, 0x3E, 0x3F, 0x74
        , 0x37, 0x33, 0x77, 0x2E, 0x30, 0x3D, 0x34, 0x2E, 0x32, 0x2B, 0x2C, 0x0C, 0x18, 0x42, 0x13, 0x16, 0x0A
        , 0x15, 0x02, 0x0B, 0x1C, 0x1E, 0x0E, 0x08, 0x60, 0x64, 0x0D, 0x09, 0x51, 0x25, 0x1A, 0x18, 0x16, 0x19
        , 0x1A, 0x58, 0x10, 0x14, 0x5B, 0x08, 0x15, 0x1B, 0x5F, 0xD5, 0xD2, 0xAE, 0xA3, 0xC1, 0xF0, 0xF4, 0xE8
        , 0xF8, 0xEC, 0xA6, 0xAB, 0xCD, 0xF8, 0xFD, 0xFB, 0xE2, 0xF0, 0xFE, 0xFA, 0xF5, 0xB5, 0xF7, 0xF9, 0xFC
        , 0xB9, 0xF5, 0xEF, 0xF4, 0xF8, 0xEC, 0xBF, 0xC3, 0xCE, 0xD7, 0xCD, 0xD0, 0xD7, 0xCF, 0xC2, 0xDB, 0xA4
        , 0xA0, 0xB0, 0xAF, 0xBE, 0x98, 0xE2, 0xC2, 0x91, 0xE5, 0xDC, 0xDA, 0xD2, 0x96, 0xC4, 0x98, 0xF8, 0xC9
        , 0xD2, 0xDD, 0xD3, 0x9E, 0xDE, 0xAE, 0xA5, 0xE2, 0x8C, 0xB6, 0xAC, 0xA3, 0xA9, 0xBC, 0xA8, 0xA6, 0xEB
        , 0x8B, 0xBF, 0xA1, 0xAC, 0xB5, 0xA3, 0xBB, 0xB6, 0xA7, 0xD8, 0xDC, 0x9A, 0xAA, 0xF9, 0x82, 0xFB, 0x9D
        , 0xB9, 0xAB, 0xB3, 0x94, 0xC1, 0xA0, 0x8C, 0x8B, 0x8E, 0x95, 0x8F, 0x87, 0x99, 0xE7, 0xE1, 0xA3, 0x83
        , 0x8B, 0xCF, 0xA3, 0x85, 0x9D, 0x83, 0xD4, 0xB7, 0x83, 0x84, 0x91, 0x97, 0x9F, 0x88, 0x8F, 0xDD, 0xAD
        , 0x90
    };
    int newOffset;

    fileOffset = fileOffset - 1;
    if(type != 0)
    {
        int final;
        int fileOffsetHigh = (int) (fileOffset & 0xFFFFFF00);
        int fileOffsetLow = (int) (fileOffset & 0xFF);

        newOffset = fileOffsetLow;
        fileOffsetLow = fileOffsetHigh;
        final = fileOffsetLow%0x300;
        if(final != 0x100 && final != 0x200)
        {
            newOffset = _decryptArray[newOffset] | fileOffsetHigh;
        }
        else if(final != 0x100 && final == 0x200)
        {
            if(newOffset == 0)
            {
                fileOffsetHigh = fileOffsetHigh - 0x100;
            }
            newOffset = _decryptArray[newOffset] | fileOffsetHigh;
        }
        else if(newOffset != 1 && newOffset != 0)
        {
            newOffset = _decryptArray[newOffset] | fileOffsetHigh;
        }
        else
        {
            fileOffsetHigh = fileOffsetHigh - 0x100;
            newOffset = _decryptArray[newOffset] | fileOffsetHigh;
        }
    }
    else
    {
        newOffset = (int) fileOffset;
    }
    return ((unsigned char) ((unsigned char) (val ^ _xorMask[newOffset%CsdXorMaskSize]) - _subMask[newOffset%CsdSubMaskSize]));
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readCsd(EmbPattern* pattern, const char* fileName)
{
    int i, type = 0;
    unsigned char identifier[8];
    unsigned char unknown1, unknown2;
    char dx = 0, dy = 0;
    int colorChange = -1;
    int flags;
    char endOfStream = 0;
    EmbFile* file = 0;
    unsigned char colorOrder[14];

    if(!pattern) { embLog_error("format-csd.c readCsd(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-csd.c readCsd(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-csd.c readCsd(), cannot open %s for reading\n", fileName);
        return 0;
    }

    binaryReadBytes(file, identifier, 8); /* TODO: check return value */

    if(identifier[0] != 0x7C && identifier[2] != 0xC3)
    {
        type = 1;
    }
    if(type == 0)
    {
        BuildDecryptionTable(0xC);
    }
    else
    {
        BuildDecryptionTable(identifier[0]);
    }
    embFile_seek(file, 8, SEEK_SET);
    for(i = 0; i < 16; i++)
    {
        EmbThread thread;
        thread.color.r = DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);
        thread.color.g = DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);
        thread.color.b = DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);
        thread.catalogNumber = "";
        thread.description = "";
        embPattern_addThread(pattern, thread);
    }
    unknown1 = DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);
    unknown2 = DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);

    for(i = 0; i < 14; i++)
    {
        colorOrder[i] = (unsigned char) DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);
    }
    for(i = 0; !endOfStream; i++)
    {
        char negativeX, negativeY;
        unsigned char b0 = DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);
        unsigned char b1 = DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);
        unsigned char b2 = DecodeCsdByte(embFile_tell(file), binaryReadByte(file), type);

        if(b0 == 0xF8 || b0 == 0x87 || b0 == 0x91)
        {
            break;
        }
        negativeX = ((b0 & 0x20) > 0);
        negativeY = ((b0 & 0x40) > 0);
        b0 = (unsigned char)(b0 & (0xFF ^ 0xE0));

        if((b0 & 0x1F) == 0) flags = NORMAL;
        else if((b0 & 0x0C) > 0)
        {
            flags = STOP;
            if(colorChange >= 14)
            {
                printf("Invalid color change detected\n");
            }
            embPattern_changeColor(pattern, colorOrder[colorChange  % 14]);
            colorChange += 1;
        }
        else if((b0 & 0x1F) > 0) flags = TRIM;
        else flags = NORMAL;
        dx = (char) b2;
        dy = (char) b1;
        if(negativeX) dx = (char) -dx;
        if(negativeY) dy = (char) -dy;
        if(flags == STOP) embPattern_addStitchRel(pattern, 0, 0, flags, 1);
        else embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeCsd(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-csd.c writeCsd(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-csd.c writeCsd(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-csd.c writeCsd(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeCsd */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
