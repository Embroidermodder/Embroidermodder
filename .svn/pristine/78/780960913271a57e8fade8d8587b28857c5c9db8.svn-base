#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "emb-pattern.h"
#include "emb-reader-writer.h"
#include "helpers-misc.h"

EmbPattern* embPattern_create()
{
    EmbPattern* p = 0;
    p = (EmbPattern*)malloc(sizeof(EmbPattern));
    if(!p) { /* TODO: error */ return 0; }
    
    p->settings = embSettings_init();
    p->currentColorIndex = 0;
    p->stitchList = 0;
    p->threadList = 0;

    p->arcObjList = 0;
    p->circleObjList = 0;
    p->ellipseObjList = 0;
    p->lineObjList = 0;
    p->pathObjList = 0;
    p->pointObjList = 0;
    p->polygonObjList = 0;
    p->polylineObjList = 0;
    p->rectObjList = 0;
    p->splineObjList = 0;

    p->lastStitch = 0;
    p->lastThread = 0;

    p->lastArcObj = 0;
    p->lastCircleObj = 0;
    p->lastLineObj = 0;
    p->lastEllipseObj = 0;
    p->lastPathObj = 0;
    p->lastPointObj = 0;
    p->lastPolylineObj = 0;
    p->lastRectObj = 0;
    p->lastSplineObj = 0;

    return p;
}

void embPattern_hideStitchesOverLength(EmbPattern* p, int length)
{
    double prevX = 0;
    double prevY = 0;
    EmbStitchList* pointer;
    pointer = p->stitchList;
    while(pointer)
    {
        if((fabs(pointer->stitch.xx - prevX) > length) || (fabs(pointer->stitch.yy - prevY) > length))
        {
            pointer->stitch.flags |= TRIM;
            pointer->stitch.flags &= ~NORMAL;
        }
        prevX = pointer->stitch.xx;
        prevY = pointer->stitch.yy;
        pointer = pointer->next;
    }
}

void embPattern_addThread(EmbPattern* p, EmbThread thread)
{
    if(!(p->threadList))
    {
        EmbThreadList* t = (EmbThreadList *)malloc(sizeof(EmbThreadList));
        t->thread = thread;
        t->next = NULL;
        p->threadList = t;
    }
    else
    {
        embThread_add(p->threadList, thread);
    }
}

void embPattern_fixColorCount(EmbPattern* p)
{
    /* fix color count to be max of color index. */
    int maxColorIndex = 0;
    EmbStitchList* list = p->stitchList;
    while(list)
    {
        maxColorIndex = max(maxColorIndex, list->stitch.color);
        list = list->next;
    }
    while((int)embThread_count(p->threadList) <= maxColorIndex)
    {
        embPattern_addThread(p, embThread_getRandom());
    }
    /*
    while(embThread_count(p->threadList) > (maxColorIndex + 1))
    {
        TODO: erase last color    p->threadList.pop_back();
    }
    */
}

/* Three routines to add a stitch to the pattern depending on format... */
void embPattern_addStitchAbs(EmbPattern* p, double x, double y, int flags, int isAutoColorIndex)
{
    EmbStitch s;

    if(flags & END)
    {
        embPattern_fixColorCount(p);
        /* HideStitchesOverLength(127); */
    }

    if((flags & STOP) && embStitch_empty(p->stitchList))
        return;
    if((flags & STOP) && isAutoColorIndex)
    {
        p->currentColorIndex++;
    }

    s.xx = x;
    s.yy = y;
    s.flags = flags;
    s.color = p->currentColorIndex;
    if(!(p->stitchList))
    {
        p->stitchList = (EmbStitchList*)malloc(sizeof(EmbStitchList));
        p->stitchList->stitch = s;
        p->stitchList->next = 0;
        p->lastStitch = p->stitchList;
    }
    else
    {
        embStitch_add(p->lastStitch, s);
        p->lastStitch = p->lastStitch->next;
    }
    p->lastX = s.xx;
    p->lastY = s.yy;
}

void embPattern_addStitchRel(EmbPattern* p, double dx, double dy, int flags, int isAutoColorIndex)
{
    double x,y;
    if(!embStitch_empty(p->stitchList))
    {
        x = p->lastX + dx;
        y = p->lastY + dy;
    }
    else
    {
        /* the list is empty so assume starting location is 0,0 */
        x = dx;
        y = dy;
    }
    embPattern_addStitchAbs(p, x, y, flags, isAutoColorIndex);
}

void embPattern_changeColor(EmbPattern* p, int index)
{
    p->currentColorIndex = index;
}

