#include "format-svg.h"
#include "emb-file.h"
#include "emb-logging.h"
#include "helpers-misc.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

EmbColor svgColorToEmbColor(char* colorString)
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    int i = 0;
    char* pEnd = 0;
    char* colorStr = copy_trim(colorString); /* Trim out any junk spaces */
    int length = strlen(colorStr);

    /* SVGTiny1.2 Spec Section 11.13.1 syntax for color values */
    if(length == 7 && colorStr[0] == '#') /* Six digit hex — #rrggbb */
    {
        EmbColor hexColor = embColor_fromHexStr(lTrim(colorStr, '#'));
        r = hexColor.r;
        g = hexColor.g;
        b = hexColor.b;
    }
    else if(length == 4 && colorStr[0] == '#') /* Three digit hex — #rgb */
    {
        EmbColor hexColor;
        /* Convert the 3 digit hex to a six digit hex */
        char hex[7];
        hex[0] = colorStr[1];
        hex[1] = colorStr[1];
        hex[2] = colorStr[2];
        hex[3] = colorStr[2];
        hex[4] = colorStr[3];
        hex[5] = colorStr[3];
        hex[6] = 0;

        hexColor = embColor_fromHexStr(hex);
        r = hexColor.r;
        g = hexColor.g;
        b = hexColor.b;
    }
    else if(strstr(colorStr, "%")) /* Float functional — rgb(R%, G%, B%) */
    {
        for(i = 0; i < length; i++)
        {
            if(colorStr[i] == 'r') colorStr[i] = ' ';
            if(colorStr[i] == 'g') colorStr[i] = ' ';
            if(colorStr[i] == 'b') colorStr[i] = ' ';
            if(colorStr[i] == ',') colorStr[i] = ' ';
            if(colorStr[i] == '(') colorStr[i] = ' ';
            if(colorStr[i] == ')') colorStr[i] = ' ';
            if(colorStr[i] == '%') colorStr[i] = ' ';
        }
        r = (unsigned char)roundDouble(255.0/100.0 * strtod(colorStr, &pEnd));
        g = (unsigned char)roundDouble(255.0/100.0 * strtod(pEnd,     &pEnd));
        b = (unsigned char)roundDouble(255.0/100.0 * strtod(pEnd,     &pEnd));
    }
    else if(length > 3 && startsWith("rgb", colorStr)) /* Integer functional — rgb(rrr, ggg, bbb) */
    {
        for(i = 0; i < length; i++)
        {
            if(colorStr[i] == 'r') colorStr[i] = ' ';
            if(colorStr[i] == 'g') colorStr[i] = ' ';
            if(colorStr[i] == 'b') colorStr[i] = ' ';
            if(colorStr[i] == ',') colorStr[i] = ' ';
            if(colorStr[i] == '(') colorStr[i] = ' ';
            if(colorStr[i] == ')') colorStr[i] = ' ';
        }
        r = (unsigned char)strtol(colorStr, &pEnd, 10);
        g = (unsigned char)strtol(pEnd,     &pEnd, 10);
        b = (unsigned char)strtol(pEnd,     &pEnd, 10);
    }
    else /* Color keyword */
    {
        if     (!strcmp(colorStr, "black"))   { r =   0; g =   0; b =   0; }
        else if(!strcmp(colorStr, "silver"))  { r = 192; g = 192; b = 192; }
        else if(!strcmp(colorStr, "gray"))    { r = 128; g = 128; b = 128; }
        else if(!strcmp(colorStr, "white"))   { r = 255; g = 255; b = 255; }
        else if(!strcmp(colorStr, "maroon"))  { r = 128; g =   0; b =   0; }
        else if(!strcmp(colorStr, "red"))     { r = 255; g =   0; b =   0; }
        else if(!strcmp(colorStr, "purple"))  { r = 128; g =   0; b = 128; }
        else if(!strcmp(colorStr, "fuchsia")) { r = 255; g =   0; b = 255; }
        else if(!strcmp(colorStr, "green"))   { r =   0; g = 128; b =   0; }
        else if(!strcmp(colorStr, "lime"))    { r =   0; g = 255; b =   0; }
        else if(!strcmp(colorStr, "olive"))   { r = 128; g = 128; b =   0; }
        else if(!strcmp(colorStr, "yellow"))  { r = 255; g = 255; b =   0; }
        else if(!strcmp(colorStr, "navy"))    { r =   0; g =   0; b = 128; }
        else if(!strcmp(colorStr, "blue"))    { r =   0; g =   0; b = 255; }
        else if(!strcmp(colorStr, "teal"))    { r =   0; g = 128; b = 128; }
        else if(!strcmp(colorStr, "aqua"))    { r =   0; g = 255; b = 255; }
    }

    free(colorStr);
    /* Returns black if all else fails */
    return embColor_make(r, g, b);
}

EmbFlag svgPathCmdToEmbPathFlag(char cmd)
{
    /* TODO: This function needs some work */
    /*
    if     (toUpper(cmd) == 'M') return MOVETO;
    else if(toUpper(cmd) == 'L') return LINETO;
    else if(toUpper(cmd) == 'C') return CUBICTOCONTROL1;
    else if(toUpper(cmd) == 'CC') return CUBICTOCONTROL2;
    else if(toUpper(cmd) == 'CCC') return CUBICTOEND;
    else if(toUpper(cmd) == 'A') return ELLIPSETORAD;
    else if(toUpper(cmd) == 'AA') return ELLIPSETOEND;
    else if(toUpper(cmd) == 'Q') return QUADTOCONTROL;
    else if(toUpper(cmd) == 'QQ') return QUADTOEND;
    else if(toUpper(cmd) == 'Z') return LINETO;
    */

    /*else if(toUpper(cmd) == 'B') return BULGETOCONTROL; /* NOTE: This is not part of the SVG spec, but hopefully Bulges will be added to the SVG spec someday */
	/*else if(toUpper(cmd) == 'BB') return BULGETOEND;    /* NOTE: This is not part of the SVG spec, but hopefully Bulges will be added to the SVG spec someday */
    /*else { embLog_error("format-svg.c svgPathCmdToEmbPathFlag(), unknown command '%c'\n", cmd); return MOVETO; } */

    return LINETO;
}

SvgAttribute svgAttribute_create(const char* name, const char* value)
{
    SvgAttribute attribute;
    char* modValue = 0;
    int last = 0;
    int i = 0;

    modValue = emb_strdup(value);
    last = strlen(modValue);
    for(i = 0; i < last; i++)
    {
        if(modValue[i] == '"') modValue[i] = ' ';
        if(modValue[i] == '\'') modValue[i] = ' ';
        if(modValue[i] == '/') modValue[i] = ' ';
        if(modValue[i] == ',') modValue[i] = ' ';
    }
    attribute.name = emb_strdup(name);
    attribute.value = modValue;
    return attribute;
}

void svgElement_addAttribute(SvgElement* element, SvgAttribute data)
{
    if(!element) { embLog_error("format-svg.c svgElement_addAttribute(), element argument is null\n"); return; }

    if(!(element->attributeList))
    {
        element->attributeList = (SvgAttributeList*)malloc(sizeof(SvgAttributeList));
        if(!(element->attributeList)) { embLog_error("format-svg.c svgElement_addAttribute(), cannot allocate memory for element->attributeList\n"); return; }
        element->attributeList->attribute = data;
        element->attributeList->next = 0;
        element->lastAttribute = element->attributeList;
        element->lastAttribute->next = 0;
    }
    else
    {
        SvgAttributeList* pointerLast = element->lastAttribute;
        SvgAttributeList* list = (SvgAttributeList*)malloc(sizeof(SvgAttributeList));
        if(!list) { embLog_error("format-svg.c svgElement_addAttribute(), cannot allocate memory for list\n"); return; }
        list->attribute = data;
        list->next = 0;
        pointerLast->next = list;
        element->lastAttribute = list;
    }
}

