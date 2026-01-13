#include "emb-pattern.h"
#include "emb-reader-writer.h"
#include "emb-settings.h"
#include "emb-logging.h"
#include "helpers-misc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifdef ARDUINO
#include "utility/ino-event.h"
#endif

/*! Returns a pointer to an EmbPattern. It is created on the heap. The caller is responsible for freeing the allocated memory with embPattern_free(). */
EmbPattern* embPattern_create(void)
{
    EmbPattern* p = 0;
    p = (EmbPattern*)malloc(sizeof(EmbPattern));
    if(!p) { embLog_error("emb-pattern.c embPattern_create(), unable to allocate memory for p\n"); return 0; }

    p->settings = embSettings_init();
    p->currentColorIndex = 0;
    p->stitchList = 0;
    p->threadList = 0;

    p->hoop.height = 0.0;
    p->hoop.width = 0.0;
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
    p->lastPolygonObj = 0;
    p->lastPolylineObj = 0;
    p->lastRectObj = 0;
    p->lastSplineObj = 0;

    p->lastX = 0.0;
    p->lastY = 0.0;

    return p;
}

void embPattern_hideStitchesOverLength(EmbPattern* p, int length)
{
    double prevX = 0;
    double prevY = 0;
    EmbStitchList* pointer = 0;

    if(!p) { embLog_error("emb-pattern.c embPattern_hideStitchesOverLength(), p argument is null\n"); return; }
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

int embPattern_addThread(EmbPattern* p, EmbThread thread)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_addThread(), p argument is null\n"); return 0; }
    if(embThreadList_empty(p->threadList))
    {
        p->threadList = p->lastThread = embThreadList_create(thread);
    }
    else
    {
        p->lastThread = embThreadList_add(p->lastThread, thread);
    }
    return 1;
}

void embPattern_fixColorCount(EmbPattern* p)
{
    /* fix color count to be max of color index. */
    int maxColorIndex = 0;
    EmbStitchList* list = 0;

    if(!p) { embLog_error("emb-pattern.c embPattern_fixColorCount(), p argument is null\n"); return; }
    list = p->stitchList;
    while(list)
    {
        maxColorIndex = max(maxColorIndex, list->stitch.color);
        list = list->next;
    }
#ifndef ARDUINO
    /* ARDUINO TODO: The while loop below never ends because memory cannot be allocated in the addThread
     *               function and thus the thread count is never incremented. Arduino or not, it's wrong.
     */
    while((int)embThreadList_count(p->threadList) <= maxColorIndex)
    {
        embPattern_addThread(p, embThread_getRandom());
    }
#endif
    /*
    while(embThreadList_count(p->threadList) > (maxColorIndex + 1))
    {
        TODO: erase last color    p->threadList.pop_back();
    }
    */
}

/*! Copies all of the EmbStitchList data to EmbPolylineObjectList data for pattern (\a p). */
void embPattern_copyStitchListToPolylines(EmbPattern* p)
{
    EmbStitchList* stList = 0;
    int breakAtFlags;

    if(!p) { embLog_error("emb-pattern.c embPattern_copyStitchListToPolylines(), p argument is null\n"); return; }

#ifdef EMB_DEBUG_JUMP
    breakAtFlags = (STOP | TRIM);
#else /* EMB_DEBUG_JUMP */
    breakAtFlags = (STOP | JUMP | TRIM);
#endif /* EMB_DEBUG_JUMP */

    stList = p->stitchList;
    while(stList)
    {
        EmbPointList* pointList = 0;
        EmbPointList* lastPoint = 0;
        EmbColor color;
        while(stList)
        {
            if(stList->stitch.flags & breakAtFlags)
            {
                break;
            }
            if(!(stList->stitch.flags & JUMP))
            {
                if(!pointList)
                {
                    pointList = lastPoint = embPointList_create(stList->stitch.xx, stList->stitch.yy);
                    color = embThreadList_getAt(p->threadList, stList->stitch.color).color;
                }
                else
                {
                    lastPoint = embPointList_add(lastPoint, embPoint_make(stList->stitch.xx, stList->stitch.yy));
                }
            }
            stList = stList->next;
        }

        /* NOTE: Ensure empty polylines are not created. This is critical. */
        if(pointList)
        {
            EmbPolylineObject* currentPolyline = (EmbPolylineObject*)malloc(sizeof(EmbPolylineObject));
            if(!currentPolyline) { embLog_error("emb-pattern.c embPattern_copyStitchListToPolylines(), cannot allocate memory for currentPolyline\n"); return; }
            currentPolyline->pointList = pointList;
            currentPolyline->color = color;
            currentPolyline->lineType = 1; /* TODO: Determine what the correct value should be */

            if(embPolylineObjectList_empty(p->polylineObjList))
            {
                p->polylineObjList = p->lastPolylineObj = embPolylineObjectList_create(currentPolyline);
            }
            else
            {
                p->lastPolylineObj = embPolylineObjectList_add(p->lastPolylineObj, currentPolyline);
            }
        }
        if(stList)
        {
            stList = stList->next;
        }
    }
}

