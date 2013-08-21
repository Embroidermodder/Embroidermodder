#include "emb-color.h"

/* Returns an EmbColor. It is created on the stack. */
EmbColor embColor_make(unsigned char r, unsigned char g, unsigned char b)
{
    EmbColor stackColor;
    stackColor.r = r;
    stackColor.g = g;
    stackColor.b = b;
    return stackColor;
}

EmbColor embColor_fromHexStr(char* val)
{
    /* TODO: pointer safety */

    /* expect string beginning with 6 hex digits */
    EmbColor color1;
    int rgb = 0;
    int i;
    for(i = 0; i < 6; i++)
    {
        if(val[i] >= '0' && val[i] <= '9')
        {
            rgb = rgb * 16 + val[i] - '0';
        }
        else if(val[i] >= 'A' && val[i] <= 'F')
        {
            rgb = rgb * 16 + val[i] - 'A';
        }
        else if(val[i] >= 'a' && val[i] <= 'f')
        {
            rgb = rgb * 16 + val[i] - 'a';
        }
    }
    color1.r = (rgb >> 16) & 0xFF;
    color1.g = (rgb >> 8) & 0xFF;
    color1.b = rgb & 0xFF;
    return (color1);
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
