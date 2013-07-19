#include "emb-color.h"

EmbColor embColor_fromHexStr(char* val)
{
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
