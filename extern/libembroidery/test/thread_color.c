/* Test */

#include <string.h>

#include "../src/embroidery.h"

int
main(void)
{
    unsigned int tColor = 0xFF0d6b2f;
    /*
    EmbColor c;
    c.r = 0x0D;
    c.g = 0x6B;
    c.b = 0x2F;
    */
    int tBrand = EMB_BRAND_SVG;
    int tNum = threadColorNum(tColor, tBrand);
    char tName[50];
    strncpy(tName, threadColorName(tColor, tBrand), 49);

    printf("Color : 0x%X\n"
       "Brand : %d\n"
       "Num   : %d\n"
       "Name  : %s\n\n",
        tColor,
        tBrand,
        tNum, /* Solution: 29 */
        tName); /* Solution: Dark Olive Green */
    return 0;
}