void svgElement_free(SvgElement* element)
{
    SvgAttributeList* list = 0;
    SvgAttributeList* nextList = 0;
    if(!element) return;

    list = element->attributeList;

    while(list)
    {
        free(list->attribute.name);
        list->attribute.name = 0;
        free(list->attribute.value);
        list->attribute.value = 0;
        nextList = list->next;
        free(list);
        list = 0;
        list = nextList;
    }

    element->lastAttribute = 0;
    free(element->name);
    element->name = 0;
    free(element);
    element = 0;
}

SvgElement* svgElement_create(const char* name)
{
    SvgElement* element = 0;

    element = (SvgElement*)malloc(sizeof(SvgElement));
    if(!element) { embLog_error("format-svg.c svgElement_create(), cannot allocate memory for element\n"); return 0; }
    element->name = emb_strdup(name);
    if(!element->name) { embLog_error("format-svg.c svgElement_create(), element->name is null\n"); return 0; }
    element->attributeList = 0;
    element->lastAttribute = 0;
    return element;
}

char* svgAttribute_getValue(SvgElement* element, const char* name)
{
    SvgAttributeList* pointer = 0;

    if(!element) { embLog_error("format-svg.c svgAttribute_getValue(), element argument is null\n"); return "none"; }
    if(!name) { embLog_error("format-svg.c svgAttribute_getValue(), name argument is null\n"); return "none"; }
    if(!element->attributeList) { /* TODO: error */ return "none"; }

    pointer = element->attributeList;
    while(pointer)
    {
        if(!strcmp(pointer->attribute.name, name)) { return pointer->attribute.value; }
        pointer = pointer->next;
    }

    return "none";
}

