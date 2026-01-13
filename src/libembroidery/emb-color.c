#include "emb-color.h"
#include "emb-logging.h"
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

/* Returns a pointer to an EmbColor. It is created on the heap. The caller is responsible for freeing the allocated memory. */
EmbColor* embColor_create(unsigned char r, unsigned char g, unsigned char b)
{
    EmbColor* heapColor = (EmbColor*)malloc(sizeof(EmbColor));
    if(!heapColor) { embLog_error("emb-color.c embColor_create(), cannot allocate memory for heapColor\n"); return 0; }
    heapColor->r = r;
    heapColor->g = g;
    heapColor->b = b;
    return heapColor;
}

/* Converts a 6 digit hex string (I.E. "00FF00") into an EmbColor and returns it. */
EmbColor embColor_fromHexStr(char* val)
{
    EmbColor color;
    char r[3];
    char g[3];
    char b[3];

    r[0] = val[0];
    r[1] = val[1];
    r[2] = 0;

    g[0] = val[2];
    g[1] = val[3];
    g[2] = 0;

    b[0] = val[4];
    b[1] = val[5];
    b[2] = 0;

    color.r = (unsigned char)strtol(r, 0, 16);
    color.g = (unsigned char)strtol(g, 0, 16);
    color.b = (unsigned char)strtol(b, 0, 16);
    return color;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
