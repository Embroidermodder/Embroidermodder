#include "format-csv.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* csvStitchFlagToStr(int flags)
{
    switch(flags)
    {
    case NORMAL:
        return "STITCH";
        break;
    case JUMP:
        return "JUMP";
        break;
    case TRIM:
        return "TRIM";
        break;
    case STOP:
        return "COLOR";
        break;
    case END:
        return "END";
        break;
    default:
        return "UNKNOWN";
        break;
    }
}

int csvStrToStitchFlag(const char* str)
{
    if(!strcmp(str, "STITCH"))
        return NORMAL;
    else if(!strcmp(str, "JUMP"))
        return JUMP;
    else if(!strcmp(str, "TRIM"))
        return TRIM;
    else if(!strcmp(str, "COLOR"))
        return STOP;
    else if(!strcmp(str, "END"))
        return END;
    else if(!strcmp(str, "UNKNOWN"))
        return -1;
    return -1;
}

int readCsv(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    int numColorChanges = 0;
    int size = 1024;
    int pos = 0;
    int c = 0;
    int cellNum = 0;
    int process = 0;
    int csvMode = CSV_MODE_NULL;
    int expect = CSV_EXPECT_QUOTE1;
    int flags = 0;
    double xx = 0.0;
    double yy = 0.0;
    unsigned char r = 0, g = 0, b = 0;

    char* buff = (char*)malloc(size);
    if(!buff) { /* TODO: error */ return 0; }

    file = fopen(fileName,"r");
    if(file == 0)
    {
        /* TODO: error */
        return 0;
    }
    else
    {
        pos = 0;
        do
        {
            c = fgetc(file);
            switch(c)
            {
                case '"':
                    if(expect == CSV_EXPECT_QUOTE1)
                    {
                        expect = CSV_EXPECT_QUOTE2;
                    }
                    else if(expect == CSV_EXPECT_QUOTE2)
                        expect = CSV_EXPECT_COMMA;
                    break;
                case ',':
                    if(expect == CSV_EXPECT_COMMA)
                    {
                        process = 1;
                    }
                    break;
                case '\n':
                    if(expect == CSV_EXPECT_COMMA)
                    {
                        process = 1;
                    }
                    else if(expect == CSV_EXPECT_QUOTE1)
                    {
                        /* Do Nothing. We encountered a blank line. */
                    }
                    else
                    {
                        /* TODO: error. premature newline */
                        return 0;
                    }
                    break;
            }
            if(pos >= size - 1)
            {
                size *= 2;
                buff = (char*)realloc(buff,size);
                if(!buff) { /*TODO: error */ return 0; }
            }

            if(process)
            {
                buff[pos] = 0;
                pos = 0;
                process = 0;
                cellNum++;
                expect = CSV_EXPECT_QUOTE1;
                if(csvMode == CSV_MODE_NULL)
                {
                    if     (!strcmp(buff, "#")) { csvMode = CSV_MODE_COMMENT; }
                    else if(!strcmp(buff, ">")) { csvMode = CSV_MODE_VARIABLE; }
                    else if(!strcmp(buff, "$")) { csvMode = CSV_MODE_THREAD; }
                    else if(!strcmp(buff, "*")) { csvMode = CSV_MODE_STITCH; }
                    else { /* TODO: error */ return 0; }
                }
                else if(csvMode == CSV_MODE_COMMENT)
                {
                    /* Do Nothing */
                }
                else if(csvMode == CSV_MODE_VARIABLE)
                {
                    /* Do Nothing */
                }
                else if(csvMode == CSV_MODE_THREAD)
                {
                    if(cellNum == 2)
                    {
                        /* Do Nothing. Ignore Thread Number */
                    }
                    else if(cellNum == 3)
                        r = atoi(buff);
                    else if(cellNum == 4)
                        g = atoi(buff);
                    else if(cellNum == 5)
                        b = atoi(buff);
                    else if(cellNum == 6)
                    {
                        /* TODO: Thread Description */
                    }
                    else if(cellNum == 7)
                    {
                        /* TODO: Thread Catalog Number */
                        EmbThread t;
                        t.color.r = r;
                        t.color.g = g;
                        t.color.b = b;
                        t.description = "TODO:DESCRIPTION";
                        t.catalogNumber = "TODO:CATALOG_NUMBER";
                        embPattern_addThread(pattern, t);
                        csvMode = CSV_MODE_NULL;
                        cellNum = 0;
                    }
                    else
                    {
                        /* TODO: error */
                        return 0;
                    }
                }
                else if(csvMode == CSV_MODE_STITCH)
                {
                    if(cellNum == 2)
                    {
                        flags = csvStrToStitchFlag(buff);
                        if(flags == STOP)
                            numColorChanges++;
                    }
                    else if(cellNum == 3)
                        xx = atof(buff);
                    else if(cellNum == 4)
                    {
                        yy = atof(buff);
                        embPattern_addStitchAbs(pattern, xx, yy, flags, 1);
                        csvMode = CSV_MODE_NULL;
                        cellNum = 0;
                    }
                    else
                    {
                        /* TODO: error */
                        return 0;
                    }
                }

                if(c == '\n')
                {
                    csvMode = CSV_MODE_NULL;
                    cellNum = 0;
                }
            }
            else
            {
                if(expect == CSV_EXPECT_QUOTE2 && c != '"')
                    buff[pos++] = (char)c;
            }
        }
        while(c != EOF);
        fclose(file);
    }

    /* if not enough colors defined, fill in random colors */
    while(embThread_count(pattern->threadList) < numColorChanges)
    {
        embPattern_addThread(pattern, embThread_getRandom());
    }

    free(buff);

    return 1;
}