/*! Copies all of the EmbPolylineObjectList data to EmbStitchList data for pattern (\a p). */
void embPattern_copyPolylinesToStitchList(EmbPattern* p)
{
    EmbPolylineObjectList* polyList = 0;
    int firstObject = 1;
    /*int currentColor = polyList->polylineObj->color TODO: polyline color */

    if(!p) { embLog_error("emb-pattern.c embPattern_copyPolylinesToStitchList(), p argument is null\n"); return; }
    polyList = p->polylineObjList;
    while(polyList)
    {
        EmbPolylineObject* currentPoly = 0;
        EmbPointList* currentPointList = 0;
        EmbThread thread;

        currentPoly = polyList->polylineObj;
        if(!currentPoly) { embLog_error("emb-pattern.c embPattern_copyPolylinesToStitchList(), currentPoly is null\n"); return; }
        currentPointList = currentPoly->pointList;
        if(!currentPointList) { embLog_error("emb-pattern.c embPattern_copyPolylinesToStitchList(), currentPointList is null\n"); return; }

        thread.catalogNumber = 0;
        thread.color = currentPoly->color;
        thread.description = 0;
        embPattern_addThread(p, thread);

        if(!firstObject)
        {
            embPattern_addStitchAbs(p, currentPointList->point.xx, currentPointList->point.yy, TRIM, 1);
            embPattern_addStitchRel(p, 0.0, 0.0, STOP, 1);
        }

        embPattern_addStitchAbs(p, currentPointList->point.xx, currentPointList->point.yy, JUMP, 1);
        while(currentPointList)
        {
            embPattern_addStitchAbs(p, currentPointList->point.xx, currentPointList->point.yy, NORMAL, 1);
            currentPointList = currentPointList->next;
        }
        firstObject = 0;
        polyList = polyList->next;
    }
    embPattern_addStitchRel(p, 0.0, 0.0, END, 1);
}

/*! Moves all of the EmbStitchList data to EmbPolylineObjectList data for pattern (\a p). */
void embPattern_moveStitchListToPolylines(EmbPattern* p)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_moveStitchListToPolylines(), p argument is null\n"); return; }
    embPattern_copyStitchListToPolylines(p);
    /* Free the stitchList and threadList since their data has now been transferred to polylines */
    embStitchList_free(p->stitchList);
    p->stitchList = 0;
    p->lastStitch = 0;
    embThreadList_free(p->threadList);
    p->threadList = 0;
    p->lastThread = 0;
}

/*! Moves all of the EmbPolylineObjectList data to EmbStitchList data for pattern (\a p). */
void embPattern_movePolylinesToStitchList(EmbPattern* p)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_movePolylinesToStitchList(), p argument is null\n"); return; }
    embPattern_copyPolylinesToStitchList(p);
    embPolylineObjectList_free(p->polylineObjList);
    p->polylineObjList = 0;
    p->lastPolylineObj = 0;
}