void svgAddToPattern(EmbPattern* p)
{
    const char* buff = 0;

    if(!p) { embLog_error("format-svg.c svgAddToPattern(), p argument is null\n"); return; }
    if(!currentElement) { return; }

    buff = currentElement->name;
    if(!buff) { return; }

    if     (!strcmp(buff, "?xml"))             {  }
    else if(!strcmp(buff, "a"))                {  }
    else if(!strcmp(buff, "animate"))          {  }
    else if(!strcmp(buff, "animateColor"))     {  }
    else if(!strcmp(buff, "animateMotion"))    {  }
    else if(!strcmp(buff, "animateTransform")) {  }
    else if(!strcmp(buff, "animation"))        {  }
    else if(!strcmp(buff, "audio"))            {  }
    else if(!strcmp(buff, "circle"))
    {
        embPattern_addCircleObjectAbs(p, atof(svgAttribute_getValue(currentElement, "cx")),
                                         atof(svgAttribute_getValue(currentElement, "cy")),
                                         atof(svgAttribute_getValue(currentElement, "r")));
    }
    else if(!strcmp(buff, "defs"))             {  }
    else if(!strcmp(buff, "desc"))             {  }
    else if(!strcmp(buff, "discard"))          {  }
    else if(!strcmp(buff, "ellipse"))
    {
        embPattern_addEllipseObjectAbs(p, atof(svgAttribute_getValue(currentElement, "cx")),
                                          atof(svgAttribute_getValue(currentElement, "cy")),
                                          atof(svgAttribute_getValue(currentElement, "rx")),
                                          atof(svgAttribute_getValue(currentElement, "ry")));
    }
    else if(!strcmp(buff, "font"))             {  }
    else if(!strcmp(buff, "font-face"))        {  }
    else if(!strcmp(buff, "font-face-src"))    {  }
    else if(!strcmp(buff, "font-face-uri"))    {  }
    else if(!strcmp(buff, "foreignObject"))    {  }
    else if(!strcmp(buff, "g"))                {  }
    else if(!strcmp(buff, "glyph"))            {  }
    else if(!strcmp(buff, "handler"))          {  }
    else if(!strcmp(buff, "hkern"))            {  }
    else if(!strcmp(buff, "image"))            {  }
    else if(!strcmp(buff, "line"))
    {
        char* x1 = svgAttribute_getValue(currentElement, "x1");
        char* y1 = svgAttribute_getValue(currentElement, "y1");
        char* x2 = svgAttribute_getValue(currentElement, "x2");
        char* y2 = svgAttribute_getValue(currentElement, "y2");

        /* If the starting and ending points are the same, it is a point */
        if(!strcmp(x1, x2) && !strcmp(y1, y2))
            embPattern_addPointObjectAbs(p, atof(x1), atof(y1));
        else
            embPattern_addLineObjectAbs(p, atof(x1), atof(y1), atof(x2), atof(y2));
    }
    else if(!strcmp(buff, "linearGradient"))   {  }
    else if(!strcmp(buff, "listener"))         {  }
    else if(!strcmp(buff, "metadata"))         {  }
    else if(!strcmp(buff, "missing-glyph"))    {  }
    else if(!strcmp(buff, "mpath"))            {  }
    else if(!strcmp(buff, "path"))
    {
        /* TODO: finish */

        char* pointStr = svgAttribute_getValue(currentElement, "d");
        char* mystrok = svgAttribute_getValue(currentElement, "stroke");

        int last = strlen(pointStr);
        int size = 32;
        int i = 0;
        int j = 0;
        int pos = 0;
        /* An odometer aka 'tripometer' used for stepping thru the pathData */
        int trip = -1; /* count of float[] that has been filled. 0=first item of array, -1=not filled = empty array */
        int reset = -1;
        double xx = 0.0;
        double yy = 0.0;
        double fx = 0.0;
        double fy = 0.0;
        double lx = 0.0;
        double ly = 0.0;
        double cx1 = 0.0, cx2 = 0.0;
        double cy1 = 0.0, cy2 = 0.0;
        int cmd = 0;
        double pathData[7];
        unsigned int numMoves = 0;
        int pendingTask = 0;
        int relative = 0;

        EmbPointList* startOfPointList = 0;
        EmbPointList* pathObjPointList = 0;
        EmbFlagList* startOfFlagList = 0;
        EmbFlagList* pathObjFlagList = 0;

        char* pathbuff = 0;
        pathbuff = (char*)malloc(size);
        if(!pathbuff) { embLog_error("format-svg.c svgAddToPattern(), cannot allocate memory for pathbuff\n"); return; }

        printf("stroke:%s\n", mystrok);

        /* M44.219,26.365c0,10.306-8.354,18.659-18.652,18.659c-10.299,0-18.663-8.354-18.663-18.659c0-10.305,8.354-18.659,18.659-18.659C35.867,7.707,44.219,16.06,44.219,26.365z */
        for(i = 0; i < last; i++)
        {
            char c = pointStr[i];
            switch(c)
            {
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '.':
                    pathbuff[pos++] = (char)c;                  /* add a more char */
                    break;

                case ' ':
                case ',':

                    /*printf("    ,'%s'    ~POS=%d  ~TRIP=%d  ~[pos]=%d\n", pathbuff,pos,trip, pathbuff[pos]);*/
                    if(pos > 0) {         /* append float to array, if it not yet stored */
                        pathbuff[pos] = 0;
                        pos = 0;
                        printf("    ,val:%s\n", pathbuff);
                        pathData[++trip] = atof(pathbuff);
                    }
                    break;

                case '-':

                    if(pos > 0) {         /* append float to array, if it not yet stored */
                        pathbuff[pos] = 0;
                        pos = 0;
                        printf("    -val:%s\n", pathbuff);
                        pathData[++trip] = atof(pathbuff);
                    }
                    pathbuff[pos++] = (char)c;                  /* add a more char */
                    break;

                default:
                    /*** ASSUMED ANY COMMAND FOUND ***/


                    if(pos > 0) {         /* just make sure: append float to array, if it not yet stored */
                        pathbuff[pos] = 0;
                        pos = 0;
                        printf("    >val:%s\n", pathbuff);
                        pathData[++trip] = atof(pathbuff);
                    }

                    /**** Compose Point List ****/

                    /* below "while" is for avoid loosing last 'z' command that maybe never accomodated. */
                    pendingTask = 1; if (i==last-1) {pendingTask = 2;}

                    while (pendingTask > 0)
                    {
                        pendingTask -= 1;

                    /* Check wether prior command need to be saved */
                    if(trip>=0)
                    {
                            trip = -1;
                            reset = -1;

                            relative = 0; /* relative to prior coordinate point or absolute coordinate? */

                            if     (cmd == 'M') { xx = pathData[0]; yy = pathData[1]; fx = xx; fy = yy; }
                            else if(cmd == 'm') { xx = pathData[0]; yy = pathData[1]; fx = xx; fy = yy; relative=1; }
                            else if(cmd == 'L') { xx = pathData[0]; yy = pathData[1]; }
                            else if(cmd == 'l') { xx = pathData[0]; yy = pathData[1]; relative=1;}
                            else if(cmd == 'H') { xx = pathData[0]; yy = ly; }
                            else if(cmd == 'h') { xx = pathData[0]; yy = ly; relative=1;}
                            else if(cmd == 'V') { xx = lx;          yy = pathData[1]; }
                            else if(cmd == 'v') { xx = lx;          yy = pathData[1]; relative=1;}
                            else if(cmd == 'C') { xx = pathData[4]; yy = pathData[5]; cx1 = pathData[0]; cy1 = pathData[1]; cx2 = pathData[2]; cy2 = pathData[3]; }
                            else if(cmd == 'c') { xx = pathData[4]; yy = pathData[5]; cx1 = pathData[0]; cy1 = pathData[1]; cx2 = pathData[2]; cy2 = pathData[3]; relative=1;}
                            /*
                            else if(cmd == 'S') { xx = pathData[0]; yy = pathData[1]; }
                            else if(cmd == 's') { xx = pathData[0]; yy = pathData[1]; }
                            else if(cmd == 'Q') { xx = pathData[0]; yy = pathData[1]; }
                            else if(cmd == 'q') { xx = pathData[0]; yy = pathData[1]; }
                            else if(cmd == 'T') { xx = pathData[0]; yy = pathData[1]; }
                            else if(cmd == 't') { xx = pathData[0]; yy = pathData[1]; }
                            else if(cmd == 'A') { xx = pathData[0]; yy = pathData[1]; }
                            else if(cmd == 'a') { xx = pathData[0]; yy = pathData[1]; }
                            */
                            else if(cmd == 'Z') { xx = fx;          yy = fy; }
                            else if(cmd == 'z') { xx = fx;          yy = fy; }

                            if(!pathObjPointList && !pathObjFlagList)
                            {
                                pathObjPointList = embPointList_create(xx, yy);
                                startOfPointList = pathObjPointList;
                                pathObjFlagList = embFlagList_create(svgPathCmdToEmbPathFlag(cmd));
                                startOfFlagList = pathObjFlagList;
                            }
                            else
                            {
                                pathObjPointList = embPointList_add(pathObjPointList, embPoint_make(xx, yy));
                                pathObjFlagList = embFlagList_add(pathObjFlagList, svgPathCmdToEmbPathFlag(cmd));
                            }
                            lx = xx; ly = yy;

                            pathbuff[0] = (char)cmd;                  /* set the command for compare */
                            pathbuff[1] = 0;
                            pos = 0;

                            printf("*prior:%s (%f, %f,  %f, %f,     %f,%f,  %f) \n", pathbuff,
                                   pathData[0],
                                   pathData[1],
                                   pathData[2],
                                   pathData[3],
                                   pathData[4],
                                   pathData[5],
                                   pathData[6]
                                   );

                    }

                    /* assign new command */
                    if(trip == -1 && reset == -1)
                    {
                        pathbuff[0] = (char)c;                  /* set the command for compare */
                        pathbuff[1] = 0;

                        printf("cmd:%s\n", pathbuff);
                        if     (!strcmp(pathbuff, "M")) { cmd = 'M'; reset = 2; numMoves++; }
                        else if(!strcmp(pathbuff, "m")) { cmd = 'm'; reset = 2; numMoves++; }
                        else if(!strcmp(pathbuff, "L")) { cmd = 'L'; reset = 2; }
                        else if(!strcmp(pathbuff, "l")) { cmd = 'l'; reset = 2; }
                        else if(!strcmp(pathbuff, "C")) { cmd = 'C'; reset = 6; }
                        else if(!strcmp(pathbuff, "c")) { cmd = 'c'; reset = 6; }
                        else if(!strcmp(pathbuff, "H")) { cmd = 'H'; reset = 1; }
                        else if(!strcmp(pathbuff, "h")) { cmd = 'h'; reset = 1; }
                        else if(!strcmp(pathbuff, "V")) { cmd = 'V'; reset = 1; }
                        else if(!strcmp(pathbuff, "v")) { cmd = 'v'; reset = 1; }
                        else if(!strcmp(pathbuff, "S")) { cmd = 'S'; reset = 4; }
                        else if(!strcmp(pathbuff, "s")) { cmd = 's'; reset = 4; }
                        else if(!strcmp(pathbuff, "Q")) { cmd = 'Q'; reset = 4; }
                        else if(!strcmp(pathbuff, "q")) { cmd = 'q'; reset = 4; }
                        else if(!strcmp(pathbuff, "T")) { cmd = 'T'; reset = 2; }
                        else if(!strcmp(pathbuff, "t")) { cmd = 't'; reset = 2; }
                        else if(!strcmp(pathbuff, "A")) { cmd = 'A'; reset = 7; }
                        else if(!strcmp(pathbuff, "a")) { cmd = 'a'; reset = 7; }
                        else if(!strcmp(pathbuff, "Z")) { cmd = 'Z'; reset = 0; }
                        else if(!strcmp(pathbuff, "z")) { cmd = 'z'; reset = 0; }
                        else {
                            embLog_error("format-svg.c svgAddToPattern(), %s is not a valid svg path command, skipping...\n", pathbuff);
                            trip = -1;
                            reset = -1;
                        }
                    }
                    /* avoid loosing 'z' command that maybe never accomodated. */
                        if (i==last-1) {
                            trip = 2;
                        }
                    } /* while pendingTask */


                    break;
            }
            if(pos >= size - 1)
            {
                /* increase pathbuff length - leave room for 0 */
                size *= 2;
                pathbuff = (char*)realloc(pathbuff, size);
                if(!pathbuff) { embLog_error("format-svg.c svgAddToPattern(), cannot re-allocate memory for pathbuff\n"); return; }
            }
        }
        free(pathbuff);
        pathbuff = 0;

        /* TODO: subdivide numMoves > 1 */

        embPattern_addPathObjectAbs(p, embPathObject_create(startOfPointList, startOfFlagList, svgColorToEmbColor(svgAttribute_getValue(currentElement, "stroke")), 1));
    }
    else if(!strcmp(buff, "polygon") ||
            !strcmp(buff, "polyline"))
    {
        char* pointStr = svgAttribute_getValue(currentElement, "points");
        int last = strlen(pointStr);
        int size = 32;
        int i = 0;
        int c = 0;
        int pos = 0;
        unsigned char odd = 1;
        double xx = 0.0;
        double yy = 0.0;

        EmbPointList* startOfPointList = 0;
        EmbPointList* polyObjPointList = 0;

        char* polybuff = 0;
        polybuff = (char*)malloc(size);
        if(!polybuff) { embLog_error("format-svg.c svgAddToPattern(), cannot allocate memory for polybuff\n"); return; }

        for(i = 0; i < last; i++)
        {
            char c = pointStr[i];
            switch(c)
            {
                case ' ':
                    if(pos == 0)
                        break;
                    polybuff[pos] = 0;
                    pos = 0;
                    /*Compose Point List */
                    if(odd)
                    {
                        odd = 0;
                        xx = atof(polybuff);
                    }
                    else
                    {
                        odd = 1;
                        yy = atof(polybuff);

                        if(!polyObjPointList)
                        {
                            polyObjPointList = embPointList_create(xx, yy);
                            startOfPointList = polyObjPointList;
                        }
                        else
                        {
                            polyObjPointList = embPointList_add(polyObjPointList, embPoint_make(xx, yy));
                        }
                    }

                    break;
                default:
                    polybuff[pos++] = (char)c;
                    break;
            }
            if(pos >= size - 1)
            {
                /* increase polybuff length - leave room for 0 */
                size *= 2;
                polybuff = (char*)realloc(polybuff, size);
                if(!polybuff) { embLog_error("format-svg.c svgAddToPattern(), cannot re-allocate memory for polybuff\n"); return; }
            }
        }
        free(polybuff);
        polybuff = 0;

        if(!strcmp(buff, "polygon"))
        {
            EmbPolygonObject* polygonObj = embPolygonObject_create(startOfPointList, svgColorToEmbColor(svgAttribute_getValue(currentElement, "stroke")), 1); /* TODO: use lineType enum */
            embPattern_addPolygonObjectAbs(p, polygonObj);
        }
        else /* polyline */
        {
            EmbPolylineObject* polylineObj = embPolylineObject_create(startOfPointList, svgColorToEmbColor(svgAttribute_getValue(currentElement, "stroke")), 1); /* TODO: use lineType enum */
            embPattern_addPolylineObjectAbs(p, polylineObj);
        }
    }
    else if(!strcmp(buff, "prefetch"))         {  }
    else if(!strcmp(buff, "radialGradient"))   {  }
    else if(!strcmp(buff, "rect"))
    {
        embPattern_addRectObjectAbs(p, atof(svgAttribute_getValue(currentElement, "x")),
                                       atof(svgAttribute_getValue(currentElement, "y")),
                                       atof(svgAttribute_getValue(currentElement, "width")),
                                       atof(svgAttribute_getValue(currentElement, "height")));
    }
    else if(!strcmp(buff, "script"))           {  }
    else if(!strcmp(buff, "set"))              {  }
    else if(!strcmp(buff, "solidColor"))       {  }
    else if(!strcmp(buff, "stop"))             {  }
    else if(!strcmp(buff, "svg"))              {  }
    else if(!strcmp(buff, "switch"))           {  }
    else if(!strcmp(buff, "tbreak"))           {  }
    else if(!strcmp(buff, "text"))             {  }
    else if(!strcmp(buff, "textArea"))         {  }
    else if(!strcmp(buff, "title"))            {  }
    else if(!strcmp(buff, "tspan"))            {  }
    else if(!strcmp(buff, "use"))              {  }
    else if(!strcmp(buff, "video"))            {  }

    svgElement_free(currentElement);
    currentElement = 0;
}


