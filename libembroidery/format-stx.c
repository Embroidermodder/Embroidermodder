#include "format-stx.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include "helpers-unused.h"
#include "emb-file.h"
#include "emb-logging.h"
#include <stdlib.h>
#include <string.h>

typedef struct SubDescriptor_
{
    int someNum;      /* TODO: better variable naming */
    int someInt;      /* TODO: better variable naming */
    int someOtherInt; /* TODO: better variable naming */
    char* colorCode;
    char* colorName;
} SubDescriptor;

typedef struct StxThread_
{
    char* colorCode;
    char* colorName;
    char* sectionName;
    SubDescriptor* subDescriptors;
    EmbColor stxColor;
} StxThread;

static int stxReadThread(StxThread* thread, EmbFile* file)
{
    int j, colorNameLength, sectionNameLength;
    int somethingSomething, somethingSomething2, somethingElse, numberOfOtherDescriptors; /* TODO: determine what these represent */
    int codeLength = 0;
    char* codeBuff = 0;
    char* codeNameBuff = 0;
    char* sectionNameBuff = 0;
    EmbColor col;
    unsigned char whatIsthis; /* TODO: determine what this represents */

    if(!thread) { embLog_error("format-stx.c stxReadThread(), thread argument is null\n"); return 0; }
    if(!file) { embLog_error("format-stx.c stxReadThread(), file argument is null\n"); return 0; }

    codeLength = binaryReadUInt8(file);
    codeBuff = (char*)malloc(codeLength);
    if(!codeBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for codeBuff\n"); return 0; }
    binaryReadBytes(file, (unsigned char*)codeBuff, codeLength); /* TODO: check return value */
    thread->colorCode = codeBuff;
    colorNameLength = binaryReadUInt8(file);
    codeNameBuff = (char*)malloc(colorNameLength);
    if(!codeNameBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for codeNameBuff\n"); return 0; }
    binaryReadBytes(file, (unsigned char*)codeNameBuff, colorNameLength); /* TODO: check return value */
    thread->colorName = codeNameBuff;

    col.r = binaryReadUInt8(file);
    col.b = binaryReadUInt8(file);
    col.g = binaryReadUInt8(file);

    whatIsthis = binaryReadUInt8(file);

    sectionNameLength = binaryReadUInt8(file);
    sectionNameBuff = (char*)malloc(sectionNameLength);
    if(!sectionNameBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for sectionNameBuff\n"); return 0; }
    binaryReadBytes(file, (unsigned char*)sectionNameBuff, sectionNameLength); /* TODO: check return value */
    thread->sectionName = sectionNameBuff;

    somethingSomething = binaryReadInt32(file);
    somethingSomething2 = binaryReadInt32(file);
    somethingElse = binaryReadInt32(file);
    numberOfOtherDescriptors = binaryReadInt16(file);

    thread->subDescriptors = (SubDescriptor*)malloc(sizeof(SubDescriptor) * numberOfOtherDescriptors);
    if(!thread->subDescriptors) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for thread->subDescriptors\n"); return 0; }
    for(j = 0; j < numberOfOtherDescriptors; j++)
    {
        SubDescriptor sd;
        char* subCodeBuff, *subColorNameBuff;
        int subCodeLength, subColorNameLength;

        sd.someNum = binaryReadInt16(file);
        /* Debug.Assert(sd.someNum == 1); TODO: review */
        sd.someInt = binaryReadInt32(file);
        subCodeLength = binaryReadUInt8(file);
        subCodeBuff = (char*)malloc(subCodeLength);
        if(!subCodeBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for subCodeBuff\n"); return 0; }
        binaryReadBytes(file, (unsigned char*)subCodeBuff, subCodeLength); /* TODO: check return value */
        sd.colorCode = subCodeBuff;
        subColorNameLength = binaryReadUInt8(file);
        subColorNameBuff = (char*)malloc(subColorNameLength);
        if(!subColorNameBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for subColorNameBuff\n"); return 0; }
        binaryReadBytes(file, (unsigned char*)subColorNameBuff, subColorNameLength); /* TODO: check return value */
        sd.colorName = subColorNameBuff;
        sd.someOtherInt = binaryReadInt32(file);
        thread->subDescriptors[j] = sd;
    }
    return 1;
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readStx(EmbPattern* pattern, const char* fileName)
{
    int i, threadCount;
    unsigned char* gif = 0;
    /* public Bitmap Image; */
    StxThread* stxThreads = 0;
    unsigned char headerBytes[7];
    char* header = 0;
    char filetype[4], version[5];
    int paletteLength, imageLength, something1, stitchDataOffset, something3, threadDescriptionOffset, stitchCount, left, right, colors;
    int val1, val2, val3, val4, val5, val6;

    int vala1, vala2, vala3, vala4, vala5, vala6;
    int bottom,top;
    EmbFile* file = 0;

    if(!pattern) { embLog_error("format-stx.c readStx(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-stx.c readStx(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "rb");
    if(!file)
    {
        embLog_error("format-stx.c readStx(), cannot open %s for reading\n", fileName);
        return 0;
    }

    binaryReadBytes(file, headerBytes, 7); /* TODO: check return value */
    header = (char*)headerBytes;

    memcpy(filetype, &header[0], 3);
    memcpy(version, &header[3], 4);
    filetype[3] = '\0';
    version[4] = '\0';
    binaryReadByte(file);
    paletteLength = binaryReadInt32(file);
    imageLength = binaryReadInt32(file);
    something1 = binaryReadInt32(file);
    stitchDataOffset = binaryReadInt32(file);
    something3 = binaryReadInt32(file);
    threadDescriptionOffset = binaryReadInt32(file);
    stitchCount = binaryReadInt32(file);
    colors = binaryReadInt32(file);
    right = binaryReadInt16(file);
    left = binaryReadInt16(file);
    bottom = binaryReadInt16(file);
    top = binaryReadInt16(file);

    gif = (unsigned char*)malloc(imageLength);
    if(!gif) { embLog_error("format-stx.c readStx(), unable to allocate memory for gif\n"); return 0; }
    binaryReadBytes(file, gif, imageLength); /* TODO: check return value */
    /*Stream s2 = new MemoryStream(gif); TODO: review */
    /*Image = new Bitmap(s2); TODO: review */

    threadCount = binaryReadInt16(file);
    stxThreads = (StxThread*)malloc(sizeof(StxThread) * threadCount);
    if(!stxThreads) { embLog_error("format-stx.c readStx(), unable to allocate memory for stxThreads\n"); return 0; }
    for(i = 0; i < threadCount; i++)
    {
        EmbThread t;
        StxThread st;
        stxReadThread(&st, file);

        t.color.r = st.stxColor.r;
        t.color.g = st.stxColor.g;
        t.color.b = st.stxColor.b;
        t.description = st.colorName;
        t.catalogNumber = st.colorCode;
        embPattern_addThread(pattern, t);
        stxThreads[i] = st;
    }

    binaryReadInt32(file);
    binaryReadInt32(file);
    binaryReadInt32(file);
    binaryReadInt16(file);
    binaryReadUInt8(file);

    val1 = binaryReadInt16(file);
    val2 = binaryReadInt16(file);
    val3 = binaryReadInt16(file);
    val4 = binaryReadInt16(file);

    val5 = binaryReadInt16(file); /* 0 */
    val6 = binaryReadInt16(file); /* 0 */

    vala1 = binaryReadInt16(file);
    vala2 = binaryReadInt16(file);
    vala3 = binaryReadInt16(file);
    vala4 = binaryReadInt16(file);
    vala5 = binaryReadInt16(file); /* 0 */
    vala6 = binaryReadInt16(file); /* 0 */

    binaryReadInt32(file); /* 0 */
    binaryReadInt32(file); /* 0 */

    /* br.BaseStream.Position = stitchDataOffset; TODO: review */
    for(i = 1; i < stitchCount; )
    {
        char b0 = binaryReadByte(file);
        char b1 = binaryReadByte(file);
        if(b0 == -128)
        {
            switch(b1)
            {
                case 1:
                    b0 = binaryReadByte(file);
                    b1 = binaryReadByte(file);
                    /*embPattern_addStitchRel(b0, b1, STOP); TODO: review */

                    i++;
                    break;
                case 2:
                    b0 = binaryReadByte(file);
                    b1 = binaryReadByte(file);
                    embPattern_addStitchRel(pattern, b0 / 10.0, b1 / 10.0, JUMP, 1);
                    i++;
                    break;
                case -94:
                    /* TODO: Is this a syncronize? If so document it in the comments. */
                    break;
                default:
                    /*Debugger.Break(); TODO: review */
                    break;
            }
        }
        else
        {
            embPattern_addStitchRel(pattern, b0 / 10.0, b1 / 10.0, NORMAL, 1);
            i++;
        }
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
int writeStx(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-stx.c writeStx(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-stx.c writeStx(), fileName argument is null\n"); return 0; }

    if(!embStitchList_count(pattern->stitchList))
    {
        embLog_error("format-stx.c writeStx(), pattern contains no stitches\n");
        return 0;
    }

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    /* TODO: embFile_open() needs to occur here after the check for no stitches */

    return 0; /*TODO: finish writeStx */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