/*! Adds a stitch to the pattern (\a p) at the absolute position (\a x,\a y). Positive y is up. Units are in millimeters. */
void embPattern_addStitchAbs(EmbPattern* p, double x, double y, int flags, int isAutoColorIndex)
{
    EmbStitch s;

    if(!p) { embLog_error("emb-pattern.c embPattern_addStitchAbs(), p argument is null\n"); return; }

    if(flags & END)
    {
        if(embStitchList_empty(p->stitchList))
            return;
        /* Prevent unnecessary multiple END stitches */
        if(p->lastStitch->stitch.flags & END)
        {
            embLog_error("emb-pattern.c embPattern_addStitchAbs(), found multiple END stitches\n");
            return;
        }

        embPattern_fixColorCount(p);

        /* HideStitchesOverLength(127); TODO: fix or remove this */
    }

    if(flags & STOP)
    {
        if(embStitchList_empty(p->stitchList))
            return;
        if(isAutoColorIndex)
            p->currentColorIndex++;
    }

    /* NOTE: If the stitchList is empty, we will create it before adding stitches to it. The first coordinate will be the HOME position. */
    if(embStitchList_empty(p->stitchList))
    {
        /* NOTE: Always HOME the machine before starting any stitching */
        EmbPoint home = embSettings_home(&(p->settings));
        EmbStitch h;
        h.xx = home.xx;
        h.yy = home.yy;
        h.flags = JUMP;
        h.color = p->currentColorIndex;
        p->stitchList = p->lastStitch = embStitchList_create(h);
    }

    s.xx = x;
    s.yy = y;
    s.flags = flags;
    s.color = p->currentColorIndex;
#ifdef ARDUINO
    inoEvent_addStitchAbs(p, s.xx, s.yy, s.flags, s.color);
#else /* ARDUINO */
    p->lastStitch = embStitchList_add(p->lastStitch, s);
#endif /* ARDUINO */
    p->lastX = s.xx;
    p->lastY = s.yy;
}

/*! Adds a stitch to the pattern (\a p) at the relative position (\a dx,\a dy) to the previous stitch. Positive y is up. Units are in millimeters. */
void embPattern_addStitchRel(EmbPattern* p, double dx, double dy, int flags, int isAutoColorIndex)
{
    double x,y;

    if(!p) { embLog_error("emb-pattern.c embPattern_addStitchRel(), p argument is null\n"); return; }
    if(!embStitchList_empty(p->stitchList))
    {
        x = p->lastX + dx;
        y = p->lastY + dy;
    }
    else
    {
        /* NOTE: The stitchList is empty, so add it to the HOME position. The embStitchList_create function will ensure the first coordinate is at the HOME position. */
        EmbPoint home = embSettings_home(&(p->settings));
        x = home.xx + dx;
        y = home.yy + dy;
    }
    embPattern_addStitchAbs(p, x, y, flags, isAutoColorIndex);
}

void embPattern_changeColor(EmbPattern* p, int index)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_changeColor(), p argument is null\n"); return; }
    p->currentColorIndex = index;
}

/*! Reads a file with the given \a fileName and loads the data into \a pattern.
 *  Returns \c true if successful, otherwise returns \c false. */