int svgIsElement(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "?xml"))                { type = SVG_NULL; } /* TODO: Fix the xml version ? messing with svg version */
    else if(!strcmp(buff, "a"))                   { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "altGlyph"))            { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "altGlyphDef"))         { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "altGlyphItem"))        { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "animate"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animateColor"))        { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animateMotion"))       { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animateTransform"))    { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "animation"))           { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "audio"))               { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "circle"))              { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "clipPath"))            { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "color-profile"))       { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "cursor"))              { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "defs"))                { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "desc"))                { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "discard"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "ellipse"))             { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "feBlend"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feColorMatrix"))       { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feComponentTransfer")) { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feComposite"))         { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feConvolveMatrix"))    { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feDiffuseLighting"))   { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feDisplacementMap"))   { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feDistantLight"))      { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feFlood"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feFuncA"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feFuncB"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feFuncG"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feFuncR"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feGaussianBlur"))      { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feImage"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feMerge"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feMergeNode"))         { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feMorphology"))        { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feOffset"))            { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "fePointLight"))        { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feSpecularLighting"))  { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feSpotLight"))         { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feTile"))              { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "feTurbulence"))        { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "filter"))              { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "font"))                { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "font-face"))           { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "font-face-format"))    { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "font-face-name"))      { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "font-face-src"))       { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "font-face-uri"))       { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "foreignObject"))       { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "g"))                   { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "glyph"))               { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "glyphRef"))            { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "handler"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "hkern"))               { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "image"))               { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "line"))                { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "linearGradient"))      { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "listener"))            { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "marker"))              { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "mask"))                { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "metadata"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "missing-glyph"))       { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "mpath"))               { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "path"))                { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "pattern"))             { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "polygon"))             { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "polyline"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "prefetch"))            { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "radialGradient"))      { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "rect"))                { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "script"))              { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "set"))                 { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "solidColor"))          { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "stop"))                { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "style"))               { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "svg"))                 { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "switch"))              { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "symbol"))              { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "tbreak"))              { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "text"))                { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "textArea"))            { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "textPath"))            { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "title"))               { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "tref"))                { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
    else if(!strcmp(buff, "tspan"))               { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "use"))                 { type = SVG_ELEMENT; }
    else if(!strcmp(buff, "video"))               { type = SVG_ELEMENT; }
 /* else if(!strcmp(buff, "view"))                { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */
 /* else if(!strcmp(buff, "vkern"))               { type = SVG_ELEMENT; } TODO: SVG Full 1.1 Spec Element */

    /* Attempt to identify the program that created the SVG file. This should be in a comment at that occurs before the svg element. */
    else if(!strcmp(buff, "Embroidermodder"))     { type = SVG_NULL; svgCreator = SVG_CREATOR_EMBROIDERMODDER; }
    else if(!strcmp(buff, "Illustrator"))         { type = SVG_NULL; svgCreator = SVG_CREATOR_ILLUSTRATOR; }
    else if(!strcmp(buff, "Inkscape"))            { type = SVG_NULL; svgCreator = SVG_CREATOR_INKSCAPE; }

    else                                          { type = SVG_NULL; }
    return type;
}

int svgIsMediaProperty(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "audio-level"))           { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "buffered-rendering"))    { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "display"))               { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "image-rendering"))       { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "pointer-events"))        { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "shape-rendering"))       { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "text-rendering"))        { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "viewport-fill"))         { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "viewport-fill-opacity")) { type = SVG_MEDIA_PROPERTY; }
    else if(!strcmp(buff, "visibility"))            { type = SVG_MEDIA_PROPERTY; }
    return type;
}

int svgIsProperty(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "audio-level"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "buffered-rendering"))    { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "color"))                 { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "color-rendering"))       { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "direction"))             { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "display"))               { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "display-align"))         { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "fill"))                  { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "fill-opacity"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "fill-rule"))             { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-family"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-size"))             { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-style"))            { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-variant"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "font-weight"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "image-rendering"))       { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "line-increment"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "opacity"))               { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "pointer-events"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "shape-rendering"))       { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "solid-color"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "solid-opacity"))         { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stop-color"))            { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stop-opacity"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke"))                { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-dasharray"))      { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-linecap"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-linejoin"))       { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-miterlimit"))     { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-opacity"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "stroke-width"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "text-align"))            { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "text-anchor"))           { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "text-rendering"))        { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "unicode-bidi"))          { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "vector-effect"))         { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "viewport-fill"))         { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "viewport-fill-opacity")) { type = SVG_PROPERTY; }
    else if(!strcmp(buff, "visibility"))            { type = SVG_PROPERTY; }
    return type;
}

int svgIsXmlAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "encoding"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "standalone")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "version"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))          { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsXmlAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsLinkAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "target"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsLinkAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsAnimateAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accumulate"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "additive"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeName"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeType"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "by"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "calcMode"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "from"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keySplines"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyTimes"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "values"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsAnimateAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsAnimateColorAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accumulate"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "additive"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeName"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeType"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "by"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "calcMode"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "from"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keySplines"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyTimes"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "values"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsAnimateColorAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsAnimateMotionAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accumulate"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "additive"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "by"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "calcMode"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "from"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyPoints"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keySplines"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyTimes"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "origin"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "path"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rotate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "values"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsAnimateMotionAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsAnimateTransformAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accumulate"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "additive"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeName"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeType"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "by"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "calcMode"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "from"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keySplines"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "keyTimes"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "values"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsAnimateTransformAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsAnimationAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "initialVisibility"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncBehavior"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncMaster"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncTolerance"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsAnimationAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsAudioAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncBehavior"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncMaster"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncTolerance"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsAudioAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsCircleAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cx"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cy"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "r"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsCircleAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsDefsAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsDefsAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsDescAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsDescAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsDiscardAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsDiscardAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsEllipseAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cx"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cy"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rx"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ry"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsEllipseAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsFontAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "horiz-adv-x"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "horiz-origin-x"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsFontAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsFontFaceAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "accent-height"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "alphabetic"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ascent"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "bbox"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cap-height"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "descent"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-family"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-stretch"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-style"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-variant"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "font-weight"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "hanging"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ideographic"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mathematical"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "overline-position"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "overline-thickness"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "panose-1"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "slope"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "stemh"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "stemv"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "strikethrough-position"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "strikethrough-thickness"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "underline-position"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "underline-thickness"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "unicode-range"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "units-per-em"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "widths"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x-height"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsFontFaceAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsFontFaceSrcAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsFontFaceSrcAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsFontFaceUriAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsFontFaceUriAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsForeignObjectAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsForeignObjectAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsGroupAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsGroupAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsGlyphAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "arabic-form")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "d"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "glyph-name"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "horiz-adv-x")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "lang"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "unicode"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))           { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsGlyphAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsHandlerAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ev:event"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsHandlerAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsHKernAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "g1"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "g2"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "k"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "u1"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "u2"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsHKernAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsImageAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "opacity"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsImageAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsLineAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x1"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x2"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y1"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y2"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsLineAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsLinearGradientAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "gradientUnits")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x1"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x2"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y1"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y2"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))             { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsLinearGradientAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsListenerAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "defaultAction")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "event"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "handler"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "observer"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "phase"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "propagate"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "target"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))             { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsListenerAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsMetadataAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsMetadataAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsMissingGlyphAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "d"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "horiz-adv-x")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))           { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsMissingGlyphAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsMPathAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))             { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsMPathAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsPathAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "d"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "pathLength"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsPathAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsPolygonAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "points"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsPolygonAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsPolylineAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "points"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsPolylineAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsPrefetchAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "bandwidth"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mediaCharacterEncoding")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mediaContentEncodings"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mediaSize"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "mediaTime"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                      { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsPrefetchAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsRadialGradientAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cx"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "cy"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "gradientUnits")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "r"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))             { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsRadialGradientAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsRectAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rx"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "ry"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsRectAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsScriptAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsScriptAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsSetAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeName"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "attributeType"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "to"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsSetAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsSolidColorAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsSolidColorAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsStopAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "offset"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsStopAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsSvgAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "baseProfile"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "contentScriptType"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "playbackOrder"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "snapshotTime"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncBehaviorDefault"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncToleranceDefault"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "timelineBegin"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "version"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "viewBox"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "zoomAndPan"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }
    else if(svgCreator == SVG_CREATOR_INKSCAPE)
    {
        if     (!strcmp(buff, "xmlns:dc"))              { type = SVG_ATTRIBUTE; }
        else if(!strcmp(buff, "xmlns:cc"))              { type = SVG_ATTRIBUTE; }
        else if(!strcmp(buff, "xmlns:rdf"))             { type = SVG_ATTRIBUTE; }
        else if(!strcmp(buff, "xmlns:svg"))             { type = SVG_ATTRIBUTE; }
        else if(!strcmp(buff, "xmlns"))                 { type = SVG_ATTRIBUTE; }
    }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsSvgAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsSwitchAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsSwitchAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsTBreakAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsTBreakAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsTextAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "editable"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rotate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsTextAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsTextAreaAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "editable"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsTextAreaAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsTitleAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsTitleAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsTSpanAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                  { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsTSpanAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsUseAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsUseAttribute(), unknown: %s\n", buff); }
    return type;
}

int svgIsVideoAttribute(const char* buff)
{
    int type = SVG_NULL;
    if     (!strcmp(buff, "about"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "begin"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "class"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "content"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "dur"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "end"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "fill"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "height"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "id"))                        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "initialVisibility"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "max"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "min"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "overlay"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "property"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatCount"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "repeatDur"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "restart"))                   { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "role"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncBehavior"))              { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncMaster"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "syncTolerance"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "transformBehavior"))         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "type"))                      { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "width"))                     { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "x"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "y"))                         { type = SVG_ATTRIBUTE; }
    else if(!strcmp(buff, "/"))                         { type = SVG_ATTRIBUTE; }

    if(type == SVG_NULL) { embLog_print("format-svg.c svgIsVideoAttribute(), unknown: %s\n", buff); }
    return type;
}


int svgIsCatchAllAttribute(const char* buff)
{
    int type = SVG_NULL;
    /* Catch All Properties */
    if     (!strcmp(buff, "audio-level"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "buffered-rendering"))        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "color"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "color-rendering"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "direction"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "display"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "display-align"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "fill"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "fill-opacity"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "fill-rule"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-family"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-size"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-style"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-variant"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-weight"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "image-rendering"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "line-increment"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "opacity"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "pointer-events"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "shape-rendering"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "solid-color"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "solid-opacity"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stop-color"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stop-opacity"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-dasharray"))          { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-linecap"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-linejoin"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-miterlimit"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-opacity"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stroke-width"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "text-align"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "text-anchor"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "text-rendering"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "unicode-bidi"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "vector-effect"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "viewport-fill"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "viewport-fill-opacity"))     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "visibility"))                { type = SVG_CATCH_ALL; }
    /* Catch All Attributes */
    else if(!strcmp(buff, "about"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "accent-height"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "accumulate"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "additive"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "alphabetic"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "arabic-form"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "ascent"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "attributeName"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "attributeType"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "bandwidth"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "baseProfile"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "bbox"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "begin"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "by"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "calcMode"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "cap-height"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "class"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "content"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "contentScriptType"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "cx"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "cy"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "d"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "datatype"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "defaultAction"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "descent"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "dur"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "editable"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "end"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "ev:event"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "event"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "externalResourcesRequired")) { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "focusHighlight"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "focusable"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-family"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-stretch"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-style"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-variant"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "font-weight"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "from"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "g1"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "g2"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "glyph-name"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "gradientUnits"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "handler"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "hanging"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "height"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "horiz-adv-x"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "horiz-origin-x"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "id"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "ideographic"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "initialVisibility"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "k"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "keyPoints"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "keySplines"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "keyTimes"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "lang"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mathematical"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "max"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mediaCharacterEncoding"))    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mediaContentEncodings"))     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mediaSize"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "mediaTime"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "min"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-down"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-down-left"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-down-right"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-left"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-next"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-prev"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-right"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-up"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-up-left"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "nav-up-right"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "observer"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "offset"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "origin"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "overlay"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "overline-position"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "overline-thickness"))        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "panose-1"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "path"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "pathLength"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "phase"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "playbackOrder"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "points"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "preserveAspectRatio"))       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "propagate"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "property"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "r"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "rel"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "repeatCount"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "repeatDur"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "requiredExtensions"))        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "requiredFeatures"))          { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "requiredFonts"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "requiredFormats"))           { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "resource"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "restart"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "rev"))                       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "role"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "rotate"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "rx"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "ry"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "slope"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "snapshotTime"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stemh"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "stemv"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "strikethrough-position"))    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "strikethrough-thickness"))   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncBehavior"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncBehaviorDefault"))       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncMaster"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncTolerance"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "syncToleranceDefault"))      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "systemLanguage"))            { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "target"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "timelineBegin"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "to"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "transform"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "transformBehavior"))         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "type"))                      { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "typeof"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "u1"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "u2"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "underline-position"))        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "underline-thickness"))       { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "unicode"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "unicode-range"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "units-per-em"))              { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "values"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "version"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "viewBox"))                   { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "width"))                     { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "widths"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "x"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "x-height"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "x1"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "x2"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:actuate"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:arcrole"))             { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:href"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:role"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:show"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:title"))               { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xlink:type"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xml:base"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xml:id"))                    { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xml:lang"))                  { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "xml:space"))                 { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "y"))                         { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "y1"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "y2"))                        { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "zoomAndPan"))                { type = SVG_CATCH_ALL; }
    else if(!strcmp(buff, "/"))                         { type = SVG_CATCH_ALL; }
    return type;
}