EmbPattern* embPattern_read(const char* fileName)
{
    EmbPattern* p = (EmbPattern*)malloc(sizeof(EmbPattern));
    EmbReaderWriter* reader;
    reader = embReaderWriter_getByFileName(fileName);
    if(reader->reader(p, fileName))
    {
        return p;
    }
    return NULL;
}

int embPattern_write(EmbPattern* p, const char *fileName)
{
    EmbReaderWriter* writer;
    writer = embReaderWriter_getByFileName(fileName);
    if(writer->writer(p, fileName))
    {
        return 1;
    }
    return 0;
}

/* Very simple scaling of the x and y axis for every point.
* Doesn't insert or delete stitches to preserve density. */
void embPattern_scale(EmbPattern* p, double scale)
{
    EmbStitchList* pointer;
    pointer = p->stitchList;
    while(pointer)
    {
        pointer->stitch.xx *= scale;
        pointer->stitch.yy *= scale;
        pointer = pointer->next;
    }
}

EmbRect embPattern_calcBoundingBox(EmbPattern* p)
{
    EmbStitchList* pointer;
    EmbRect BoundingRect;
    EmbStitch pt;
    EmbCircleObjectList* cObjList;
    EmbCircle circle;
    EmbArcObjectList* aObjList;
    EmbArc arc;
    EmbEllipseObjectList* eObjList;
    EmbEllipse ellipse;
    EmbLineObjectList* liObjList;
    EmbLine line;
    EmbPointObjectList* pObjList;
    EmbPoint point;
    EmbPolygonObjectList* pogObjList;
    EmbPoint pogPoint;
    EmbPolylineObjectList* polObjList;
    EmbPoint polPoint;
    EmbRectObjectList* rObjList;
    EmbRect rect;
    EmbSplineObjectList* sObjList;
    EmbBezier bezier;
    /* Calculate the bounding rectangle.  It's needed for smart repainting. */
    /* TODO: Come back and optimize this mess so that after going thru all objects
            and stitches, if the rectangle isn't reasonable, then return a default rect */
    if(embStitch_empty(p->stitchList) &&
    embArc_empty(p->arcObjList) &&
    embCircle_empty(p->circleObjList) &&
    embEllipse_empty(p->ellipseObjList) &&
    embLine_empty(p->lineObjList) &&
    embPoint_empty(p->pointObjList) &&
    embPolygon_empty(p->polygonObjList) &&
    embPolyline_empty(p->polylineObjList) &&
    embRect_empty(p->rectObjList) &&
    embSpline_empty(p->splineObjList))
    {
        BoundingRect.top = 0.0;
        BoundingRect.left = 0.0;
        BoundingRect.bottom = 1.0;
        BoundingRect.right = 1.0;
        return BoundingRect;
    }
    BoundingRect.left = 99999.0;
    BoundingRect.top =  99999.0;
    BoundingRect.right = -99999.0;
    BoundingRect.bottom = -99999.0;

    pointer = p->stitchList;
    while(pointer)
    {
        /* If the point lies outside of the accumulated bounding
        * rectangle, then inflate the bounding rect to include it. */
        pt = pointer->stitch;
        if(!(pt.flags & TRIM))
        {
            BoundingRect.left = (double)min(BoundingRect.left, pt.xx);
            BoundingRect.top = (double)min(BoundingRect.top, pt.yy);
            BoundingRect.right = (double)max(BoundingRect.right, pt.xx);
            BoundingRect.bottom = (double)max(BoundingRect.bottom, pt.yy);
        }
        pointer = pointer->next;
    }

    aObjList = p->arcObjList;
    while(aObjList)
    {
        arc = aObjList->arcObj.arc;
        /* TODO: embPattern_calcBoundingBox for arcs */

        aObjList = aObjList->next;
    }

    cObjList = p->circleObjList;
    while(cObjList)
    {
        circle = cObjList->circleObj.circle;
        BoundingRect.left = (double)min(BoundingRect.left, circle.centerX - circle.radius);
        BoundingRect.top = (double)min(BoundingRect.top, circle.centerY - circle.radius);
        BoundingRect.right = (double)max(BoundingRect.right, circle.centerX + circle.radius);
        BoundingRect.bottom = (double)max(BoundingRect.bottom, circle.centerY + circle.radius);

        cObjList = cObjList->next;
    }

    eObjList = p->ellipseObjList;
    while(eObjList)
    {
        ellipse = eObjList->ellipseObj.ellipse;
        /* TODO: embPattern_calcBoundingBox for ellipses */

        eObjList = eObjList->next;
    }

    liObjList = p->lineObjList;
    while(liObjList)
    {
        line = liObjList->lineObj.line;
        /* TODO: embPattern_calcBoundingBox for lines */

        liObjList = liObjList->next;
    }

    pObjList = p->pointObjList;
    while(pObjList)
    {
        point = pObjList->pointObj.point;
        /* TODO: embPattern_calcBoundingBox for points */

        pObjList = pObjList->next;
    }

    pogObjList = p->polygonObjList;
    while(pogObjList)
    {
        pogPoint = pogObjList->polygonObj.polygonPoint;
        /* TODO: embPattern_calcBoundingBox for polygons */

        pogObjList = pogObjList->next;
    }

    polObjList = p->polylineObjList;
    while(polObjList)
    {
        polPoint = polObjList->polylineObj.polylinePoint;
        /* TODO: embPattern_calcBoundingBox for polylines */

        polObjList = polObjList->next;
    }

    rObjList = p->rectObjList;
    while(rObjList)
    {
        rect = rObjList->rectObj.rect;
        /* TODO: embPattern_calcBoundingBox for rectangles */

        rObjList = rObjList->next;
    }

    sObjList = p->splineObjList;
    while(sObjList)
    {
        bezier = sObjList->splineObj.bezier;
        /* TODO: embPattern_calcBoundingBox for splines */

        sObjList = sObjList->next;
    }

    return BoundingRect;
}