int embPattern_read(EmbPattern* pattern, const char* fileName) /* TODO: Write test case using this convenience function. */
{
    EmbReaderWriter* reader = 0;
    int result = 0;

    if(!pattern) { embLog_error("emb-pattern.c embPattern_read(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("emb-pattern.c embPattern_read(), fileName argument is null\n"); return 0; }

    reader = embReaderWriter_getByFileName(fileName);
    if(!reader) { embLog_error("emb-pattern.c embPattern_read(), unsupported read file type: %s\n", fileName); return 0; }
    result = reader->reader(pattern, fileName);
    free(reader);
    reader = 0;
    return result;
}

/*! Writes the data from \a pattern to a file with the given \a fileName.
 *  Returns \c true if successful, otherwise returns \c false. */
int embPattern_write(EmbPattern* pattern, const char* fileName) /* TODO: Write test case using this convenience function. */
{
    EmbReaderWriter* writer = 0;
    int result = 0;

    if(!pattern) { embLog_error("emb-pattern.c embPattern_write(), pattern argument is null\n"); return 0; }
    if(!fileName) { embLog_error("emb-pattern.c embPattern_write(), fileName argument is null\n"); return 0; }

    writer = embReaderWriter_getByFileName(fileName);
    if(!writer) { embLog_error("emb-pattern.c embPattern_write(), unsupported write file type: %s\n", fileName); return 0; }
    result = writer->writer(pattern, fileName);
    free(writer);
    writer = 0;
    return result;
}

/* Very simple scaling of the x and y axis for every point.
* Doesn't insert or delete stitches to preserve density. */
void embPattern_scale(EmbPattern* p, double scale)
{
    EmbStitchList* pointer = 0;

    if(!p) { embLog_error("emb-pattern.c embPattern_scale(), p argument is null\n"); return; }
    pointer = p->stitchList;
    while(pointer)
    {
        pointer->stitch.xx *= scale;
        pointer->stitch.yy *= scale;
        pointer = pointer->next;
    }
}

/*! Returns an EmbRect that encapsulates all stitches and objects in the pattern (\a p). */
EmbRect embPattern_calcBoundingBox(EmbPattern* p)
{
    EmbStitchList* pointer = 0;
    EmbRect boundingRect;
    EmbStitch pt;
    EmbArcObjectList* aObjList = 0;
    EmbArc arc;
    EmbCircleObjectList* cObjList = 0;
    EmbCircle circle;
    EmbEllipseObjectList* eObjList = 0;
    EmbEllipse ellipse;
    EmbLineObjectList* liObjList = 0;
    EmbLine line;
    EmbPointObjectList* pObjList = 0;
    EmbPoint point;
    EmbPolygonObjectList* pogObjList = 0;
    EmbPointList* pogPointList = 0;
    EmbPoint pogPoint;
    EmbPolylineObjectList* polObjList = 0;
    EmbPointList* polPointList = 0;
    EmbPoint polPoint;
    EmbRectObjectList* rObjList = 0;
    EmbRect rect;
    EmbSplineObjectList* sObjList = 0;
    EmbBezier bezier;

    boundingRect.left = 0;
    boundingRect.right = 0;
    boundingRect.top = 0;
    boundingRect.bottom = 0;

    if(!p) { embLog_error("emb-pattern.c embPattern_calcBoundingBox(), p argument is null\n"); return boundingRect; }

    /* Calculate the bounding rectangle.  It's needed for smart repainting. */
    /* TODO: Come back and optimize this mess so that after going thru all objects
            and stitches, if the rectangle isn't reasonable, then return a default rect */
    if(embStitchList_empty(p->stitchList) &&
    embArcObjectList_empty(p->arcObjList) &&
    embCircleObjectList_empty(p->circleObjList) &&
    embEllipseObjectList_empty(p->ellipseObjList) &&
    embLineObjectList_empty(p->lineObjList) &&
    embPointObjectList_empty(p->pointObjList) &&
    embPolygonObjectList_empty(p->polygonObjList) &&
    embPolylineObjectList_empty(p->polylineObjList) &&
    embRectObjectList_empty(p->rectObjList) &&
    embSplineObjectList_empty(p->splineObjList))
    {
        boundingRect.top = 0.0;
        boundingRect.left = 0.0;
        boundingRect.bottom = 1.0;
        boundingRect.right = 1.0;
        return boundingRect;
    }
    boundingRect.left = 99999.0;
    boundingRect.top =  99999.0;
    boundingRect.right = -99999.0;
    boundingRect.bottom = -99999.0;

    pointer = p->stitchList;
    while(pointer)
    {
        /* If the point lies outside of the accumulated bounding
        * rectangle, then inflate the bounding rect to include it. */
        pt = pointer->stitch;
        if(!(pt.flags & TRIM))
        {
            boundingRect.left = (double)min(boundingRect.left, pt.xx);
            boundingRect.top = (double)min(boundingRect.top, pt.yy);
            boundingRect.right = (double)max(boundingRect.right, pt.xx);
            boundingRect.bottom = (double)max(boundingRect.bottom, pt.yy);
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
        boundingRect.left = (double)min(boundingRect.left, circle.centerX - circle.radius);
        boundingRect.top = (double)min(boundingRect.top, circle.centerY - circle.radius);
        boundingRect.right = (double)max(boundingRect.right, circle.centerX + circle.radius);
        boundingRect.bottom = (double)max(boundingRect.bottom, circle.centerY + circle.radius);

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
        pogPointList = pogObjList->polygonObj->pointList;
        while(pogPointList)
        {
            pogPoint = pogPointList->point;
            /* TODO: embPattern_calcBoundingBox for polygons */

            pogPointList = pogPointList->next;
        }
        pogObjList = pogObjList->next;
    }

    polObjList = p->polylineObjList;
    while(polObjList)
    {
        polPointList = polObjList->polylineObj->pointList;
        while(polPointList)
        {
            polPoint = polPointList->point;
            /* TODO: embPattern_calcBoundingBox for polylines */

            polPointList = polPointList->next;
        }
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

    return boundingRect;
}

/*! Flips the entire pattern (\a p) horizontally about the y-axis. */
void embPattern_flipHorizontal(EmbPattern* p)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_flipHorizontal(), p argument is null\n"); return; }
    embPattern_flip(p, 1, 0);
}

/*! Flips the entire pattern (\a p) vertically about the x-axis. */
void embPattern_flipVertical(EmbPattern* p)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_flipVertical(), p argument is null\n"); return; }
    embPattern_flip(p, 0, 1);
}

/*! Flips the entire pattern (\a p) horizontally about the x-axis if (\a horz) is true.
 *  Flips the entire pattern (\a p) vertically about the y-axis if (\a vert) is true. */
void embPattern_flip(EmbPattern* p, int horz, int vert)
{
    EmbStitchList* stList = 0;
    EmbArcObjectList* aObjList = 0;
    EmbCircleObjectList* cObjList = 0;
    EmbEllipseObjectList* eObjList = 0;
    EmbLineObjectList* liObjList = 0;
    EmbPathObjectList* paObjList = 0;
    EmbPointList* paPointList = 0;
    EmbPointObjectList* pObjList = 0;
    EmbPolygonObjectList* pogObjList = 0;
    EmbPointList* pogPointList = 0;
    EmbPolylineObjectList* polObjList = 0;
    EmbPointList* polPointList = 0;
    EmbRectObjectList* rObjList = 0;
    EmbSplineObjectList* sObjList = 0;

    if(!p) { embLog_error("emb-pattern.c embPattern_flip(), p argument is null\n"); return; }

    stList = p->stitchList;
    while(stList)
    {
        if(horz) { stList->stitch.xx = -stList->stitch.xx; }
        if(vert) { stList->stitch.yy = -stList->stitch.yy; }
        stList = stList->next;
    }

    aObjList = p->arcObjList;
    while(aObjList)
    {
        /* TODO: embPattern_flip for arcs */
        aObjList = aObjList->next;
    }

    cObjList = p->circleObjList;
    while(cObjList)
    {
        if(horz) { cObjList->circleObj.circle.centerX = -cObjList->circleObj.circle.centerX; }
        if(vert) { cObjList->circleObj.circle.centerY = -cObjList->circleObj.circle.centerY; }
        cObjList = cObjList->next;
    }

    eObjList = p->ellipseObjList;
    while(eObjList)
    {
        if(horz) { eObjList->ellipseObj.ellipse.centerX = -eObjList->ellipseObj.ellipse.centerX; }
        if(vert) { eObjList->ellipseObj.ellipse.centerY = -eObjList->ellipseObj.ellipse.centerY; }
        eObjList = eObjList->next;
    }

    liObjList = p->lineObjList;
    while(liObjList)
    {
        if(horz)
        {
            liObjList->lineObj.line.x1 = -liObjList->lineObj.line.x1;
            liObjList->lineObj.line.x2 = -liObjList->lineObj.line.x2;
        }
        if(vert)
        {
            liObjList->lineObj.line.y1 = -liObjList->lineObj.line.y1;
            liObjList->lineObj.line.y2 = -liObjList->lineObj.line.y2;
        }
        liObjList = liObjList->next;
    }

    paObjList = p->pathObjList;
    while(paObjList)
    {
        paPointList = paObjList->pathObj->pointList;
        while(paPointList)
        {
            if(horz) { paPointList->point.xx = -paPointList->point.xx; }
            if(vert) { paPointList->point.yy = -paPointList->point.yy; }
            paPointList = paPointList->next;
        }
        paObjList = paObjList->next;
    }

    pObjList = p->pointObjList;
    while(pObjList)
    {
        if(horz) { pObjList->pointObj.point.xx = -pObjList->pointObj.point.xx; }
        if(vert) { pObjList->pointObj.point.yy = -pObjList->pointObj.point.yy; }
        pObjList = pObjList->next;
    }

    pogObjList = p->polygonObjList;
    while(pogObjList)
    {
        pogPointList = pogObjList->polygonObj->pointList;
        while(pogPointList)
        {
            if(horz) { pogPointList->point.xx = -pogPointList->point.xx; }
            if(vert) { pogPointList->point.yy = -pogPointList->point.yy; }
            pogPointList = pogPointList->next;
        }
        pogObjList = pogObjList->next;
    }

    polObjList = p->polylineObjList;
    while(polObjList)
    {
        polPointList = polObjList->polylineObj->pointList;
        while(polPointList)
        {
            if(horz) { polPointList->point.xx = -polPointList->point.xx; }
            if(vert) { polPointList->point.yy = -polPointList->point.yy; }
            polPointList = polPointList->next;
        }
        polObjList = polObjList->next;
    }

    rObjList = p->rectObjList;
    while(rObjList)
    {
        if(horz)
        {
            rObjList->rectObj.rect.left = -rObjList->rectObj.rect.left;
            rObjList->rectObj.rect.right = -rObjList->rectObj.rect.right;
        }
        if(vert)
        {
            rObjList->rectObj.rect.top = -rObjList->rectObj.rect.top;
            rObjList->rectObj.rect.bottom = -rObjList->rectObj.rect.bottom;
        }
        rObjList = rObjList->next;
    }

    sObjList = p->splineObjList;
    while(sObjList)
    {
        /* TODO: embPattern_flip for splines */
        sObjList = sObjList->next;
    }
}

void embPattern_combineJumpStitches(EmbPattern* p)
{
    EmbStitchList* pointer = 0;
    int jumpCount = 0;
    EmbStitchList* jumpListStart = 0;

    if(!p) { embLog_error("emb-pattern.c embPattern_combineJumpStitches(), p argument is null\n"); return; }
    pointer = p->stitchList;
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

/*TODO: The params determine the max XY movement rather than the length. They need renamed or clarified further. */
void embPattern_correctForMaxStitchLength(EmbPattern* p, double maxStitchLength, double maxJumpLength)
{
    int j = 0, splits;
    double maxXY, maxLen, addX, addY;

    if(!p) { embLog_error("emb-pattern.c embPattern_correctForMaxStitchLength(), p argument is null\n"); return; }
    if(embStitchList_count(p->stitchList) > 1)
    {
        EmbStitchList* pointer = 0;
        EmbStitchList* prev = 0;
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
                        EmbStitchList* item = 0;
                        EmbStitch s;
                        s.xx = xx + addX * j;
                        s.yy = yy + addY * j;
                        s.flags = flagsToUse;
                        s.color = colorToUse;
                        item = (EmbStitchList*)malloc(sizeof(EmbStitchList));
                        if(!item) { embLog_error("emb-pattern.c embPattern_correctForMaxStitchLength(), cannot allocate memory for item\n"); return; }
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
    EmbStitchList* pointer = 0;

    if(!p) { embLog_error("emb-pattern.c embPattern_center(), p argument is null\n"); return; }
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

/*TODO: Description needed. */
void embPattern_loadExternalColorFile(EmbPattern* p, const char* fileName)
{
#ifdef ARDUINO
    return; /* TODO ARDUINO: This function leaks memory. While it isn't crucial to running the machine, it would be nice use this function, so fix it up. */
#endif /* ARDUINO */

    char hasRead = 0;
    EmbReaderWriter* colorFile = 0;
    const char* dotPos = strrchr(fileName, '.');
    char* extractName = 0;

    if(!p) { embLog_error("emb-pattern.c embPattern_loadExternalColorFile(), p argument is null\n"); return; }
    if(!fileName) { embLog_error("emb-pattern.c embPattern_loadExternalColorFile(), fileName argument is null\n"); return; }

    extractName = (char*)malloc(dotPos - fileName + 5);
    if(!extractName) { embLog_error("emb-pattern.c embPattern_loadExternalColorFile(), cannot allocate memory for extractName\n"); return; }
    extractName = (char*)memcpy(extractName, fileName, dotPos - fileName);
    extractName[dotPos - fileName] = '\0';
    strcat(extractName,".edr");
    colorFile = embReaderWriter_getByFileName(extractName);
    if(colorFile)
    {
        hasRead = (char)colorFile->reader(p, extractName);
    }
    if(!hasRead)
    {
        free(colorFile);
        colorFile = 0;
        extractName = (char*)memcpy(extractName, fileName, dotPos - fileName);
        extractName[dotPos - fileName] = '\0';
        strcat(extractName,".rgb");
        colorFile = embReaderWriter_getByFileName(extractName);
        if(colorFile)
        {
            hasRead = (char)colorFile->reader(p, extractName);
        }
    }
    if(!hasRead)
    {
        free(colorFile);
        colorFile = 0;
        extractName = (char*)memcpy(extractName, fileName, dotPos - fileName);
        extractName[dotPos - fileName] = '\0';
        strcat(extractName,".col");
        colorFile = embReaderWriter_getByFileName(extractName);
        if(colorFile)
        {
            hasRead = (char)colorFile->reader(p, extractName);
        }
    }
    if(!hasRead)
    {
        free(colorFile);
        colorFile = 0;
        extractName = (char*)memcpy(extractName, fileName, dotPos - fileName);
        extractName[dotPos - fileName] = '\0';
        strcat(extractName,".inf");
        colorFile = embReaderWriter_getByFileName(extractName);
        if(colorFile)
        {
            hasRead = (char)colorFile->reader(p, extractName);
        }
    }
    free(colorFile);
    colorFile = 0;
    free(extractName);
    extractName = 0;
}

/*! Frees all memory allocated in the pattern (\a p). */
void embPattern_free(EmbPattern* p)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_free(), p argument is null\n"); return; }
    embStitchList_free(p->stitchList);              p->stitchList = 0;      p->lastStitch = 0;
    embThreadList_free(p->threadList);              p->threadList = 0;      p->lastThread = 0;

    embArcObjectList_free(p->arcObjList);           p->arcObjList = 0;      p->lastArcObj = 0;
    embCircleObjectList_free(p->circleObjList);     p->circleObjList = 0;   p->lastCircleObj = 0;
    embEllipseObjectList_free(p->ellipseObjList);   p->ellipseObjList = 0;  p->lastEllipseObj = 0;
    embLineObjectList_free(p->lineObjList);         p->lineObjList = 0;     p->lastLineObj = 0;
    embPathObjectList_free(p->pathObjList);         p->pathObjList = 0;     p->lastPathObj = 0;
    embPointObjectList_free(p->pointObjList);       p->pointObjList = 0;    p->lastPointObj = 0;
    embPolygonObjectList_free(p->polygonObjList);   p->polygonObjList = 0;  p->lastPolygonObj = 0;
    embPolylineObjectList_free(p->polylineObjList); p->polylineObjList = 0; p->lastPolylineObj = 0;
    embRectObjectList_free(p->rectObjList);         p->rectObjList = 0;     p->lastRectObj = 0;
 /* embSplineObjectList_free(p->splineObjList);     p->splineObjList = 0;   p->lastSplineObj = 0; TODO: finish this */

    free(p);
    p = 0;
}

/*! Adds a circle object to pattern (\a p) with its center at the absolute position (\a cx,\a cy) with a radius of (\a r). Positive y is up. Units are in millimeters. */
void embPattern_addCircleObjectAbs(EmbPattern* p, double cx, double cy, double r)
{
    EmbCircleObject circleObj = embCircleObject_make(cx, cy, r);

    if(!p) { embLog_error("emb-pattern.c embPattern_addCircleObjectAbs(), p argument is null\n"); return; }
    if(embCircleObjectList_empty(p->circleObjList))
    {
        p->circleObjList = p->lastCircleObj = embCircleObjectList_create(circleObj);
    }
    else
    {
        p->lastCircleObj = embCircleObjectList_add(p->lastCircleObj, circleObj);
    }
}

/*! Adds an ellipse object to pattern (\a p) with its center at the absolute position (\a cx,\a cy) with radii of (\a rx,\a ry). Positive y is up. Units are in millimeters. */
void embPattern_addEllipseObjectAbs(EmbPattern* p, double cx, double cy, double rx, double ry)
{
    EmbEllipseObject ellipseObj = embEllipseObject_make(cx, cy, rx, ry);

    if(!p) { embLog_error("emb-pattern.c embPattern_addEllipseObjectAbs(), p argument is null\n"); return; }
    if(embEllipseObjectList_empty(p->ellipseObjList))
    {
        p->ellipseObjList = p->lastEllipseObj = embEllipseObjectList_create(ellipseObj);
    }
    else
    {
        p->lastEllipseObj = embEllipseObjectList_add(p->lastEllipseObj, ellipseObj);
    }
}

/*! Adds a line object to pattern (\a p) starting at the absolute position (\a x1,\a y1) and ending at the absolute position (\a x2,\a y2). Positive y is up. Units are in millimeters. */
void embPattern_addLineObjectAbs(EmbPattern* p, double x1, double y1, double x2, double y2)
{
    EmbLineObject lineObj = embLineObject_make(x1, y1, x2, y2);

    if(!p) { embLog_error("emb-pattern.c embPattern_addLineObjectAbs(), p argument is null\n"); return; }
    if(embLineObjectList_empty(p->lineObjList))
    {
        p->lineObjList = p->lastLineObj = embLineObjectList_create(lineObj);
    }
    else
    {
        p->lastLineObj = embLineObjectList_add(p->lastLineObj, lineObj);
    }
}

void embPattern_addPathObjectAbs(EmbPattern* p, EmbPathObject* obj)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_addPathObjectAbs(), p argument is null\n"); return; }
    if(!obj) { embLog_error("emb-pattern.c embPattern_addPathObjectAbs(), obj argument is null\n"); return; }
    if(embPointList_empty(obj->pointList)) { embLog_error("emb-pattern.c embPattern_addPathObjectAbs(), obj->pointList is empty\n"); return; }

    if(embPathObjectList_empty(p->pathObjList))
    {
        p->pathObjList = p->lastPathObj = embPathObjectList_create(obj);
    }
    else
    {
        p->lastPathObj = embPathObjectList_add(p->lastPathObj, obj);
    }
}

/*! Adds a point object to pattern (\a p) at the absolute position (\a x,\a y). Positive y is up. Units are in millimeters. */
void embPattern_addPointObjectAbs(EmbPattern* p, double x, double y)
{
    EmbPointObject pointObj = embPointObject_make(x, y);

    if(!p) { embLog_error("emb-pattern.c embPattern_addPointObjectAbs(), p argument is null\n"); return; }
    if(embPointObjectList_empty(p->pointObjList))
    {
        p->pointObjList = p->lastPointObj = embPointObjectList_create(pointObj);
    }
    else
    {
        p->lastPointObj = embPointObjectList_add(p->lastPointObj, pointObj);
    }
}

void embPattern_addPolygonObjectAbs(EmbPattern* p, EmbPolygonObject* obj)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_addPolygonObjectAbs(), p argument is null\n"); return; }
    if(!obj) { embLog_error("emb-pattern.c embPattern_addPolygonObjectAbs(), obj argument is null\n"); return; }
    if(embPointList_empty(obj->pointList)) { embLog_error("emb-pattern.c embPattern_addPolygonObjectAbs(), obj->pointList is empty\n"); return; }

    if(embPolygonObjectList_empty(p->polygonObjList))
    {
        p->polygonObjList = p->lastPolygonObj = embPolygonObjectList_create(obj);
    }
    else
    {
        p->lastPolygonObj = embPolygonObjectList_add(p->lastPolygonObj, obj);
    }
}

void embPattern_addPolylineObjectAbs(EmbPattern* p, EmbPolylineObject* obj)
{
    if(!p) { embLog_error("emb-pattern.c embPattern_addPolylineObjectAbs(), p argument is null\n"); return; }
    if(!obj) { embLog_error("emb-pattern.c embPattern_addPolylineObjectAbs(), obj argument is null\n"); return; }
    if(embPointList_empty(obj->pointList)) { embLog_error("emb-pattern.c embPattern_addPolylineObjectAbs(), obj->pointList is empty\n"); return; }

    if(embPolylineObjectList_empty(p->polylineObjList))
    {
        p->polylineObjList = p->lastPolylineObj = embPolylineObjectList_create(obj);
    }
    else
    {
        p->lastPolylineObj = embPolylineObjectList_add(p->lastPolylineObj, obj);
    }
}

/*! Adds a rectangle object to pattern (\a p) at the absolute position (\a x,\a y) with a width of (\a w) and a height of (\a h). Positive y is up. Units are in millimeters. */
void embPattern_addRectObjectAbs(EmbPattern* p, double x, double y, double w, double h)
{
    EmbRectObject rectObj = embRectObject_make(x, y, w, h);

    if(!p) { embLog_error("emb-pattern.c embPattern_addRectObjectAbs(), p argument is null\n"); return; }
    if(embRectObjectList_empty(p->rectObjList))
    {
        p->rectObjList = p->lastRectObj = embRectObjectList_create(rectObj);
    }
    else
    {
        p->lastRectObj = embRectObjectList_add(p->lastRectObj, rectObj);
    }
}

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
