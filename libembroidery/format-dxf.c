#include "format-dxf.h"
#include "helpers-misc.h"
#include "emb-hash.h"
#include "emb-logging.h"

#include "geom-arc.h"
/*#include "geom-line.h" */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DXF Version Identifiers */
#define DXF_VERSION_R10 "AC1006"
#define DXF_VERSION_R11 "AC1009"
#define DXF_VERSION_R12 "AC1009"
#define DXF_VERSION_R13 "AC1012"
#define DXF_VERSION_R14 "AC1014"
#define DXF_VERSION_R15 "AC1015"
#define DXF_VERSION_R18 "AC1018"
#define DXF_VERSION_R21 "AC1021"
#define DXF_VERSION_R24 "AC1024"
#define DXF_VERSION_R27 "AC1027"

#define DXF_VERSION_2000 "AC1015"
#define DXF_VERSION_2002 "AC1015"
#define DXF_VERSION_2004 "AC1018"
#define DXF_VERSION_2006 "AC1018"
#define DXF_VERSION_2007 "AC1021"
#define DXF_VERSION_2009 "AC1021"
#define DXF_VERSION_2010 "AC1024"
#define DXF_VERSION_2013 "AC1027"

/* Based on the DraftSight color table */
static const unsigned char _dxfColorTable[][3] = {
{   0,   0,   0 }, /*   '0' (BYBLOCK)    */
{ 255,   0,   0 }, /*   '1' (red)        */
{ 255, 255,   0 }, /*   '2' (yellow)     */
{   0, 255,   0 }, /*   '3' (green)      */
{   0, 255, 255 }, /*   '4' (cyan)       */
{   0,   0, 255 }, /*   '5' (blue)       */
{ 255,   0, 255 }, /*   '6' (magenta)    */
{ 255, 255, 255 }, /*   '7' (white)      */
{ 128, 128, 128 }, /*   '8' (dark gray)  */
{ 192, 192, 192 }, /*   '9' (light gray) */
{ 255,   0,   0 }, /*  '10' */
{ 255, 127, 127 }, /*  '11' */
{ 204,   0,   0 }, /*  '12' */
{ 204, 102, 102 }, /*  '13' */
{ 153,   0,   0 }, /*  '14' */
{ 153,  76,  76 }, /*  '15' */
{ 127,   0,   0 }, /*  '16' */
{ 127,  63,  63 }, /*  '17' */
{  76,   0,   0 }, /*  '18' */
{  76,  38,  38 }, /*  '19' */
{ 255,  63,   0 }, /*  '20' */
{ 255, 159, 127 }, /*  '21' */
{ 204,  51,   0 }, /*  '22' */
{ 204, 127, 102 }, /*  '23' */
{ 153,  38,   0 }, /*  '24' */
{ 153,  95,  76 }, /*  '25' */
{ 127,  31,   0 }, /*  '26' */
{ 127,  79,  63 }, /*  '27' */
{  76,  19,   0 }, /*  '28' */
{  76,  47,  38 }, /*  '29' */
{ 255, 127,   0 }, /*  '30' */
{ 255, 191, 127 }, /*  '31' */
{ 204, 102,   0 }, /*  '32' */
{ 204, 153, 102 }, /*  '33' */
{ 153,  76,   0 }, /*  '34' */
{ 153, 114,  76 }, /*  '35' */
{ 127,  63,   0 }, /*  '36' */
{ 127,  95,  63 }, /*  '37' */
{  76,  38,   0 }, /*  '38' */
{  76,  57,  38 }, /*  '39' */
{ 255, 191,   0 }, /*  '40' */
{ 255, 223, 127 }, /*  '41' */
{ 204, 153,   0 }, /*  '42' */
{ 204, 178, 102 }, /*  '43' */
{ 153, 114,   0 }, /*  '44' */
{ 153, 133,  76 }, /*  '45' */
{ 127,  95,   0 }, /*  '46' */
{ 127, 111,  63 }, /*  '47' */
{  76,  57,   0 }, /*  '48' */
{  76,  66,  38 }, /*  '49' */
{ 255, 255,   0 }, /*  '50' */
{ 255, 255, 127 }, /*  '51' */
{ 204, 204,   0 }, /*  '52' */
{ 204, 204, 102 }, /*  '53' */
{ 153, 153,   0 }, /*  '54' */
{ 153, 153,  76 }, /*  '55' */
{ 127, 127,   0 }, /*  '56' */
{ 127, 127,  63 }, /*  '57' */
{  76,  76,   0 }, /*  '58' */
{  76,  76,  38 }, /*  '59' */
{ 191, 255,   0 }, /*  '60' */
{ 223, 255, 127 }, /*  '61' */
{ 153, 204,   0 }, /*  '62' */
{ 178, 204, 102 }, /*  '63' */
{ 114, 153,   0 }, /*  '64' */
{ 133, 153,  76 }, /*  '65' */
{  95, 127,   0 }, /*  '66' */
{ 111, 127,  63 }, /*  '67' */
{  57,  76,   0 }, /*  '68' */
{  66,  76,  38 }, /*  '69' */
{ 127, 255,   0 }, /*  '70' */
{ 191, 255, 127 }, /*  '71' */
{ 102, 204,   0 }, /*  '72' */
{ 153, 204, 102 }, /*  '73' */
{  76, 153,   0 }, /*  '74' */
{ 114, 153,  76 }, /*  '75' */
{  63, 127,   0 }, /*  '76' */
{  95, 127,  63 }, /*  '77' */
{  38,  76,   0 }, /*  '78' */
{  57,  76,  38 }, /*  '79' */
{  63, 255,   0 }, /*  '80' */
{ 159, 255, 127 }, /*  '81' */
{  51, 204,   0 }, /*  '82' */
{ 127, 204, 102 }, /*  '83' */
{  38, 153,   0 }, /*  '84' */
{  95, 153,  76 }, /*  '85' */
{  31, 127,   0 }, /*  '86' */
{  79, 127,  63 }, /*  '87' */
{  19,  76,   0 }, /*  '88' */
{  47,  76,  38 }, /*  '89' */
{   0, 255,   0 }, /*  '90' */
{ 127, 255, 127 }, /*  '91' */
{   0, 204,   0 }, /*  '92' */
{ 102, 204, 102 }, /*  '93' */
{   0, 153,   0 }, /*  '94' */
{  76, 153,  76 }, /*  '95' */
{   0, 127,   0 }, /*  '96' */
{  63, 127,  63 }, /*  '97' */
{   0,  76,   0 }, /*  '98' */
{  38,  76,  38 }, /*  '99' */
{   0, 255,  63 }, /* '100' */
{ 127, 255, 159 }, /* '101' */
{   0, 204,  51 }, /* '102' */
{ 102, 204, 127 }, /* '103' */
{   0, 153,  38 }, /* '104' */
{  76, 153,  95 }, /* '105' */
{   0, 127,  31 }, /* '106' */
{  63, 127,  79 }, /* '107' */
{   0,  76,  19 }, /* '108' */
{  38,  76,  47 }, /* '109' */
{   0, 255, 127 }, /* '110' */
{ 127, 255, 191 }, /* '111' */
{   0, 204, 102 }, /* '112' */
{ 102, 204, 153 }, /* '113' */
{   0, 153,  76 }, /* '114' */
{  76, 153, 114 }, /* '115' */
{   0, 127,  63 }, /* '116' */
{  63, 127,  95 }, /* '117' */
{   0,  76,  38 }, /* '118' */
{  38,  76,  57 }, /* '119' */
{   0, 255, 191 }, /* '120' */
{ 127, 255, 223 }, /* '121' */
{   0, 204, 153 }, /* '122' */
{ 102, 204, 178 }, /* '123' */
{   0, 153, 114 }, /* '124' */
{  76, 153, 133 }, /* '125' */
{   0, 127,  95 }, /* '126' */
{  63, 127, 111 }, /* '127' */
{   0,  76,  57 }, /* '128' */
{  38,  76,  66 }, /* '129' */
{   0, 255, 255 }, /* '130' */
{ 127, 255, 255 }, /* '131' */
{   0, 204, 204 }, /* '132' */
{ 102, 204, 204 }, /* '133' */
{   0, 153, 153 }, /* '134' */
{  76, 153, 153 }, /* '135' */
{   0, 127, 127 }, /* '136' */
{  63, 127, 127 }, /* '137' */
{   0,  76,  76 }, /* '138' */
{  38,  76,  76 }, /* '139' */
{   0, 191, 255 }, /* '140' */
{ 127, 223, 255 }, /* '141' */
{   0, 153, 204 }, /* '142' */
{ 102, 178, 204 }, /* '143' */
{   0, 114, 153 }, /* '144' */
{  76, 133, 153 }, /* '145' */
{   0,  95, 127 }, /* '146' */
{  63, 111, 127 }, /* '147' */
{   0,  57,  76 }, /* '148' */
{  38,  66,  76 }, /* '149' */
{   0, 127, 255 }, /* '150' */
{ 127, 191, 255 }, /* '151' */
{   0, 102, 204 }, /* '152' */
{ 102, 153, 204 }, /* '153' */
{   0,  76, 153 }, /* '154' */
{  76, 114, 153 }, /* '155' */
{   0,  63, 127 }, /* '156' */
{  63,  95, 127 }, /* '157' */
{   0,  38,  76 }, /* '158' */
{  38,  57,  76 }, /* '159' */
{   0,  63, 255 }, /* '160' */
{ 127, 159, 255 }, /* '161' */
{   0,  51, 204 }, /* '162' */
{ 102, 127, 204 }, /* '163' */
{   0,  38, 153 }, /* '164' */
{  76,  95, 153 }, /* '165' */
{   0,  31, 127 }, /* '166' */
{  63,  79, 127 }, /* '167' */
{   0,  19,  76 }, /* '168' */
{  38,  47,  76 }, /* '169' */
{   0,   0, 255 }, /* '170' */
{ 127, 127, 255 }, /* '171' */
{   0,   0, 204 }, /* '172' */
{ 102, 102, 204 }, /* '173' */
{   0,   0, 153 }, /* '174' */
{  76,  76, 153 }, /* '175' */
{   0,   0, 127 }, /* '176' */
{  63,  63, 127 }, /* '177' */
{   0,   0,  76 }, /* '178' */
{  38,  38,  76 }, /* '179' */
{  63,   0, 255 }, /* '180' */
{ 159, 127, 255 }, /* '181' */
{  51,   0, 204 }, /* '182' */
{ 127, 102, 204 }, /* '183' */
{  38,   0, 153 }, /* '184' */
{  95,  76, 153 }, /* '185' */
{  31,   0, 127 }, /* '186' */
{  79,  63, 127 }, /* '187' */
{  19,   0,  76 }, /* '188' */
{  47,  38,  76 }, /* '189' */
{ 127,   0, 255 }, /* '190' */
{ 191, 127, 255 }, /* '191' */
{ 102,   0, 204 }, /* '192' */
{ 153, 102, 204 }, /* '193' */
{  76,   0, 153 }, /* '194' */
{ 114,  76, 153 }, /* '195' */
{  63,   0, 127 }, /* '196' */
{  95,  63, 127 }, /* '197' */
{  38,   0,  76 }, /* '198' */
{  57,  38,  76 }, /* '199' */
{ 191,   0, 255 }, /* '200' */
{ 223, 127, 255 }, /* '201' */
{ 153,   0, 204 }, /* '202' */
{ 178, 102, 204 }, /* '203' */
{ 114,   0, 153 }, /* '204' */
{ 133,  76, 153 }, /* '205' */
{  95,   0, 127 }, /* '206' */
{ 111,  63, 127 }, /* '207' */
{  57,   0,  76 }, /* '208' */
{  66,  38,  76 }, /* '209' */
{ 255,   0, 255 }, /* '210' */
{ 255, 127, 255 }, /* '211' */
{ 204,   0, 204 }, /* '212' */
{ 204, 102, 204 }, /* '213' */
{ 153,   0, 153 }, /* '214' */
{ 153,  76, 153 }, /* '215' */
{ 127,   0, 127 }, /* '216' */
{ 127,  63, 127 }, /* '217' */
{  76,   0,  76 }, /* '218' */
{  76,  38,  76 }, /* '219' */
{ 255,   0, 191 }, /* '220' */
{ 255, 127, 223 }, /* '221' */
{ 204,   0, 153 }, /* '222' */
{ 204, 102, 178 }, /* '223' */
{ 153,   0, 114 }, /* '224' */
{ 153,  76, 133 }, /* '225' */
{ 127,   0,  95 }, /* '226' */
{ 127,  63, 111 }, /* '227' */
{  76,   0,  57 }, /* '228' */
{  76,  38,  66 }, /* '229' */
{ 255,   0, 127 }, /* '230' */
{ 255, 127, 191 }, /* '231' */
{ 204,   0, 102 }, /* '232' */
{ 204, 102, 153 }, /* '233' */
{ 153,   0,  76 }, /* '234' */
{ 153,  76, 114 }, /* '235' */
{ 127,   0,  63 }, /* '236' */
{ 127,  63,  95 }, /* '237' */
{  76,   0,  38 }, /* '238' */
{  76,  38,  57 }, /* '239' */
{ 255,   0,  63 }, /* '240' */
{ 255, 127, 159 }, /* '241' */
{ 204,   0,  51 }, /* '242' */
{ 204, 102, 127 }, /* '243' */
{ 153,   0,  38 }, /* '244' */
{ 153,  76,  95 }, /* '245' */
{ 127,   0,  31 }, /* '246' */
{ 127,  63,  79 }, /* '247' */
{  76,   0,  19 }, /* '248' */
{  76,  38,  47 }, /* '249' */
{  51,  51,  51 }, /* '250' */
{  91,  91,  91 }, /* '251' */
{ 132, 132, 132 }, /* '252' */
{ 173, 173, 173 }, /* '253' */
{ 214, 214, 214 }, /* '254' */
{ 255, 255, 255 }, /* '255' */
{   0,   0,   0 }  /* '256' (BYLAYER) */
};