void embPattern_flipVertical(EmbPattern* p)
{
    EmbStitchList* pointer;
    pointer= p->stitchList;
    while(pointer)
    {
        pointer->stitch.yy = -pointer->stitch.yy;
        pointer = pointer->next;
    }
}

void embPattern_combineJumpStitches(EmbPattern* p)
{
    EmbStitchList* pointer;
    int jumpCount = 0;
    EmbStitchList* jumpListStart;
    while(pointer)
    {
        if(pointer->stitch.flags & JUMP)
        {
            if(jumpCount == 0)
            {
                jumpListStart = pointer;
            }
            jumpCount++;
        }
        else
        {
            if(jumpCount > 0)
            {
                EmbStitchList* removePointer = jumpListStart->next;
                jumpListStart->stitch.xx = pointer->stitch.xx;
                jumpListStart->stitch.yy = pointer->stitch.yy;
                jumpListStart->next = pointer;
                
                for(; jumpCount > 0; jumpCount--)
                {
                    EmbStitchList* tempPointer = removePointer->next;
                    free(removePointer);
                    removePointer = tempPointer;
                }
                jumpCount = 0;
            }
        }
        pointer = pointer->next;
    }
}

void embPattern_correctForMaxStitchLength(EmbPattern* p, double maxStitchLength, double maxJumpLength)
{
    int j = 0, splits;
    double maxXY, maxLen, addX, addY;

    if(embStitch_count(p->stitchList) > 1)
    {
        EmbStitchList* pointer;
        EmbStitchList* prev;
        prev = p->stitchList;
        pointer = prev->next;

        while(pointer)
        {
            double xx = prev->stitch.xx;
            double yy = prev->stitch.yy;
            double dx = pointer->stitch.xx - xx;
            double dy = pointer->stitch.yy - yy;
            if((fabs(dx) > maxStitchLength) || (fabs(dy) > maxStitchLength))
            {
                EmbStitchList *tempPointer = pointer;
                maxXY = max(fabs(dx), fabs(dy));
                if(pointer->stitch.flags & (JUMP | TRIM)) maxLen = maxJumpLength;
                else maxLen = maxStitchLength;

                splits = (int)ceil((double)maxXY / maxLen);
                
                if(splits > 1)
                {
                    int flagsToUse = pointer->stitch.flags;
                    int colorToUse = pointer->stitch.color;
                    addX = (double)dx / splits;
                    addY = (double)dy / splits;

                    for(j = 1; j < splits; j++)
                    {
                        EmbStitchList *item;
                        EmbStitch s;
                        s.xx = xx + addX * j;
                        s.yy = yy + addY * j;
                        s.flags = flagsToUse;
                        s.color = colorToUse;
                        item = (EmbStitchList *)malloc(sizeof(EmbStitchList));
                        item->stitch = s;
                        item->next = pointer;
                        prev->next = item;
                        prev = item;
                    }
                }
            }
            prev = pointer;
            if(pointer)
            {
                pointer = pointer->next;
            }
        }
    }
    if(p->lastStitch && p->lastStitch->stitch.flags != END)
    {
        embPattern_addStitchAbs(p, p->lastStitch->stitch.xx, p->lastStitch->stitch.yy, END, 1);
    }
}

