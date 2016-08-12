/* .DST (Tajima) embroidery file read/write routines
 * Format comments are thanks to tspilman@dalcoathletic.com who's
 * notes appeared at http://www.wotsit.org under Tajima Format.
 */

#include "format-dst.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

static int decode_record_flags(unsigned char b2)
{
    int returnCode = 0;
    if(b2 == 0xF3)
    {
        return END;
    }
    if(b2 & 0x80)
    {
        returnCode |= JUMP;
    }
    if(b2 & 0x40)
    {
        returnCode |= STOP;
    }
    return returnCode;
}

static unsigned char setbit(int pos)
{
    return (unsigned char)(1 << pos);
}

/* TODO: review this then remove since emb-pattern.c has a similar function */
/* void combineJumpStitches(EmbPattern* p, int jumpsPerTrim)
{
    if(!p) { embLog_error("format-dst.c combineJumpStitches(), p argument is null\n"); return; }
    EmbStitchList* pointer = p->stitchList;
    int jumpCount = 0;
    EmbStitchList* jumpListStart = 0;
    char needleDown = 0;
    while(pointer)
    {
        if((pointer->stitch.flags & JUMP) && !(pointer->stitch.flags & STOP))
        {
            if(jumpCount == 0)
            {
                jumpListStart = pointer;
            }
            jumpCount++;
            if(needleDown && jumpCount >= jumpsPerTrim)
            {
                EmbStitchList* removePointer = jumpListStart->next;
                jumpListStart->stitch.xx = pointer->stitch.xx;
                jumpListStart->stitch.yy = pointer->stitch.yy;
                jumpListStart->stitch.flags |= TRIM;
                jumpListStart->next = pointer;

                jumpCount-=2;
                for(; jumpCount > 0; jumpCount--)
                {
                    EmbStitchList* tempPointer = removePointer->next;
                    jumpListStart->stitch.flags |= removePointer->stitch.flags;
                    free(removePointer);
                    removePointer = 0;
                    removePointer = tempPointer;
                }
                jumpCount = 0;
                needleDown = 0;
            }
        }
        else
        {
            if(pointer->stitch.flags == NORMAL)
            {
                needleDown = 1;
                jumpCount = 0;
            }
        }
        pointer = pointer->next;
    }
}
*/

static void encode_record(EmbFile* file, int x, int y, int flags)
{
    char b0, b1, b2;
    b0 = b1 = b2 = 0;

    /* cannot encode values > +121 or < -121. */
    if(x > 121 || x < -121) embLog_error("format-dst.c encode_record(), x is not in valid range [-121,121] , x = %d\n", x);
    if(y > 121 || y < -121) embLog_error("format-dst.c encode_record(), y is not in valid range [-121,121] , y = %d\n", y);

    if(x >= +41) { b2 += setbit(2); x -= 81; }
    if(x <= -41) { b2 += setbit(3); x += 81; }
    if(x >= +14) { b1 += setbit(2); x -= 27; }
    if(x <= -14) { b1 += setbit(3); x += 27; }
    if(x >=  +5) { b0 += setbit(2); x -= 9; }
    if(x <=  -5) { b0 += setbit(3); x += 9; }
    if(x >=  +2) { b1 += setbit(0); x -= 3; }
    if(x <=  -2) { b1 += setbit(1); x += 3; }
    if(x >=  +1) { b0 += setbit(0); x -= 1; }
    if(x <=  -1) { b0 += setbit(1); x += 1; }
    if(x !=   0) { embLog_error("format-dst.c encode_record(), x should be zero yet x = %d\n", x); }
    if(y >= +41) { b2 += setbit(5); y -= 81; }
    if(y <= -41) { b2 += setbit(4); y += 81; }
    if(y >= +14) { b1 += setbit(5); y -= 27; }
    if(y <= -14) { b1 += setbit(4); y += 27; }
    if(y >=  +5) { b0 += setbit(5); y -= 9; }
    if(y <=  -5) { b0 += setbit(4); y += 9; }
    if(y >=  +2) { b1 += setbit(7); y -= 3; }
    if(y <=  -2) { b1 += setbit(6); y += 3; }
    if(y >=  +1) { b0 += setbit(7); y -= 1; }
    if(y <=  -1) { b0 += setbit(6); y += 1; }
    if(y !=   0) { embLog_error("format-dst.c encode_record(), y should be zero yet y = %d\n", y); }

    b2 |= (char) 3;

    if(flags & END)
    {
        b2 = (char) -13;
        b0 = b1 = (char) 0;
    }

    /* if(flags & TRIM)
    {
        int v = 5;
        int dx = (int)(x/v), dy = (int)(y/v);
        for(i = 1; i < v; i++)
        {
            encode_record(file, dx, dy, JUMP);
        }
        encode_record(file, x - (dx * (v - 1)), y - (dy * (v - 1)), JUMP);
        return;
    } */
    if(flags & (JUMP | TRIM))
    {
        b2 = (char) (b2 | 0x83);
    }
    if(flags & STOP)
    {
        b2 = (char) (b2 | 0xC3);
    }

    binaryWriteByte(file, (unsigned char)b0);
    binaryWriteByte(file, (unsigned char)b1);
    binaryWriteByte(file, (unsigned char)b2);
}