int writeCsv(EmbPattern* pattern, const char* fileName)
{
    FILE* file;
    EmbStitchList* sList;
    EmbThreadList* tList;
    EmbRect boundingRect;
    int i = 0;
    int stitchCount = 0;
    int threadCount = 0;

    sList = pattern->stitchList;
    stitchCount = embStitch_count(sList);

    tList = pattern->threadList;
    threadCount = embThread_count(tList);

    boundingRect = embPattern_calcBoundingBox(pattern);

    if(stitchCount == 0)
    {
        fprintf(stderr,"No file to save\n");
        return 0;
    }

    file = fopen(fileName, "w");
    if(file == 0)
    {
        fprintf(stderr, "Error opening %s for write\n", fileName);
        return 0;
    }

    /* write header */
    fprintf(file, "\"#\",\"Embroidermodder 2 CSV Embroidery File\"\n");
    fprintf(file, "\"#\",\"http://embroidermodder.sourceforge.net\"\n");
    fprintf(file, "\n");
    fprintf(file, "\"#\",\"General Notes:\"\n");
    fprintf(file, "\"#\",\"This file can be read by Excel or LibreOffice as CSV (Comma Separated Value) or with a text editor.\"\n");
    fprintf(file, "\"#\",\"Lines beginning with # are comments.\"\n");
    fprintf(file, "\"#\",\"Lines beginning with > are variables: [VAR_NAME], [VAR_VALUE]\"\n");
    fprintf(file, "\"#\",\"Lines beginning with $ are threads: [THREAD_NUMBER], [RED], [GREEN], [BLUE], [DESCRIPTION], [CATALOG_NUMBER]\"\n");
    fprintf(file, "\"#\",\"Lines beginning with * are stitch entries: [STITCH_TYPE], [X], [Y]\"\n");
    fprintf(file, "\n");
    fprintf(file, "\"#\",\"Stitch Entry Notes:\"\n");
    fprintf(file, "\"#\",\"STITCH instructs the machine to move to the position [X][Y] and then make a stitch.\"\n");
    fprintf(file, "\"#\",\"JUMP instructs the machine to move to the position [X][Y] without making a stitch.\"\n");
    fprintf(file, "\"#\",\"TRIM instructs the machine to cut the thread before moving to the position [X][Y] without making a stitch.\"\n");
    fprintf(file, "\"#\",\"COLOR instructs the machine to stop so that the user can change to a different color thread.\"\n");
    fprintf(file, "\"#\",\"END instructs the machine that the design is completed and there are no further instructions.\"\n");
    fprintf(file, "\"#\",\"UNKNOWN encompasses instructions that may not be supported currently.\"\n");
    fprintf(file, "\"#\",\"[X] and [Y] are absolute coordinates in millimeters (mm).\"\n");
    fprintf(file, "\n");

    /* write variables */
    fprintf(file,"\"#\",\"[VAR_NAME]\",\"[VAR_VALUE]\"\n");
    fprintf(file, "\">\",\"STITCH_COUNT:\",\"%u\"\n",   stitchCount);
    fprintf(file, "\">\",\"THREAD_COUNT:\",\"%u\"\n",   threadCount);
    fprintf(file, "\">\",\"EXTENTS_LEFT:\",\"%f\"\n",   boundingRect.left);
    fprintf(file, "\">\",\"EXTENTS_TOP:\",\"%f\"\n",    boundingRect.top);
    fprintf(file, "\">\",\"EXTENTS_RIGHT:\",\"%f\"\n",  boundingRect.right);
    fprintf(file, "\">\",\"EXTENTS_BOTTOM:\",\"%f\"\n", boundingRect.bottom);
    fprintf(file, "\">\",\"EXTENTS_WIDTH:\",\"%f\"\n",  embRect_width(boundingRect));
    fprintf(file, "\">\",\"EXTENTS_HEIGHT:\",\"%f\"\n", embRect_height(boundingRect));
    fprintf(file,"\n");

    /* write colors */
    fprintf(file, "\"#\",\"[THREAD_NUMBER]\",\"[RED]\",\"[GREEN]\",\"[BLUE]\",\"[DESCRIPTION]\",\"[CATALOG_NUMBER]\"\n");
    i = 1;
    while(tList)
    {
        fprintf(file, "\"$\",\"%d\",\"%d\",\"%d\",\"%d\",\"%s\",\"%s\"\n", i,
                tList->thread.color.r,
                tList->thread.color.g,
                tList->thread.color.b,
                tList->thread.description,
                tList->thread.catalogNumber);
        i++;
        tList = tList->next;
    }
    fprintf(file, "\n");

    /* write stitches */
    fprintf(file, "\"#\",\"[STITCH_TYPE]\",\"[X]\",\"[Y]\"\n");
    while(sList)
    {
        EmbStitch s = sList->stitch;
        fprintf(file, "\"*\",\"%s\",\"%lf\",\"%lf\"\n", csvStitchFlagToStr(s.flags), s.xx, s.yy);
        sList = sList->next;
    }

    fclose(file);
    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
