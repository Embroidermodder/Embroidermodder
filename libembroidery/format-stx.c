#include "format-stx.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include "helpers-unused.h"
#include "emb-logging.h"
#include <stdlib.h>
#include <string.h>

typedef struct SubDescriptor_
{
    int SomeNum;
    int SomeInt;
    int SomeOtherInt;
    char* ColorCode;
    char* ColorName;
} SubDescriptor;

typedef struct StxThread_
{
    char* ColorCode;
    char* ColorName;
    char* SectionName;
    SubDescriptor* SubDescriptors;
    EmbColor StxColor;
} StxThread;

static int stxReadThread(StxThread* thread, FILE* file)
{
    /* TODO: pointer safety */
    int j, colorNameLength, sectionNameLength;
    int somethingsomething, somethingsomething2, somethingelse, numberOfOtherDescriptors;
    char* codeNameBuff = 0;
    char* sectionNameBuff = 0;
    EmbColor col;
    unsigned char whatIsthis;

    int codeLength = binaryReadUInt8(file);
    char* codeBuff = (char*)malloc(codeLength);
    if(!codeBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for codeBuff\n"); return 0; }
    binaryReadBytes(file, (unsigned char*)codeBuff, codeLength);
    thread->ColorCode = codeBuff;
    colorNameLength = binaryReadUInt8(file);
    codeNameBuff = (char*)malloc(colorNameLength);
    if(!codeNameBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for codeNameBuff\n"); return 0; }
    binaryReadBytes(file, (unsigned char*)codeNameBuff, colorNameLength);
    thread->ColorName = codeNameBuff;

    col.r = binaryReadUInt8(file);
    col.b = binaryReadUInt8(file);
    col.g = binaryReadUInt8(file);

    whatIsthis = binaryReadUInt8(file);

    sectionNameLength = binaryReadUInt8(file);
    sectionNameBuff = (char*)malloc(sectionNameLength);
    if(!sectionNameBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for sectionNameBuff\n"); return 0; }
    binaryReadBytes(file, (unsigned char*)sectionNameBuff, sectionNameLength);
    thread->SectionName = sectionNameBuff;

    somethingsomething = binaryReadInt32(file);
    somethingsomething2 = binaryReadInt32(file);
    somethingelse = binaryReadInt32(file);
    numberOfOtherDescriptors = binaryReadInt16(file);

    thread->SubDescriptors = (SubDescriptor*)malloc(sizeof(SubDescriptor) * numberOfOtherDescriptors);
    if(!thread->SubDescriptors) return 0;
    for(j = 0; j < numberOfOtherDescriptors; j++)
    {
        SubDescriptor sd;
        char* subCodeBuff, *subColorNameBuff;
        int subcodeLength, subcolorNameLength;

        sd.SomeNum = binaryReadInt16(file);
        /* Debug.Assert(sd.SomeNum == 1); */
        sd.SomeInt = binaryReadInt32(file);
        subcodeLength = binaryReadUInt8(file);
        subCodeBuff = (char*)malloc(subcodeLength);
        if(!subCodeBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for subCodeBuff\n"); return 0; }
        binaryReadBytes(file, (unsigned char*)subCodeBuff, subcodeLength);
        sd.ColorCode = subCodeBuff;
        subcolorNameLength = binaryReadUInt8(file);
        subColorNameBuff = (char*)malloc(subcolorNameLength);
        if(!subColorNameBuff) { embLog_error("format-stx.c stxReadThread(), unable to allocate memory for subColorNameBuff\n"); return 0; }
        binaryReadBytes(file, (unsigned char*)subColorNameBuff, subcolorNameLength);
        sd.ColorName = subColorNameBuff;
        sd.SomeOtherInt = binaryReadInt32(file);
        thread->SubDescriptors[j] = sd;
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
    FILE* file = 0;

    if(!pattern) { embLog_error("format-stx.c readStx(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-stx.c readStx(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "rb");
    if(!file)
    {
        embLog_error("format-stx.c readStx(), cannot open %s for reading\n", fileName);
        return 0;
    }

    binaryReadBytes(file, headerBytes, 7);
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
    binaryReadBytes(file, gif, imageLength);
    /*Stream s2 = new MemoryStream(gif); */
    /*Image = new Bitmap(s2); */

    threadCount = binaryReadInt16(file);
    stxThreads = (StxThread*)malloc(sizeof(StxThread) * threadCount);
    if(!stxThreads) { embLog_error("format-stx.c readStx(), unable to allocate memory for stxThreads\n"); return 0; }
    for(i = 0; i < threadCount; i++)
    {
        EmbThread t;
        StxThread st;
        stxReadThread(&st, file);

        t.color.r = st.StxColor.r;
        t.color.g = st.StxColor.g;
        t.color.b = st.StxColor.b;
        t.description = st.ColorName;
        t.catalogNumber = st.ColorCode;
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

    /* br.BaseStream.Position = stitchDataOffset; */
    for(i = 1; i < stitchCount; )
    {
        char b0 = binaryReadByte(file);
        char b1 = binaryReadByte(file);
        if(b0 == -128)
        {
            switch (b1)
            {
                case 1:
                    b0 = binaryReadByte(file);
                    b1 = binaryReadByte(file);
                    /*embPattern_addStitchRel(b0, b1, STOP);*/

                    i++;
                    break;
                case 2:
                    b0 = binaryReadByte(file);
                    b1 = binaryReadByte(file);
                    embPattern_addStitchRel(pattern, b0 / 10.0, b1 / 10.0, JUMP, 1);
                    i++;
                    break;
                case -94:
                    /* NOTE: Is this a syncronize */
                    break;
                default:
                    /*Debugger.Break(); */
                    break;
            }
        }
        else
        {
            embPattern_addStitchRel(pattern, b0 / 10.0, b1 / 10.0, NORMAL, 1);
            i++;
        }
    }
    embPattern_addStitchRel(pattern, 0, 0, END, 1);
    fclose(file);
    embPattern_flipVertical(pattern);
    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeStx(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-stx.c writeStx(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-stx.c writeStx(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeStx */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