/*convert 2 characters into 1 int for case statement */
/*#define cci(s) (s[0]*256+s[1]) */
#define cci(c1,c2) (c1*256+c2)

static void set_dst_variable(EmbPattern* pattern, char* var, char* val)
{
    unsigned int i;
    EmbThread t;

    for(i = 0; i <= (unsigned int)strlen(var); i++)
    {
        /* uppercase the var */
        if(var[i] >= 'a' && var[i] <= 'z')
        {
            var[i] += 'A' - 'a';
        }
    }

    /* macro converts 2 characters to 1 int, allows case statement... */
    switch(cci(var[0],var[1]))
    {
    case cci('L','A'): /* Design Name (LA) */
        /*pattern->set_variable("Design_Name",val); TODO: review this line. */
        break;
    case cci('S','T'): /* Stitch count, 7 digits padded by leading 0's */
    case cci('C','O'): /* Color change count, 3 digits padded by leading 0's */
    case cci('+','X'): /* Design extents (+/-X,+/-Y), 5 digits padded by leading 0's */
    case cci('-','X'):
    case cci('+','Y'):
    case cci('-','Y'):
        /* don't store these variables, they are recalculated at save */
        break;
    case cci('A','X'): /* Relative coordinates of last point, 6 digits, padded with leading spaces, first char may be +/- */
    case cci('A','Y'):
    case cci('M','X'): /* Coordinates of last point in previous file of multi-volume design, 6 digits, padded with leading spaces, first char may be +/- */
    case cci('M','Y'):
        /* store these variables as-is, they will be converted to numbers and back at save; */
        /*pattern->set_variable(var,val); TODO: review this line. */
        break;
    case cci('P','D'):
        /* store this string as-is, it will be saved as-is, 6 characters */
        if(strlen(val) != 6)
        {
            /*pattern->messages.add("Warning: in DST file read, PD is not 6 characters, but ",(int)strlen(val)); */
        }
        /*pattern->set_variable(var,val);*/
        break;
        /* Begin extended fields section */
    case cci('A','U'): /* Author string, arbitrary length */
    case cci('C','P'): /* Copyright string, arbitrary length */
        /*pattern->set_variable(var,val); TODO: review this line. */
        break;
    case cci('T','C'): /*Thread Color: #RRGGBB,Description,Catalog Number (1st field RGB hex values, 2nd&3rd fields optional arbitrary length) */
        /* TODO: review these lines below.
        description=split_cell_str(val,2);
        catalog_number=split_cell_str(val,3);
        */
        t.color = embColor_fromHexStr(val);
        t.description = "";
        t.catalogNumber = "";
        embPattern_addThread(pattern, t);
        break;
    default:
        /* unknown field, just save it. */
        /*pattern->set_variable(var,val); TODO: review this line. */
        break;
    }
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readDst(EmbPattern* pattern, const char* fileName)
{
    char var[3];   /* temporary storage variable name */
    char val[512]; /* temporary storage variable value */
    int valpos;
    unsigned char b[3];
    char header[512 + 1];
    EmbFile* file = 0;
    int i = 0;
    int flags; /* for converting stitches from file encoding */

    /*
    * The header seems to contain information about the design.
    * Seems to be ASCII text delimited by 0x0D (carriage returns).
    * This must be in the file for most new software or hardware
    * to consider it a good file! This is much more important
    * than I originally believed. The header is 125 bytes in
    * length and padded out by 0x20 to 512 bytes total.
    * All entries in the header seem to be 2 ASCII characters
    * followed by a colon, then it's value trailed by a carriage return.
    *
    * char LA[16+1];  First is the 'LA' entry, which is the design name with no
    *                 path or extension information. The blank is 16 characters
    *                 in total, but the name must not be longer that 8 characters
    *                 and padded out with 0x20.
    *
    * char ST[7+1];   Next is the stitch count ST, this is a 7 digit number
    *                 padded by leading zeros. This is the total stitch count
    *                 including color changes, jumps, nups, and special records.
    *
    * char CO[3+1];   Next, is CO or colors, a 3 digit number padded by leading
    *                 zeros. This is the number of color change records in the file.
    *
    * char POSX[5+1]; Next is +X or the positive X extent in centimeters, a 5
    *                 digit non-decimal number padded by leading zeros.
    *
    * char NEGX[5+1]; Following is the -X or the negative X extent in millimeters,
    *                 a 5 digit non-decimal number padded by leading zeros.
    *
    * char POSY[5+1]; Again, the +Y extents.
    *
    * char NEGY[5+1]; Again, the -Y extents.
    *
    * char AX[6+1];   AX and AY should express the relative coordinates of the
    * char AY[6+1];   last point from the start point in 0.1 mm. If the start
    *                 and last points are the same, the coordinates are (0,0).
    *
    * char MX[6+1];   MX and MY should express coordinates of the last point of
    * char MY[6+1];   the previous file for a multi-volume design. A multi-
    *                 volume design means a design consisted of two or more files.
    *                 This was used for huge designs that can not be stored in a
    *                 single paper tape roll. It is not used so much (almost
    *                 never) nowadays.
    *
    * char PD[9+1];   PD is also storing some information for multi-volume design.
    */

    /* TODO: review commented code below
    pattern->clear();
    pattern->set_variable("file_name",filename);
    */

    if(!pattern) { embLog_error("format-dst.c readDst(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dst.c readDst(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-dst.c readDst(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);
    /* READ 512 BYTE HEADER INTO header[] */
    for(i = 0; i < 512; i++)
    {
        header[i] = (char)embFile_getc(file);
    }

    /*TODO:It would probably be a good idea to validate file before accepting it. */

    /* fill variables from header fields */
    for(i = 0; i < 512; i++)
    {
        if(header[i] == ':' && i > 1)
        {
            var[0] = header[i - 2];
            var[1] = header[i - 1];
            var[2] = '\0';
            valpos = i + 1;
            for(i++; i < 512; i++)
            {
                /* don't accept : without CR because there's a bug below: i-valpos must be > 0 which is not the case if the : is before the third character. */
                if(header[i] == 13/*||header[i]==':'*/) /* 0x0d = carriage return */
                {
                    if(header[i] == ':') /* : indicates another variable, CR was missing! */
                    {
                        i -= 2;
                    }
                    strncpy(val, &header[valpos], (size_t)(i - valpos));
                    val[i - valpos] = '\0';
                    set_dst_variable(pattern, var, val);
                    break;
                }
            }
        }
    }

    while(embFile_read(b, 1, 3, file) == 3)
    {
        int x = 0;
        int y = 0;
        if(b[0] & 0x01)
            x += 1;
        if(b[0] & 0x02)
            x -= 1;
        if(b[0] & 0x04)
            x += 9;
        if(b[0] & 0x08)
            x -= 9;
        if(b[0] & 0x80)
            y += 1;
        if(b[0] & 0x40)
            y -= 1;
        if(b[0] & 0x20)
            y += 9;
        if(b[0] & 0x10)
            y -= 9;
        if(b[1] & 0x01)
            x += 3;
        if(b[1] & 0x02)
            x -= 3;
        if(b[1] & 0x04)
            x += 27;
        if(b[1] & 0x08)
            x -= 27;
        if(b[1] & 0x80)
            y += 3;
        if(b[1] & 0x40)
            y -= 3;
        if(b[1] & 0x20)
            y += 27;
        if(b[1] & 0x10)
            y -= 27;
        if(b[2] & 0x04)
            x += 81;
        if(b[2] & 0x08)
            x -= 81;
        if(b[2] & 0x20)
            y += 81;
        if(b[2] & 0x10)
            y -= 81;
        flags = decode_record_flags(b[2]);
        if(flags == END)
        {
            break;
        }
        embPattern_addStitchRel(pattern, x / 10.0, y / 10.0, flags, 1);
    }
    embFile_close(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* combineJumpStitches(pattern, 5); */
    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeDst(EmbPattern* pattern, const char* fileName)
{
    EmbRect boundingRect;
    EmbFile* file = 0;
    int xx, yy, dx, dy, flags;
    int i;
    int co = 1, st = 0;
    int ax, ay, mx, my;
    char* pd = 0;
    EmbStitchList* pointer = 0;

    if(!pattern) { embLog_error("format-dst.c writeDst(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dst.c writeDst(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-dst.c writeDst(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    file = embFile_open(fileName, "wb");
    if(!file)
    {
        embLog_error("format-dst.c writeDst(), cannot open %s for writing\n", fileName);
        return 0;
    }

    embPattern_correctForMaxStitchLength(pattern, 12.1, 12.1);

    xx = yy = 0;
    co = 1;
    co = embThreadList_count(pattern->threadList);
    st = 0;
    st = embStitchList_count(pattern->stitchList);
    flags = NORMAL;
    boundingRect = embPattern_calcBoundingBox(pattern);
    /* TODO: review the code below
    if(pattern->get_variable("design_name") != NULL)
    {
    char *la = stralloccopy(pattern->get_variable("design_name"));
    if(strlen(la)>16) la[16]='\0';

    embFile_printf(file,"LA:%-16s\x0d",la);
    free(la);
    }
    else
    {
    */
    embFile_printf(file, "LA:%-16s\x0d", "Untitled");
    /*} */
    embFile_printf(file, "ST:%7d\x0d", st);
    embFile_printf(file, "CO:%3d\x0d", co - 1); /* number of color changes, not number of colors! */
    embFile_printf(file, "+X:%5d\x0d", (int)(boundingRect.right * 10.0));
    embFile_printf(file, "-X:%5d\x0d", (int)(fabs(boundingRect.left) * 10.0));
    embFile_printf(file, "+Y:%5d\x0d", (int)(boundingRect.bottom * 10.0));
    embFile_printf(file, "-Y:%5d\x0d", (int)(fabs(boundingRect.top) * 10.0));


    ax = ay = mx = my = 0;
    /* TODO: review the code below */
    /*ax=pattern->get_variable_int("ax"); */ /* will return 0 if not defined */
    /*ay=pattern->get_variable_int("ay"); */
    /*mx=pattern->get_variable_int("mx"); */
    /*my=pattern->get_variable_int("my"); */

    /*pd=pattern->get_variable("pd");*/ /* will return null pointer if not defined */
    pd = 0;
    if(pd == 0 || strlen(pd) != 6)
    {
        /* pd is not valid, so fill in a default consisting of "******" */
        pd = "******";
    }
    embFile_printf(file, "AX:+%5d\x0d", ax);
    embFile_printf(file, "AY:+%5d\x0d", ay);
    embFile_printf(file, "MX:+%5d\x0d", mx);
    embFile_printf(file, "MY:+%5d\x0d", my);
    embFile_printf(file, "PD:%6s\x0d", pd);
    binaryWriteByte(file, 0x1a); /* 0x1a is the code for end of section. */

    /* pad out header to proper length */
    for(i = 125; i < 512; i++)
    {
        embFile_printf(file, " ");
    }

    /* write stitches */
    xx = yy = 0;
    pointer = pattern->stitchList;
    while(pointer)
    {
        /* convert from mm to 0.1mm for file format */
        dx = roundDouble(pointer->stitch.xx * 10.0) - xx;
        dy = roundDouble(pointer->stitch.yy * 10.0) - yy;
        xx = roundDouble(pointer->stitch.xx * 10.0);
        yy = roundDouble(pointer->stitch.yy * 10.0);
        flags = pointer->stitch.flags;
        encode_record(file, dx, dy, flags);
        pointer = pointer->next;
    }
    binaryWriteByte(file, 0xA1); /* finish file with a terminator character */
    binaryWriteShort(file, 0);
    embFile_close(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