void embPattern_center(EmbPattern* p)
{
    /* TODO: review this. currently not used in anywhere. Also needs to handle various design objects */
    int moveLeft, moveTop;
    EmbRect boundingRect;
    EmbStitchList* pointer;

    boundingRect = embPattern_calcBoundingBox(p);

    moveLeft = (int)(boundingRect.left - (embRect_width(boundingRect) / 2.0));
    moveTop = (int)(boundingRect.top - (embRect_height(boundingRect) / 2.0));

    pointer = p->stitchList;
    while(pointer)
    {
        EmbStitch s;
        s = pointer->stitch;
        s.xx -= moveLeft;
        s.yy -= moveTop;
    }
}

void embPattern_loadExternalColorFile(EmbPattern* p, const char* fileName)
{
    /*TODO: Fix the malloc memory corruption issues below */
    /*
    const char* dotPos = strrchr(filename, '.');

    char* extractName = (char *)malloc(dotPos - fileName);
    extractName = strncpy(extractName, filename, dotPos - filename);
    EmbReaderWriter* colorfile;
    colorfile = embReaderWriter_getByFileName(strcat(extractName,".edr"));
    if(!colorfile) return; //Ensure the pointer is valid
    if(!colorfile->reader(p, strcat(extractName,".edr")))
    {
        free(colorfile);
        colorfile = embReaderWriter_getByFileName(strcat(extractName,".rgb"));
        if(!colorfile->reader(p, strcat(extractName,".rgb")))
        {
            free(colorfile);
            colorfile = embReaderWriter_getByFileName(strcat(extractName,".col"));
            colorfile->reader(p, strcat(extractName,".col"));
        }
    }
    free(colorfile);
    */
}

void embPattern_free(EmbPattern* p)
{
    EmbThreadList* thisThreadList;
    EmbThreadList* nextThreadList;
    EmbStitchList* thisStitchList;
    EmbStitchList* nextStitchList;

    EmbArcObjectList* thisArcObjList;
    EmbArcObjectList* nextArcObjList;
    EmbCircleObjectList* thisCircleObjList;
    EmbCircleObjectList* nextCircleObjList;
    EmbEllipseObjectList* thisEllipseObjList;
    EmbEllipseObjectList* nextEllipseObjList;
    EmbLineObjectList* thisLineObjList;
    EmbLineObjectList* nextLineObjList;
    EmbPathObjectList* thisPathObjList;
    EmbPathObjectList* nextPathObjList;
    EmbPathObject* thisPathObj;
    EmbPathObject* nextPathObj;
    EmbPointObjectList* thisPointObjList;
    EmbPointObjectList* nextPointObjList;
    EmbPolygonObjectList* thisPolygonObjList;
    EmbPolygonObjectList* nextPolygonObjList;
    EmbPolylineObjectList* thisPolylineObjList;
    EmbPolylineObjectList* nextPolylineObjList;
    EmbRectObjectList* thisRectObjList;
    EmbRectObjectList* nextRectObjList;
    EmbSplineObjectList* thisSplineObjList;
    EmbSplineObjectList* nextSplineObjList;

    thisStitchList = p->stitchList;
    while(thisStitchList)
    {
        nextStitchList = thisStitchList->next;
        free(thisStitchList);
        thisStitchList = nextStitchList;
    }
    thisThreadList = p->threadList;
    while(thisThreadList)
    {
        nextThreadList = thisThreadList->next;
        free(thisThreadList);
        thisThreadList = nextThreadList;
    }

    thisArcObjList = p->arcObjList;
    while(thisArcObjList)
    {
        nextArcObjList = thisArcObjList->next;
        free(thisArcObjList);
        thisArcObjList = nextArcObjList;
    }
    thisCircleObjList = p->circleObjList;
    while(thisCircleObjList)
    {
        nextCircleObjList = thisCircleObjList->next;
        free(thisCircleObjList);
        thisCircleObjList = nextCircleObjList;
    }
    thisEllipseObjList = p->ellipseObjList;
    while(thisEllipseObjList)
    {
        nextEllipseObjList = thisEllipseObjList->next;
        free(thisEllipseObjList);
        thisEllipseObjList = nextEllipseObjList;
    }
    thisLineObjList = p->lineObjList;
    while(thisLineObjList)
    {
        nextLineObjList = thisLineObjList->next;
        free(thisLineObjList);
        thisLineObjList = nextLineObjList;
    }

    /*
    thisPathObjList = p->pathObjList;
    while(thisPathObjList)
    {


        thisPathObj = &thisPathObjList->pathObj;
        while(thisPathObj)
        {
            nextPathObj = thisPathObj->next;
            free(thisPathObj);
            thisPathObj = nextPathObj;
        }





        nextPathObjList = thisPathObjList->next;
        free(thisPathObjList);
        thisPathObjList = nextPathObjList;
    }
    */

    thisPointObjList = p->pointObjList;
    while(thisPointObjList)
    {
        nextPointObjList = thisPointObjList->next;
        free(thisPointObjList);
        thisPointObjList = nextPointObjList;
    }
    thisPolygonObjList = p->polygonObjList;
    while(thisPolygonObjList)
    {
        nextPolygonObjList = thisPolygonObjList->next;
        free(thisPolygonObjList);
        thisPolygonObjList = nextPolygonObjList;
    }
    thisPolylineObjList = p->polylineObjList;
    while(thisPolylineObjList)
    {
        nextPolylineObjList = thisPolylineObjList->next;
        free(thisPolylineObjList);
        thisPolylineObjList = nextPolylineObjList;
    }
    thisRectObjList = p->rectObjList;
    while(thisRectObjList)
    {
        nextRectObjList = thisRectObjList->next;
        free(thisRectObjList);
        thisRectObjList = nextRectObjList;
    }
    thisSplineObjList = p->splineObjList;
    while(thisSplineObjList)
    {
        nextSplineObjList = thisSplineObjList->next;
        free(thisSplineObjList);
        thisSplineObjList = nextSplineObjList;
    }
    free(p);
}

