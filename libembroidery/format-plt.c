#include "format-plt.h"
#include "emb-logging.h"
#include "helpers-binary.h"
#include "helpers-misc.h"
#include <stdio.h>
#include <string.h>

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readPlt(EmbPattern* pattern, const char* fileName)
{
    double x, y;
    double scalingFactor = 40;
    char input[512];
    FILE* file = 0;

    if(!pattern) { embLog_error("format-plt.c readPlt(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-plt.c readPlt(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "rb");
    if(!file)
    {
        embLog_error("format-plt.c readPlt(), cannot open %s for reading\n", fileName);
        return 0;
    }

    embPattern_loadExternalColorFile(pattern, fileName);
    /* TODO: replace all scanf code */
    while(fscanf(file, "%s", input) >= 0)
    {
        if(startsWith("PD", input))
        {
            /* TODO: replace all scanf code */
            if(sscanf(input, "PD%lf,%lf;", &x, &y) < 2)
            {
                break;
            }
            embPattern_addStitchAbs(pattern, x / scalingFactor, y / scalingFactor, NORMAL, 1);
        }
        else if(startsWith("PU", input))
        {
            /* TODO: replace all scanf code */
            if(sscanf(input, "PU%lf,%lf;", &x, &y) < 2)
            {
                break;
            }
            embPattern_addStitchAbs(pattern, x / scalingFactor, y / scalingFactor, STOP, 1);
        }
    }
    fclose(file);

    /* Check for an END stitch and add one if it is not present */
    if(pattern->lastStitch->stitch.flags != END)
        embPattern_addStitchRel(pattern, 0, 0, END, 1);

    return 1;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writePlt(EmbPattern* pattern, const char* fileName)
{
    /* TODO: pointer safety */
    double scalingFactor = 40;
    EmbStitch stitch;
    EmbStitchList* pointer = 0;
    char firstStitchOfBlock = 1;
    FILE* file = 0;

    if(!pattern) { embLog_error("format-plt.c writePlt(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-plt.c writePlt(), fileName argument is null\n"); return 0; }

    file = fopen(fileName, "wb");
    if(!file)
    {
        embLog_error("format-plt.c writePlt(), cannot open %s for writing\n", fileName);
        return 0;
    }

    fprintf(file, "IN;");
    fprintf(file, "ND;");

    pointer = pattern->stitchList;
    while(pointer)
    {
        stitch = pointer->stitch;
        if(stitch.flags & STOP)
        {
            firstStitchOfBlock = 1;
        }
        if(firstStitchOfBlock)
        {
            fprintf(file, "PU%f,%f;", stitch.xx * scalingFactor, stitch.yy * scalingFactor);
            fprintf(file, "ST0.00,0.00;");
            fprintf(file, "SP0;");
            fprintf(file, "HT0;");
            fprintf(file, "HS0;");
            fprintf(file, "TT0;");
            fprintf(file, "TS0;");
            firstStitchOfBlock = 0;
        }
        else
        {
            fprintf(file, "PD%f,%f;", stitch.xx * scalingFactor, stitch.yy * scalingFactor);
        }

        pointer = pointer->next;
    }
    fprintf(file, "PU0.0,0.0;");
    fprintf(file, "PU0.0,0.0;");
    fclose(file);
    return 1; /*TODO: finish WritePlt */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */

