#include <stdlib.h>
#include <string.h>
#include "helpers-binary.h"
#include "emb-pattern.h"
#include "helpers-misc.h"
#include "format-stx.h"

/* TODO: Dispose all the allocated memory */

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

void ReadStxThread(StxThread* thread, FILE* file)
{
    int j, colorNameLength, sectionNameLength;
    int somethingsomething, somethingsomething2, somethingelse, numberOfOtherDescriptors;
    char* codeNameBuff, *sectionNameBuff;
    EmbColor col;
    unsigned char whatIsthis;

    int codeLength = binaryReadUInt8(file);
    char* codeBuff = (char*)malloc(codeLength);
    binaryReadBytes(file, (unsigned char*)codeBuff, codeLength);
    thread->ColorCode = codeBuff;
    colorNameLength = binaryReadUInt8(file);
    codeNameBuff = (char*)malloc(colorNameLength);
    binaryReadBytes(file, (unsigned char*)codeNameBuff, colorNameLength);
    thread->ColorName = codeNameBuff;

    col.r = binaryReadUInt8(file);
    col.b = binaryReadUInt8(file);
    col.g = binaryReadUInt8(file);

    whatIsthis = binaryReadUInt8(file);

    sectionNameLength = binaryReadUInt8(file);
    sectionNameBuff = (char*)malloc(sectionNameLength);
    binaryReadBytes(file, (unsigned char*)sectionNameBuff, sectionNameLength);
    thread->SectionName = sectionNameBuff;

    somethingsomething = binaryReadInt32(file);
    somethingsomething2 = binaryReadInt32(file);
    somethingelse = binaryReadInt32(file);
    numberOfOtherDescriptors = binaryReadInt16(file);

    thread->SubDescriptors = (SubDescriptor *) malloc(sizeof(SubDescriptor) * numberOfOtherDescriptors);
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
        binaryReadBytes(file, (unsigned char*)subCodeBuff, subcodeLength);
        sd.ColorCode = subCodeBuff;
        subcolorNameLength = binaryReadUInt8(file);
        subColorNameBuff = (char*)malloc(subcolorNameLength);
        binaryReadBytes(file, (unsigned char*)subColorNameBuff, subcolorNameLength);
        sd.ColorName = subColorNameBuff;
        sd.SomeOtherInt = binaryReadInt32(file);
        thread->SubDescriptors[j] = sd;
    }
}

int readStx(EmbPattern* pattern, const char* fileName)
{
    int i, threadCount;
    unsigned char* gif;
    /* public Bitmap Image; */
    StxThread* StxThreads;
    unsigned char headerBytes[7];
    char* header;
    char filetype[4], version[5];
    int paletteLength, imageLength,something1, stitchDataOffset,something3,threadDescriptionOffset, stitchCount, left, right, colors;
    int val1, val2,val3,val4,val5,val6;

    int vala1,vala2,vala3,vala4,vala5,vala6;
    int bottom,top;
    FILE* file = fopen(fileName, "rb");
    if(file == 0)
    {
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
    binaryReadBytes(file, gif, imageLength);
    /*Stream s2 = new MemoryStream(gif); */
    /*Image = new Bitmap(s2); */

    threadCount = binaryReadInt16(file);
    StxThreads = (StxThread *) malloc(sizeof(StxThread) * threadCount);
    for(i = 0; i < threadCount; i++)
    {
        EmbThread t;
        StxThread st;
        ReadStxThread(&st, file);

        t.color.r = st.StxColor.r;
        t.color.g = st.StxColor.g;
        t.color.b = st.StxColor.b;
        t.description = st.ColorName;
        t.catalogNumber = st.ColorCode;
        embPattern_addThread(pattern, t);
        StxThreads[i] = st;
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

int writeStx(EmbPattern* pattern, const char* fileName)
{
    return 0; /*TODO: finish WriteStx */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