char* readLine(FILE* file)
{
    char str[255];
    /* TODO: replace all scanf code */
    fscanf(file, "%s", str);
    return lTrim(str, ' ');
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readDxf(EmbPattern* pattern, const char* fileName)
{

    FILE* file = 0;
    char* buff = "";

    char* dxfVersion = "";
    char* section = "";
    char* tableName = "";
    char* layerName = "";
    char* entityType = "";
    EmbHash* layerColorHash = 0; /* hash <layerName, EmbColor> */

    int eof = 0; /* End Of File */

    double bulge = 0.0, firstX = 0.0, firstY = 0.0, x = 0.0, y, prevX = 0.0, prevY = 0.0;
    char firstStitch = 1;
    char bulgeFlag = 0;
    int fileLength = 0;
    unsigned char colorNum = 0;

    if(!pattern) { embLog_error("format-dxf.c readDxf(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dxf.c readDxf(), fileName argument is null\n"); return 0; }

    layerColorHash = embHash_create();
    if(!layerColorHash) { embLog_error("format-dxf.c readDxf(), unable to allocate memory for layerColorHash\n"); return 0; }

    file = fopen(fileName, "r");
    if(!file)
    {
        embLog_error("format-dxf.c readDxf(), cannot open %s for reading\n", fileName);
        return 0;
    }

    fseek(file, 0L, SEEK_END);

    fileLength = ftell(file);
    fseek(file, 0L, SEEK_SET);

    while(ftell(file) < fileLength)
    {
        buff = readLine(file);
        /*printf("%s\n", buff);*/
        if( (!strcmp(buff, "HEADER"))   ||
            (!strcmp(buff, "CLASSES"))  ||
            (!strcmp(buff, "TABLES"))   ||
            (!strcmp(buff, "BLOCKS"))   ||
            (!strcmp(buff, "ENTITIES")) ||
            (!strcmp(buff, "OBJECTS"))  ||
            (!strcmp(buff, "THUMBNAILIMAGE")))
        {
            section = buff;
            printf("SECTION:%s\n", buff);
        }
        if(!strcmp(buff, "ENDSEC"))
        {
            section = "";
            printf("ENDSEC:%s\n", buff);
        }
        if( (!strcmp(buff, "ARC"))        ||
            (!strcmp(buff, "CIRCLE"))     ||
            (!strcmp(buff, "ELLIPSE"))    ||
            (!strcmp(buff, "LINE"))       ||
            (!strcmp(buff, "LWPOLYLINE")) ||
            (!strcmp(buff, "POINT")))
        {
            entityType = buff;
        }
        if(!strcmp(buff, "EOF"))
        {
            eof = 1;
        }

        if(!strcmp(section, "HEADER"))
        {
            if(!strcmp(buff, "$ACADVER"))
            {
                buff = readLine(file);
                dxfVersion = readLine(file);
                /* TODO: Allow these versions when POLYLINE is handled. */
                if((!strcmp(dxfVersion, DXF_VERSION_R10))
                || (!strcmp(dxfVersion, DXF_VERSION_R11))
                || (!strcmp(dxfVersion, DXF_VERSION_R12))
                || (!strcmp(dxfVersion, DXF_VERSION_R13)))
                    return 0;
            }
        }
        else if(!strcmp(section,"TABLES"))
        {
            if(!strcmp(buff,"ENDTAB"))
            {
                tableName = NULL;
            }

            if(tableName == NULL)
            {
                if(!strcmp(buff,"2")) /* Table Name */
                {
                    tableName = readLine(file);
                }
            }
            else if(!strcmp(tableName, "LAYER"))
            {
                /* Common Group Codes for Tables */
                if(!strcmp(buff,"5")) /* Handle */
                {
                    buff = readLine(file);
                    continue;
                }
                else if(!strcmp(buff,"330")) /* Soft Pointer */
                {
                    buff = readLine(file);
                    continue;
                }
                else if(!strcmp(buff,"100")) /* Subclass Marker */
                {
                    buff = readLine(file);
                    continue;
                }
                else if(!strcmp(buff,"70")) /* Number of Entries in Table */
                {
                    buff = readLine(file);
                    continue;
                }
                /* The meaty stuff */
                else if(!strcmp(buff,"2")) /* Layer Name */
                {
                    layerName = readLine(file);
                }
                else if(!strcmp(buff,"62")) /* Color Number */
                {
                    buff = readLine(file);
                    colorNum = atoi(buff);
/*
                    TODO: finish this
                    unsigned char colorNum = atoi(buff);
                    EmbColor* co = embColor_create(_dxfColorTable[colorNum][0], _dxfColorTable[colorNum][1], _dxfColorTable[colorNum][2]);
                    if(!co) { / TODO: error allocating memory for EmbColor  return 0; }
                    printf("inserting:%s,%d,%d,%d\n", layerName, co->r, co->g, co->b);
                    if(embHash_insert(layerColorHash, emb_strdup(layerName), co))
                    {
                         TODO: log error: failed inserting into layerColorHash
                    }
*/
                    layerName = NULL;
                }
            }
        }
        else if(!strcmp(section,"ENTITIES"))
        {
            /* Common Group Codes for Entities */
            if(!strcmp(buff, "5")) /* Handle */
            {
                buff = readLine(file);
                continue;
            }
            else if(!strcmp(buff, "330")) /* Soft Pointer */
            {
                buff = readLine(file);
                continue;
            }
            else if(!strcmp(buff, "100")) /* Subclass Marker */
            {
                buff = readLine(file);
                continue;
            }
            else if(!strcmp(buff, "8")) /* Layer Name */
            {
                buff = readLine(file);
                /* embPattern_changeColor(pattern, colorIndexMap[buff]); TODO: port to C */
                continue;
            }

            if(!strcmp(entityType,"LWPOLYLINE"))
            {
                double* arcMidX = 0;
                double* arcMidY = 0;
                /* The not so important group codes */
                if(!strcmp(buff, "90")) /* Vertices */
                {
                    buff = readLine(file);
                    continue;
                }
                else if(!strcmp(buff,"70")) /* Polyline Flag */
                {
                    buff = readLine(file);
                    continue;
                }
                /* TODO: Try to use the widths at some point */
                else if(!strcmp(buff,"40")) /* Starting Width */
                {
                    buff = readLine(file);
                    continue;
                }
                else if(!strcmp(buff,"41")) /* Ending Width */
                {
                    buff = readLine(file);
                    continue;
                }
                else if(!strcmp(buff,"43")) /* Constant Width */
                {
                    buff = readLine(file);
                    continue;
                }
                /* The meaty stuff */
                else if(!strcmp(buff,"42")) /* Bulge */
                {
                    buff = readLine(file);
                    bulge = atof(buff);
                    bulgeFlag = 1;
                }
                else if(!strcmp(buff,"10")) /* X */
                {
                    buff = readLine(file);
                    x = atof(buff);
                }
                else if(!strcmp(buff,"20")) /* Y */
                {
                    buff = readLine(file);
                    y = atof(buff);

                    if(bulgeFlag)
                    {
                        bulgeFlag = 0;
                        if(!getArcDataFromBulge(bulge, prevX, prevY, x, y, arcMidX, arcMidY, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
                        {
                            /*TODO: error */
                            return 0;
                        }
                        if(firstStitch)
                        {
                            /* embPattern_addStitchAbs(pattern, x, y, TRIM, 1); TODO: Add moveTo point to embPath pointList */
                        }
                        /* embPattern_addStitchAbs(pattern, x, y, ARC, 1); TODO: Add arcTo point to embPath pointList */
                    }
                    else
                    {
                        /*if(firstStitch) embPattern_addStitchAbs(pattern, x, y, TRIM, 1); TODO: Add moveTo point to embPath pointList */
                        /*else            embPattern_addStitchAbs(pattern, x, y, NORMAL, 1); TODO: Add lineTo point to embPath pointList */
                    }
                    prevX = x;
                    prevY = y;
                    if(firstStitch)
                    {
                        firstX = x;
                        firstY = y;
                        firstStitch = 0;
                    }
                }
                else if(!strcmp(buff,"0"))
                {
                    entityType = NULL;
                    firstStitch = 1;
                    if(bulgeFlag)
                    {
                        bulgeFlag = 0;
                        if(!getArcDataFromBulge(bulge, prevX, prevY, firstX, firstY, arcMidX, arcMidY, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
                        {
                            /*TODO: error */
                            return 0;
                        }
                        /* embPattern_addStitchAbs(pattern, prevX, prevY, ARC, 1); TODO: Add arcTo point to embPath pointList */
                    }
                    else
                    {
                        /* embPattern_addStitchAbs(pattern, firstX, firstY, NORMAL, 1); TODO: Add lineTo point to embPath pointList */
                    }
                }
            } /* end LWPOLYLINE */
        } /* end ENTITIES section */
    } /* end while loop */

    fclose(file);

    /*
    EmbColor* testColor = 0;
    testColor = embHash_value(layerColorHash, "OMEGA");
    if(!testColor) printf("NULL POINTER!!!!!!!!!!!!!!\n");
    else printf("LAYERCOLOR: %d,%d,%d\n", testColor->r, testColor->g, testColor->b);
    */

    if(!eof)
    {
        /* NOTE: The EOF item must be present at the end of file to be considered a valid DXF file. */
        embLog_error("format-dxf.c readDxf(), missing EOF at end of DXF file\n");
    }
    return eof;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeDxf(EmbPattern* pattern, const char* fileName)
{
    if(!pattern) { embLog_error("format-dxf.c writeDxf(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-dxf.c writeDxf(), fileName argument is null\n"); return 0; }
    return 0; /*TODO: finish writeDxf */
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
