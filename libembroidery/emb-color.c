#include "emb-color.h"
#include <stdlib.h>

/* Returns an EmbColor. It is created on the stack. */
EmbColor embColor_make(unsigned char r, unsigned char g, unsigned char b)
{
    EmbColor stackColor;
    stackColor.r = r;
    stackColor.g = g;
    stackColor.b = b;
    return stackColor;
}

/* Converts a 6 digit hex string (I.E. "00FF00") into an EmbColor and returns it. */
EmbColor embColor_fromHexStr(char* val)
{
    EmbColor color;
    char r[3] = { val[0], val[1], 0 };
    char g[3] = { val[2], val[3], 0 };
    char b[3] = { val[4], val[5], 0 };
    color.r = (unsigned char)strtol(r, 0, 16);
    color.g = (unsigned char)strtol(g, 0, 16);
    color.b = (unsigned char)strtol(b, 0, 16);
    return color;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
