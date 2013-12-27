#include <stdio.h>
#include "format-mit.h"
#include "helpers-binary.h"
#include "helpers-unused.h"

static int decodeMitStitch(unsigned char value)
{
    if(value & 0x80) return -(value & 0x1F);
    return value;
}

int readMit(EmbPattern* pattern, const char* fileName)
{
    unsigned char data[2];
    FILE* file = 0;

    file = fopen(fileName, "rb");
    if(!file)
    {
        return 0;
    }
    /* embPattern_loadExternalColorFile(pattern, fileName); */

    while(binaryReadBytes(file, data, 2) == 2)
    {
        embPattern_addStitchRel(pattern, decodeMitStitch(data[0]) / 10.0, decodeMitStitch(data[1]) / 10.0, NORMAL, 1);
    }
    embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);
    return 1;
}

int writeMit(EmbPattern* pattern, const char* fileName)
{
    emb_unused(pattern); /*TODO: finish writeMit */
    emb_unused(fileName); /*TODO: finish writeMit */
    return 0; /*TODO: finish writeMit */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