void embPattern_addCircleObjectAbs(EmbPattern* p, double cx, double cy, double r)
{
    EmbCircleObject circleObj = embCircleObj_make(cx, cy, r);

    if(!(p->circleObjList))
    {
        p->circleObjList = (EmbCircleObjectList*)malloc(sizeof(EmbCircleObjectList));
        p->circleObjList->circleObj = circleObj;
        p->circleObjList->next = 0;
        p->lastCircleObj = p->circleObjList;
    }
    else
    {
        embCircle_add(p->lastCircleObj, circleObj);
        p->lastCircleObj = p->lastCircleObj->next;
    }
}

void embPattern_addEllipseObjectAbs(EmbPattern* p, double cx, double cy, double rx, double ry)
{
    EmbEllipseObject ellipseObj = embEllipseObj_make(cx, cy, rx, ry);

    if(!(p->ellipseObjList))
    {
        p->ellipseObjList = (EmbEllipseObjectList*)malloc(sizeof(EmbEllipseObjectList));
        p->ellipseObjList->ellipseObj = ellipseObj;
        p->ellipseObjList->next = 0;
        p->lastEllipseObj = p->ellipseObjList;
    }
    else
    {
        embEllipse_add(p->lastEllipseObj, ellipseObj);
        p->lastEllipseObj = p->lastEllipseObj->next;
    }
}

void embPattern_addLineObjectAbs(EmbPattern* p, double x1, double y1, double x2, double y2)
{
    EmbLineObject lineObj = embLineObj_make(x1, y1, x2, y2);

    if(!(p->lineObjList))
    {
        p->lineObjList = (EmbLineObjectList*)malloc(sizeof(EmbLineObjectList));
        p->lineObjList->lineObj = lineObj;
        p->lineObjList->next = 0;
        p->lastLineObj = p->lineObjList;
    }
    else
    {
        embLine_add(p->lastLineObj, lineObj);
        p->lastLineObj = p->lastLineObj->next;
    }
}

void embPattern_addPathObjectAbs(EmbPattern* p, EmbPathObject* pathObj)
{
}

void embPattern_addPointObjectAbs(EmbPattern* p, double x, double y)
{
    EmbPointObject pointObj = embPointObj_make(x, y);

    if(!(p->pointObjList))
    {
        p->pointObjList = (EmbPointObjectList*)malloc(sizeof(EmbPointObjectList));
        p->pointObjList->pointObj = pointObj;
        p->pointObjList->next = 0;
        p->lastPointObj = p->pointObjList;
    }
    else
    {
        embPoint_add(p->lastPointObj, pointObj);
        p->lastPointObj = p->lastPointObj->next;
    }
}

void embPattern_addRectObjectAbs(EmbPattern* p, double x, double y, double w, double h)
{
    EmbRectObject rectObj = embRectObj_make(x, y, w, h);

    if(!(p->rectObjList))
    {
        p->rectObjList = (EmbRectObjectList*)malloc(sizeof(EmbRectObjectList));
        p->rectObjList->rectObj = rectObj;
        p->rectObjList->next = 0;
        p->lastRectObj = p->rectObjList;
    }
    else
    {
        embRect_add(p->lastRectObj, rectObj);
        p->lastRectObj = p->lastRectObj->next;
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