void svgProcess(int c, const char* buff)
{
    if(svgExpect == SVG_EXPECT_ELEMENT)
    {
        char advance = 0;
        if(buff[0] == '/') { return; }

        advance = (char)svgIsElement(buff);

        if(advance) { printf("ELEMENT:\n"); svgExpect = SVG_EXPECT_ATTRIBUTE; currentElement = svgElement_create(buff); }
        else        { return; }
    }
    else if(svgExpect == SVG_EXPECT_ATTRIBUTE)
    {
        char advance = 0;
        const char* name = 0;
        if(!currentElement) { /* TODO: error */ return; }
        name = currentElement->name;

        if(!strcmp(name, "?xml"))
        {
            if(!advance) { advance = (char)svgIsXmlAttribute(buff); }
        }
        else if(!strcmp(name, "a"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsLinkAttribute(buff); }
        }
        else if(!strcmp(name, "animate"))
        {
            if(!advance) { advance = (char)svgIsAnimateAttribute(buff); }
        }
        else if(!strcmp(name, "animateColor"))
        {
            if(!advance) { advance = (char)svgIsAnimateColorAttribute(buff); }
        }
        else if(!strcmp(name, "animateMotion"))
        {
            if(!advance) { advance = (char)svgIsAnimateMotionAttribute(buff); }
        }
        else if(!strcmp(name, "animateTransform"))
        {
            if(!advance) { advance = (char)svgIsAnimateTransformAttribute(buff); }
        }
        else if(!strcmp(name, "animation"))
        {
            if(!advance) { advance = (char)svgIsMediaProperty(buff); }
            if(!advance) { advance = (char)svgIsAnimationAttribute(buff); }
        }
        else if(!strcmp(name, "audio"))
        {
            if(!advance) { advance = (char)svgIsMediaProperty(buff); }
            if(!advance) { advance = (char)svgIsAudioAttribute(buff); }
        }
        else if(!strcmp(name, "circle"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsCircleAttribute(buff); }
        }
        else if(!strcmp(name, "defs"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsDefsAttribute(buff); }
        }
        else if(!strcmp(name, "desc"))
        {
            if(!advance) { advance = (char)svgIsMediaProperty(buff); }
            if(!advance) { advance = (char)svgIsDescAttribute(buff); }
        }
        else if(!strcmp(name, "discard"))
        {
            if(!advance) { advance = (char)svgIsDiscardAttribute(buff); }
        }
        else if(!strcmp(name, "ellipse"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsEllipseAttribute(buff); }
        }
        else if(!strcmp(name, "font"))
        {
            if(!advance) { advance = (char)svgIsFontAttribute(buff); }
        }
        else if(!strcmp(name, "font-face"))
        {
            if(!advance) { advance = (char)svgIsFontFaceAttribute(buff); }
        }
        else if(!strcmp(name, "font-face-src"))
        {
            if(!advance) { advance = (char)svgIsFontFaceSrcAttribute(buff); }
        }
        else if(!strcmp(name, "font-face-uri"))
        {
            if(!advance) { advance = (char)svgIsFontFaceUriAttribute(buff); }
        }
        else if(!strcmp(name, "foreignObject"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsForeignObjectAttribute(buff); }
        }
        else if(!strcmp(name, "g"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsGroupAttribute(buff); }
        }
        else if(!strcmp(name, "glyph"))
        {
            if(!advance) { advance = (char)svgIsGlyphAttribute(buff); }
        }
        else if(!strcmp(name, "handler"))
        {
            if(!advance) { advance = (char)svgIsHandlerAttribute(buff); }
        }
        else if(!strcmp(name, "hkern"))
        {
            if(!advance) { advance = (char)svgIsHKernAttribute(buff); }
        }
        else if(!strcmp(name, "image"))
        {
            if(!advance) { advance = (char)svgIsMediaProperty(buff); }
            if(!advance) { advance = (char)svgIsImageAttribute(buff); }
        }
        else if(!strcmp(name, "line"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsLineAttribute(buff); }
        }
        else if(!strcmp(name, "linearGradient"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsLinearGradientAttribute(buff); }
        }
        else if(!strcmp(name, "listener"))
        {
            if(!advance) { advance = (char)svgIsListenerAttribute(buff); }
        }
        else if(!strcmp(name, "metadata"))
        {
            if(!advance) { advance = (char)svgIsMediaProperty(buff); }
            if(!advance) { advance = (char)svgIsMetadataAttribute(buff); }
        }
        else if(!strcmp(name, "missing-glyph"))
        {
            if(!advance) { advance = (char)svgIsMissingGlyphAttribute(buff); }
        }
        else if(!strcmp(name, "mpath"))
        {
            if(!advance) { advance = (char)svgIsMPathAttribute(buff); }
        }
        else if(!strcmp(name, "path"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsPathAttribute(buff); }
        }
        else if(!strcmp(name, "polygon"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsPolygonAttribute(buff); }
        }
        else if(!strcmp(name, "polyline"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsPolylineAttribute(buff); }
        }
        else if(!strcmp(name, "prefetch"))
        {
            if(!advance) { advance = (char)svgIsPrefetchAttribute(buff); }
        }
        else if(!strcmp(name, "radialGradient"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsRadialGradientAttribute(buff); }
        }
        else if(!strcmp(name, "rect"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsRectAttribute(buff); }
        }
        else if(!strcmp(name, "script"))
        {
            if(!advance) { advance = (char)svgIsScriptAttribute(buff); }
        }
        else if(!strcmp(name, "set"))
        {
            if(!advance) { advance = (char)svgIsSetAttribute(buff); }
        }
        else if(!strcmp(name, "solidColor"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsSolidColorAttribute(buff); }
        }
        else if(!strcmp(name, "stop"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsStopAttribute(buff); }
        }
        else if(!strcmp(name, "svg"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsSvgAttribute(buff); }
        }
        else if(!strcmp(name, "switch"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsSwitchAttribute(buff); }
        }
        else if(!strcmp(name, "tbreak"))
        {
            if(!advance) { advance = (char)svgIsTBreakAttribute(buff); }
        }
        else if(!strcmp(name, "text"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsTextAttribute(buff); }
        }
        else if(!strcmp(name, "textArea"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsTextAreaAttribute(buff); }
        }
        else if(!strcmp(name, "title"))
        {
            if(!advance) { advance = (char)svgIsMediaProperty(buff); }
            if(!advance) { advance = (char)svgIsTitleAttribute(buff); }
        }
        else if(!strcmp(name, "tspan"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsTSpanAttribute(buff); }
        }
        else if(!strcmp(name, "use"))
        {
            if(!advance) { advance = (char)svgIsProperty(buff); }
            if(!advance) { advance = (char)svgIsUseAttribute(buff); }
        }
        else if(!strcmp(name, "video"))
        {
            if(!advance) { advance = (char)svgIsMediaProperty(buff); }
            if(!advance) { advance = (char)svgIsVideoAttribute(buff); }
        }

        if(advance)
        {
            printf("ATTRIBUTE:\n");
            svgExpect = SVG_EXPECT_VALUE;
            free(currentAttribute);
            currentAttribute = 0;
            currentAttribute = emb_strdup(buff);
        }
    }
    else if(svgExpect == SVG_EXPECT_VALUE)
    {
        int last = strlen(buff) - 1;
        printf("VALUE:\n");

        /* single-value */
        if((buff[0] == '"' || buff[0] == '\'') && (buff[last] == '/' || buff[last] == '"' || buff[last] == '\'') && !svgMultiValue)
        {
            svgExpect = SVG_EXPECT_ATTRIBUTE;
            svgElement_addAttribute(currentElement, svgAttribute_create(currentAttribute, buff));
        }
        else /* multi-value */
        {
            svgMultiValue = 1;
            if(!currentValue)
            {
                currentValue = emb_strdup(buff);
                if(!currentValue) { /*TODO: error */ return; }
            }
            else
            {
                char* tmp = 0;
                tmp = emb_strdup(currentValue);
                free(currentValue);
                currentValue = 0;
                currentValue = (char*)malloc(strlen(buff) + strlen(tmp) + 2);
                if(!currentValue) { embLog_error("format-svg.c svgProcess(), cannot allocate memory for currentValue\n"); return; }
                if(currentValue) memset(currentValue, 0, strlen(buff) + strlen(tmp) + 2);
                strcat(currentValue, tmp);
                strcat(currentValue, " ");
                strcat(currentValue, buff);
                free(tmp);
                tmp = 0;
            }

            if(buff[last] == '/' || buff[last] == '"' || buff[last] == '\'')
            {
                svgMultiValue = 0;
                svgExpect = SVG_EXPECT_ATTRIBUTE;
                svgElement_addAttribute(currentElement, svgAttribute_create(currentAttribute, currentValue));
                free(currentValue);
                currentValue = 0;
            }
        }
    }

    if(svgExpect != SVG_EXPECT_NULL)
        printf("%s\n", buff);

    if(c == '>')
        svgExpect = SVG_EXPECT_NULL;
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int readSvg(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;
    int size = 1024;
    int pos;
    int c = 0;
    EmbRectObjectList* rList = 0;
    EmbCircleObjectList* cList = 0;
    EmbEllipseObjectList* eList = 0;
    EmbLineObjectList* liList = 0;
    EmbPointObjectList* poList = 0;
    EmbPolygonObjectList* pogList = 0;
    EmbPolylineObjectList* polList = 0;
    char* buff = 0;

    if(!pattern) { embLog_error("format-svg.c readSvg(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-svg.c readSvg(), fileName argument is null\n"); return 0; }

    buff = (char*)malloc(size);
    if(!buff) { embLog_error("format-svg.c readSvg(), cannot allocate memory for buff\n"); return 0; }

    svgCreator = SVG_CREATOR_NULL;

    svgExpect = SVG_EXPECT_NULL;
    svgMultiValue = 0;

    currentElement = 0;
    currentAttribute = 0;
    currentValue = 0;

    /* Pre-flip incase of multiple reads on the same pattern */
    embPattern_flipVertical(pattern);

    file = embFile_open(fileName, "r");
    if(file)
    {
        pos = 0;
        do
        {
            c = embFile_getc(file);
            switch(c)
            {
                case '<':
                    if(svgExpect == SVG_EXPECT_NULL)
                    {
                        svgAddToPattern(pattern);
                        svgExpect = SVG_EXPECT_ELEMENT;
                    }
                case '>':
                    if(pos == 0) /* abnormal case that may occur in svg element where '>' is all by itself */
                    {
                        /*TODO: log a warning about this absurdity! */
                        svgExpect = SVG_EXPECT_ELEMENT;
                        break;
                    }
                case ' ':
                case '\t':
                case '\r':
                case '\n':
                case '=':
                    if(pos == 0)
                        break;
                    buff[pos] = 0;
                    pos = 0;
                    svgProcess(c, buff);
                    break;
                default:
                    buff[pos++] = (char)c;
                    break;
            }
            if(pos >= size - 1)
            {
                /* increase buff length - leave room for 0 */
                size *= 2;
                buff = (char*)realloc(buff, size);
                if(!buff) { embLog_error("format-svg.c readSvg(), cannot re-allocate memory for buff\n"); return 0; }
            }
        }
        while(c != EOF);
        embFile_close(file);
    }
    free(buff);
    buff = 0;
    free(currentAttribute);
    currentAttribute = 0;
    free(currentValue);
    currentValue = 0;

    /*TODO: remove this summary after testing is complete */
    printf("OBJECT SUMMARY:\n");
    cList = pattern->circleObjList;
    while(cList)
    {
        EmbCircle c = cList->circleObj.circle;
        printf("circle %f %f %f\n", c.centerX, c.centerY, c.radius);
        cList = cList->next;
    }
    eList = pattern->ellipseObjList;
    while(eList)
    {
        EmbEllipse e = eList->ellipseObj.ellipse;
        printf("ellipse %f %f %f %f\n", embEllipse_centerX(e), embEllipse_centerY(e), embEllipse_radiusX(e), embEllipse_radiusY(e));
        eList = eList->next;
    }
    liList = pattern->lineObjList;
    while(liList)
    {
        EmbLine li = liList->lineObj.line;
        printf("line %f %f %f %f\n", embLine_x1(li), embLine_y1(li), embLine_x2(li), embLine_y2(li));
        liList = liList->next;
    }
    poList = pattern->pointObjList;
    while(poList)
    {
        EmbPoint po = poList->pointObj.point;
        printf("point %f %f\n", embPoint_x(po), embPoint_y(po));
        poList = poList->next;
    }
    pogList = pattern->polygonObjList;
    while(pogList)
    {
        int vertices = embPointList_count(pogList->polygonObj->pointList);
        printf("polygon %d\n", vertices);
        pogList = pogList->next;
    }
    polList = pattern->polylineObjList;
    while(polList)
    {
        int vertices = embPointList_count(polList->polylineObj->pointList);
        printf("polyline %d\n", vertices);
        polList = polList->next;
    }
    rList = pattern->rectObjList;
    while(rList)
    {
        EmbRect r = rList->rectObj.rect;
        printf("rect %f %f %f %f\n", embRect_x(r), embRect_y(r), embRect_width(r), embRect_height(r));
        rList = rList->next;
    }

    /* Flip the pattern since SVG Y+ is down and libembroidery Y+ is up. */
    embPattern_flipVertical(pattern);

    return 1; /*TODO: finish readSvg */
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int writeSvg(EmbPattern* pattern, const char* fileName)
{
    EmbFile* file = 0;
    EmbRect boundingRect;
    EmbStitchList* stList;
    EmbCircleObjectList* cObjList = 0;
    EmbCircle circle;
    EmbEllipseObjectList* eObjList = 0;
    EmbEllipse ellipse;
    EmbLineObjectList* liObjList = 0;
    EmbLine line;
    EmbPointObjectList* poObjList = 0;
    EmbPoint point;
    EmbPolygonObjectList* pogObjList = 0;
    EmbPointList* pogPointList = 0;
    EmbPolylineObjectList* polObjList = 0;
    EmbPointList* polPointList = 0;
    EmbRectObjectList* rObjList = 0;
    EmbRect rect;
    EmbColor color;

    char tmpX[32];
    char tmpY[32];

    if(!pattern) { embLog_error("format-svg.c writeSvg(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("format-svg.c writeSvg(), fileName argument is null\n"); return 0; }

    file = embFile_open(fileName, "w");
    if(!file)
    {
        embLog_error("format-svg.c writeSvg(), cannot open %s for writing\n", fileName);
        return 0;
    }

    /* Pre-flip the pattern since SVG Y+ is down and libembroidery Y+ is up. */
    embPattern_flipVertical(pattern);

    boundingRect = embPattern_calcBoundingBox(pattern);
    embFile_printf(file, "<?xml version=\"1.0\"?>\n");
    embFile_printf(file, "<!-- Embroidermodder 2 SVG Embroidery File -->\n");
    embFile_printf(file, "<!-- http://embroidermodder.github.io -->\n");
    embFile_printf(file, "<svg ");

    /* TODO: See the SVG Tiny Version 1.2 Specification Section 7.14.
    *       Until all of the formats and API is stable, the width, height and viewBox attributes need to be left unspecified.
    *       If the attribute values are incorrect, some applications wont open it at all.
    embFile_printf(file, "viewBox=\"%f %f %f %f\" ",
            boundingRect.left,
            boundingRect.top,
            embRect_width(boundingRect),
            embRect_height(boundingRect)); */

    embFile_printf(file, "xmlns=\"http://www.w3.org/2000/svg\" version=\"1.2\" baseProfile=\"tiny\">");

    /*TODO: Low Priority Optimization:
    *      Using %g in embFile_printf just doesn't work good enough at trimming trailing zeroes.
    *      It's precision refers to significant digits, not decimal places (which is what we want).
    *      We need to roll our own function for trimming trailing zeroes to keep
    *      the precision as high as possible if needed, but help reduce file size also. */

    /*TODO: Low Priority Optimization:
    *      Make sure that the line length that is output doesn't exceed 1000 characters. */

    /*TODO: Low Priority: Indent output properly. */

    /* write circles */
    cObjList = pattern->circleObjList;
    while(cObjList)
    {
        circle = cObjList->circleObj.circle;
        color = cObjList->circleObj.color;
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        embFile_printf(file, "\n<circle stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" cx=\"%f\" cy=\"%f\" r=\"%f\" />",
                        color.r,
                        color.g,
                        color.b,
                        circle.centerX,
                        circle.centerY,
                        circle.radius);
        cObjList = cObjList->next;
    }

    /* write ellipses */
    eObjList = pattern->ellipseObjList;
    while(eObjList)
    {
        ellipse = eObjList->ellipseObj.ellipse;
        color = eObjList->ellipseObj.color;
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        embFile_printf(file, "\n<ellipse stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" cx=\"%f\" cy=\"%f\" rx=\"%f\" ry=\"%f\" />",
                        color.r,
                        color.g,
                        color.b,
                        ellipse.centerX,
                        ellipse.centerY,
                        ellipse.radiusX,
                        ellipse.radiusY);
        eObjList = eObjList->next;
    }

    /* write lines */
    liObjList = pattern->lineObjList;
    while(liObjList)
    {
        line = liObjList->lineObj.line;
        color = liObjList->lineObj.color;
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        embFile_printf(file, "\n<line stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" />",
                        color.r,
                        color.g,
                        color.b,
                        line.x1,
                        line.y1,
                        line.x2,
                        line.y2);
        liObjList = liObjList->next;
    }

    /* write points */
    poObjList = pattern->pointObjList;
    while(poObjList)
    {
        point = poObjList->pointObj.point;
        color = poObjList->pointObj.color;
        /* See SVG Tiny 1.2 Spec:
        * Section 9.5 The 'line' element
        * Section C.6 'path' element implementation notes */
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        embFile_printf(file, "\n<line stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" x1=\"%f\" y1=\"%f\" x2=\"%f\" y2=\"%f\" />",
                        color.r,
                        color.g,
                        color.b,
                        point.xx,
                        point.yy,
                        point.xx,
                        point.yy);
        poObjList = poObjList->next;
    }

    /* write polygons */
    pogObjList = pattern->polygonObjList;
    while(pogObjList)
    {
        pogPointList = pogObjList->polygonObj->pointList;
        if(pogPointList)
        {
            color = pogObjList->polygonObj->color;
            /* TODO: use proper thread width for stoke-width rather than just 0.2 */
            embFile_printf(file, "\n<polygon stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" points=\"%s,%s",
                    color.r,
                    color.g,
                    color.b,
                    emb_optOut(pogPointList->point.xx, tmpX),
                    emb_optOut(pogPointList->point.yy, tmpY));
            pogPointList = pogPointList->next;
            while(pogPointList)
            {
                embFile_printf(file, " %s,%s", emb_optOut(pogPointList->point.xx, tmpX), emb_optOut(pogPointList->point.yy, tmpY));
                pogPointList = pogPointList->next;
            }
            embFile_printf(file, "\"/>");
        }
        pogObjList = pogObjList->next;
    }

    /* write polylines */
    polObjList = pattern->polylineObjList;
    while(polObjList)
    {
        polPointList = polObjList->polylineObj->pointList;
        if(polPointList)
        {
            color = polObjList->polylineObj->color;
            /* TODO: use proper thread width for stoke-width rather than just 0.2 */
            embFile_printf(file, "\n<polyline stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" points=\"%s,%s",
                    color.r,
                    color.g,
                    color.b,
                    emb_optOut(polPointList->point.xx, tmpX),
                    emb_optOut(polPointList->point.yy, tmpY));
            polPointList = polPointList->next;
            while(polPointList)
            {
                embFile_printf(file, " %s,%s", emb_optOut(polPointList->point.xx, tmpX), emb_optOut(polPointList->point.yy, tmpY));
                polPointList = polPointList->next;
            }
            embFile_printf(file, "\"/>");
        }
        polObjList = polObjList->next;
    }

    /* write rects */
    rObjList = pattern->rectObjList;
    while(rObjList)
    {
        rect = rObjList->rectObj.rect;
        color = rObjList->rectObj.color;
        /* TODO: use proper thread width for stoke-width rather than just 0.2 */
        embFile_printf(file, "\n<rect stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" x=\"%f\" y=\"%f\" width=\"%f\" height=\"%f\" />",
                        color.r,
                        color.g,
                        color.b,
                        embRect_x(rect),
                        embRect_y(rect),
                        embRect_width(rect),
                        embRect_height(rect));
        rObjList = rObjList->next;
    }

    stList = pattern->stitchList;
    if(stList)
    {
        /*TODO: #ifdef SVG_DEBUG for Josh which outputs JUMPS/TRIMS instead of chopping them out */
        char isNormal = 0;
        while(stList)
        {
            if(stList->stitch.flags == NORMAL && !isNormal)
            {
                    isNormal = 1;
                    color = embThreadList_getAt(pattern->threadList, stList->stitch.color).color;
                    /* TODO: use proper thread width for stoke-width rather than just 0.2 */
                    embFile_printf(file, "\n<polyline stroke-linejoin=\"round\" stroke-linecap=\"round\" stroke-width=\"0.2\" stroke=\"#%02x%02x%02x\" fill=\"none\" points=\"%s,%s",
                                color.r,
                                color.g,
                                color.b,
                                emb_optOut(stList->stitch.xx, tmpX),
                                emb_optOut(stList->stitch.yy, tmpY));
            }
            else if(stList->stitch.flags == NORMAL && isNormal)
            {
                embFile_printf(file, " %s,%s", emb_optOut(stList->stitch.xx, tmpX), emb_optOut(stList->stitch.yy, tmpY));
            }
            else if(stList->stitch.flags != NORMAL && isNormal)
            {
                isNormal = 0;
                embFile_printf(file, "\"/>");
            }

            stList = stList->next;
        }
    }
    embFile_printf(file, "\n</svg>\n");
    embFile_close(file);

    /* Reset the pattern so future writes(regardless of format) are not flipped */
    embPattern_flipVertical(pattern);

    return 1;
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
