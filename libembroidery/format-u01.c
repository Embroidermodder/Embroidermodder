#include "format-u01.h"
#include "helpers-unused.h"
#include <stdio.h>

/* TODO: AFAIK this is a duplicate of U00. Review for differences and merge files and handle accordingly. */

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readU01(EmbPattern* pattern, const char* fileName)
{
    int fileLength, negativeX = 0, negativeY = 0, flags = NORMAL;
    char dx, dy;
    unsigned char data[3];
    FILE* file = fopen(fileName, "rb");
    if(!file)
    {
        return 0;
    }
    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);
    fseek(file, 0x100, SEEK_SET);
    while(fread(data, 1, 3, file) == 3)
    {
        if(data[0] == 0xF8 || data[0] == 0x87 || data[0] == 0x91)
        {
            break;
        }
        if((data[0] & 0x0F) == 0)
        {
            flags = NORMAL;
        }
        else if((data[0] & 0x1f) == 1)
        {
            flags = JUMP;
        }
        else if((data[0] & 0x0F) > 0)
        {
            flags = STOP;
        }
        negativeX = ((data[0] & 0x20) > 0);
        negativeY = ((data[0] & 0x40) > 0);

        dx = (char) data[2];
        dy = (char) data[1];
        if(negativeX) dx = (char) -dx;
        if(negativeY) dy = (char) -dy;
        embPattern_addStitchRel(pattern, dx / 10.0, dy / 10.0, flags, 1);
    }
    embPattern_addStitchRel(pattern, 0.0, 0.0, END, 1);

    fclose(file);
    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeU01(EmbPattern* pattern, const char* fileName)
{
    emb_unused(pattern); /*TODO: finish writeU01 */
    emb_unused(fileName); /*TODO: finish writeU01 */
    return 0; /*TODO: finish writeU01 */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
