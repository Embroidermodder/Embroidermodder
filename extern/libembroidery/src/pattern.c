/* 
 * This file is part of libembroidery.
 *
 * Copyright 2018-2022 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * The file is for the management of the main struct: EmbPattern.
 *******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "embroidery.h"

/*! Returns a pointer to an EmbPattern. It is created on the heap.
 * The caller is responsible for freeing the allocated memory with
 * embPattern_free(). */
EmbPattern*
embPattern_create(void)
{
    EmbPattern* p = (EmbPattern*)malloc(sizeof(EmbPattern));
    if (!p) { 
        printf("ERROR: emb-pattern.c embPattern_create(), ");
        printf("unable to allocate memory for p\n");
        return 0;
    }
    p->dstJumpsPerTrim = 6;
    p->home.x = 0.0;
    p->home.y = 0.0;
    p->currentColorIndex = 0;
    p->stitchList = embArray_create(EMB_STITCH);
    p->n_threads = 0;
    p->hoop_height = 0.0;
    p->hoop_width = 0.0;
    p->arcs = 0;
    p->circles = 0;
    p->ellipses = 0;
    p->lines = 0;
    p->paths = 0;
    p->points = 0;
    p->polygons = 0;
    p->polylines = 0;
    p->rects = 0;
    p->splines = 0;
    return p;
}

void
embPattern_hideStitchesOverLength(EmbPattern* p, int length)
{
    EmbVector prev;
    int i;
    prev.x = 0.0;
    prev.y = 0.0;

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_hideStitchesOverLength(), ");
        printf("p argument is null\n");
        return;
    }
    for (i = 0; i < p->stitchList->count; i++) {
        if ((fabs(p->stitchList->stitch[i].x - prev.x) > length)
         || (fabs(p->stitchList->stitch[i].y - prev.y) > length)) {
            p->stitchList->stitch[i].flags |= TRIM;
            p->stitchList->stitch[i].flags &= ~NORMAL;
        }
        prev.x = p->stitchList->stitch[i].x;
        prev.y = p->stitchList->stitch[i].y;
    }
}

int
embPattern_addThread(EmbPattern *pattern, EmbThread thread)
{
    if (pattern->n_threads >= MAX_THREADS) {
        puts("ERROR: Maximum number of threads has been reached.");
        printf("If you genuinely need: %d threads\n", pattern->n_threads);
        printf("please contact the Embroidermodder team and describe your\n");
        printf("use case.\n");
    }
    pattern->thread_list[pattern->n_threads] = thread;
    pattern->n_threads++;
    return 1;
}

void
embPattern_fixColorCount(EmbPattern* p)
{
    /* fix color count to be max of color index. */
    int maxColorIndex = 0, i;

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_fixColorCount(), ");
        printf("p argument is null\n");
        return;
    }
    for (i = 0; i < p->stitchList->count; i++) {
/*        printf("%d %d\n", list->stitch.color, maxColorIndex);*/
        maxColorIndex = EMB_MAX(maxColorIndex, p->stitchList->stitch[i].color);
    }
    if (p->n_threads == 0 || maxColorIndex == 0) {
        embPattern_addThread(p, black_thread);
    }
    else {
        if (maxColorIndex > 0) {
            while (p->n_threads <= maxColorIndex) {
/*        printf("%d %d\n", p->n_threads, maxColorIndex);*/
                embPattern_addThread(p, embThread_getRandom());
            }
        }
    }
    /*
    while (p->threadLists->count > (maxColorIndex + 1)) {
        TODO: erase last color    p->threadList.pop_back();
    }
    */
}

/*! Copies all of the EmbStitchList data to 
    EmbPolylineObjectList data for pattern (\a p). */
void
embPattern_copyStitchListToPolylines(EmbPattern* p)
{
    int breakAtFlags, i;
    EmbPoint point;
    EmbColor color;

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_copyStitchListToPolylines(), ");
        printf("p argument is null\n");
        return;
    }

    breakAtFlags = (STOP | JUMP | TRIM);

    for (i = 0; i < p->stitchList->count; i++) {
        EmbArray *pointList = 0;
        for (; i < p->stitchList->count; i++) {
            EmbStitch st = p->stitchList->stitch[i];
            if (st.flags & breakAtFlags) {
                break;
            }
            if (!(st.flags & JUMP)) {
                if (!pointList) {
                    pointList = embArray_create(EMB_POINT);
                    color = p->thread_list[st.color].color;
                }
                point.position.x = st.x;
                point.position.y = st.y;
                embArray_addPoint(pointList, point);
            }
        }

        /* NOTE: Ensure empty polylines are not created. This is critical. */
        if (pointList) {
            EmbPolyline currentPolyline;
            currentPolyline.pointList = pointList;
            currentPolyline.color = color;
            /* TODO: Determine what the correct value should be */
            currentPolyline.lineType = 1;

            embArray_addPolyline(p->polylines, currentPolyline);
        }
    }
}

/*! Copies all of the EmbPolylineObjectList data to EmbStitchList data for pattern (\a p). */
void
embPattern_copyPolylinesToStitchList(EmbPattern* p)
{
    int firstObject = 1, i, j;
    /*int currentColor = polyList->polylineObj->color TODO: polyline color */

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_copyPolylinesToStitchList(), ");
        printf("p argument is null\n");
        return;
    }
    if (!p->polylines) {
        printf("ERROR: emb-pattern.c embPattern_copyPolylinesToStitchList(), p argument is null\n");
        return;
    }
    for (i = 0; i < p->polylines->count; i++) {
        EmbPolyline currentPoly;
        EmbArray* currentPointList;
        EmbThread thread;

        currentPoly = p->polylines->polyline[i];
        currentPointList = currentPoly.pointList;

        strcpy(thread.catalogNumber, "");
        thread.color = currentPoly.color;
        strcpy(thread.description, "");
        embPattern_addThread(p, thread);

        if (!firstObject) {
            embPattern_addStitchAbs(p, currentPointList->point[0].position.x, currentPointList->point[0].position.y, TRIM, 1);
            embPattern_addStitchRel(p, 0.0, 0.0, STOP, 1);
        }

        embPattern_addStitchAbs(p, currentPointList->point[0].position.x, currentPointList->point[0].position.y, JUMP, 1);
        for (j = 1; j < currentPointList->count; j++) { 
            embPattern_addStitchAbs(p, currentPointList->point[j].position.x, currentPointList->point[j].position.y, NORMAL, 1);
        }
        firstObject = 0;
    }
    embPattern_addStitchRel(p, 0.0, 0.0, END, 1);
}

/*! Moves all of the EmbStitchList data to EmbPolylineObjectList data for pattern (\a p). */
void
embPattern_moveStitchListToPolylines(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_moveStitchListToPolylines(), p argument is null\n");
        return;
    }
    embPattern_copyStitchListToPolylines(p);
    /* Free the stitchList and threadList since their data has now been transferred to polylines */
    p->stitchList->count = 0;
    p->n_threads = 0;
}

/*! Moves all of the EmbPolylineObjectList data to EmbStitchList data for pattern (\a p). */
void
embPattern_movePolylinesToStitchList(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_movePolylinesToStitchList(), p argument is null\n");
        return;
    }
    embPattern_copyPolylinesToStitchList(p);
    embArray_free(p->polylines);
}

/*! Adds a stitch to the pattern (\a p) at the absolute position (\a x,\a y). Positive y is up. Units are in millimeters. */
void
embPattern_addStitchAbs(EmbPattern* p, double x, double y, 
                            int flags, int isAutoColorIndex)
{
    EmbStitch s;

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addStitchAbs(), ");
        printf("p argument is null\n");
        return;
    }

    if (flags & END) {
        if (p->stitchList->count == 0) {
            return;
        }
        /* Prevent unnecessary multiple END stitches */
        if (p->stitchList->stitch[p->stitchList->count - 1].flags & END) {
            printf("ERROR: emb-pattern.c embPattern_addStitchAbs(), found multiple END stitches\n");
            return;
        }
        embPattern_fixColorCount(p);
        /* HideStitchesOverLength(127); TODO: fix or remove this */
    }

    if (flags & STOP) {
        if (p->stitchList->count == 0) {
            return;
        }
        if (isAutoColorIndex) {
            p->currentColorIndex++;
        }
    }

    /* NOTE: If the stitchList is empty, we will create it before adding 
        stitches to it. The first coordinate will be the HOME position. */
    if (p->stitchList->count == 0) {
        /* NOTE: Always HOME the machine before starting any stitching */
        EmbStitch h;
        h.x = p->home.x;
        h.y = p->home.y;
        h.flags = JUMP;
        h.color = p->currentColorIndex;
        embArray_addStitch(p->stitchList, h);
    }
    s.x = x;
    s.y = y;
    s.flags = flags;
    s.color = p->currentColorIndex;
    embArray_addStitch(p->stitchList, s);
}

/*! Adds a stitch to the pattern (\a p) at the relative position (\a dx,\a dy) 
    to the previous stitch. Positive y is up. Units are in millimeters. */
void
embPattern_addStitchRel(EmbPattern* p, double dx, double dy, 
                            int flags, int isAutoColorIndex)
{
    double x, y;
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addStitchRel(), p argument is null\n");
        return;
    }
    if (p->stitchList->count > 0) {
        EmbStitch st = p->stitchList->stitch[p->stitchList->count - 1];
        x = st.x + dx;
        y = st.y + dy;
    } else {
        /* NOTE: The stitchList is empty, so add it to the HOME position.
         * The embStitchList_create function will ensure the first coordinate is at the HOME position. */
        x = p->home.x + dx;
        y = p->home.y + dy;
    }
    embPattern_addStitchAbs(p, x, y, flags, isAutoColorIndex);
}

void
embPattern_changeColor(EmbPattern* p, int index)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_changeColor(), p argument is null\n");
        return;
    }
    p->currentColorIndex = index;
}

/* Very simple scaling of the x and y axis for every point.
* Doesn't insert or delete stitches to preserve density. */
void
embPattern_scale(EmbPattern* p, double scale)
{
    int i;
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_scale(), p argument is null\n");
        return;
    }

    for (i = 0; i < p->stitchList->count; i++) {
        p->stitchList->stitch[i].x *= scale;
        p->stitchList->stitch[i].y *= scale;
    }
}

/*! Returns an EmbRect that encapsulates all stitches and objects in the pattern (\a p). */
EmbRect
embPattern_calcBoundingBox(EmbPattern* p)
{
    EmbRect r;
    EmbStitch pt;
    EmbArc arc;
    EmbCircle circle;
    EmbEllipse ellipse;
    EmbLine line;
    EmbVector point;
    EmbArray *polygon;
    EmbArray *polyline;
    int i, j;

    r.left = 0;
    r.right = 0;
    r.top = 0;
    r.bottom = 0;

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_calcBoundingBox(), ");
        printf("p argument is null\n");
        return r;
    }

    /* Calculate the bounding rectangle.  It's needed for smart repainting. */
    /* TODO: Come back and optimize this mess so that after going thru all objects
            and stitches, if the rectangle isn't reasonable, then return a default rect */
    if (p->stitchList->count == 0 &&
        !(p->arcs || p->circles || p->ellipses || p->lines || p->points ||
        p->polygons || p->polylines || p->rects || p->splines)) {
        r.top = 0.0;
        r.left = 0.0;
        r.bottom = 1.0;
        r.right = 1.0;
        return r;
    }
    r.left = 99999.0;
    r.top =  99999.0;
    r.right = -99999.0;
    r.bottom = -99999.0;

    for (i = 0; i < p->stitchList->count; i++) {
        /* If the point lies outside of the accumulated bounding
        * rectangle, then inflate the bounding rect to include it. */
        pt = p->stitchList->stitch[i];
        if (!(pt.flags & TRIM)) {
            r.left = EMB_MIN(r.left, pt.x);
            r.top = EMB_MIN(r.top, pt.y);
            r.right = EMB_MAX(r.right, pt.x);
            r.bottom = EMB_MAX(r.bottom, pt.y);
        }
    }

    if (p->arcs) {
        /* TODO: embPattern_calcBoundingBox for arcs, 
            for now just checks the start point */
        for (i = 0; i < p->arcs->count; i++) {
            arc = p->arcs->arc[i];
            r.left = EMB_MIN(r.left, arc.start.x);
            r.top = EMB_MIN(r.top, arc.start.y);
            r.right = EMB_MAX(r.right, arc.start.x);
            r.bottom = EMB_MAX(r.bottom, arc.start.y);
        }
    }

    if (p->circles) {
        for (i = 0; i < p->circles->count; i++) {
            circle = p->circles->circle[i];
            r.left = EMB_MIN(r.left, circle.center.x - circle.radius);
            r.top = EMB_MIN(r.top, circle.center.y - circle.radius);
            r.right = EMB_MAX(r.right, circle.center.x + circle.radius);
            r.bottom = EMB_MAX(r.bottom, circle.center.y + circle.radius);
        }
    }

    if (p->ellipses) {
        for (i = 0; i < p->ellipses->count; i++) {
            /* TODO: account for rotation */
            ellipse = p->ellipses->ellipse[i];
            r.left = EMB_MIN(r.left, ellipse.center.x - ellipse.radius.x);
            r.top = EMB_MIN(r.top, ellipse.center.y - ellipse.radius.y);
            r.right = EMB_MAX(r.right, ellipse.center.x + ellipse.radius.x);
            r.bottom = EMB_MAX(r.bottom, ellipse.center.y + ellipse.radius.y);
        }
    }

    if (p->lines) {
        for (i = 0; i < p->lines->count; i++) {
            line = p->lines->line[i];
            r.left = EMB_MIN(r.left, line.start.x);
            r.left = EMB_MIN(r.left, line.end.x);
            r.top = EMB_MIN(r.top, line.start.y);
            r.top = EMB_MIN(r.top, line.end.y);
            r.right = EMB_MAX(r.right, line.start.x);
            r.right = EMB_MAX(r.right, line.end.x);
            r.bottom = EMB_MAX(r.bottom, line.start.y);
            r.bottom = EMB_MAX(r.bottom, line.end.y);
        }
    }

    if (p->points) {
        for (i = 0; i < p->points->count; i++) {
            point = p->points->point[i].position;
            r.left = EMB_MIN(r.left, point.x);
            r.top = EMB_MIN(r.top, point.y);
            r.right = EMB_MAX(r.right, point.x);
            r.bottom = EMB_MAX(r.bottom, point.y);
        }
    }

    if (p->polygons) {
        for (i = 0; i < p->polygons->count; i++) {
            polygon = p->polygons->polygon[i].pointList;
            for (j=0; j < polygon->count; j++) {
                /* TODO: embPattern_calcBoundingBox for polygons */
            }
        }
    }

    if (p->polylines) {
        for (i = 0; i < p->polylines->count; i++) {
            polyline = p->polylines->polyline[i].pointList;
            for (j=0; j < polyline->count; j++) {
                /* TODO: embPattern_calcBoundingBox for polylines */
            }
        }
    }

    if (p->rects) {
        EmbRect rect;
        for (i = 0; i < p->rects->count; i++) {
            rect = p->rects->rect[i];
            r.left = EMB_MIN(r.left, rect.left);
            r.top = EMB_MIN(r.top, rect.top);
            r.right = EMB_MAX(r.right, rect.right);
            r.bottom = EMB_MAX(r.bottom, rect.bottom);
        }
    }

    if (p->splines) {
        for (i = 0; i < p->splines->count; i++) {
            /* EmbBezier bezier;
            bezier = p->splines->spline[i].bezier; */
            /* TODO: embPattern_calcBoundingBox for splines */
        }
    }
    return r;
}

/*! Flips the entire pattern (\a p) horizontally about the y-axis. */
void
embPattern_flipHorizontal(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_flipHorizontal(), ");
        printf("p argument is null\n");
        return;
    }
    embPattern_flip(p, 1, 0);
}

/*! Flips the entire pattern (\a p) vertically about the x-axis. */
void
embPattern_flipVertical(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_flipVertical(), ");
        printf("p argument is null\n");
        return;
    }
    embPattern_flip(p, 0, 1);
}

/*! Flips the entire pattern (\a p) horizontally about the x-axis if (\a horz) is true.
 *  Flips the entire pattern (\a p) vertically about the y-axis if (\a vert) is true. */
void
embPattern_flip(EmbPattern* p, int horz, int vert)
{
    int i, j;

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_flip(), p argument is null\n");
        return;
    }

    for (i = 0; i < p->stitchList->count; i++) {
        if (horz) {
            p->stitchList->stitch[i].x *= -1.0;
        }
        if (vert) {
            p->stitchList->stitch[i].y *= -1.0;
        }
    }

    if (p->arcs) {
        for (i = 0; i < p->arcs->count; i++) {
            if (horz) {
                p->arcs->arc[i].start.x *= -1.0;
                p->arcs->arc[i].mid.x *= -1.0;
                p->arcs->arc[i].end.x *= -1.0;
            }
            if (vert) {
                p->arcs->arc[i].start.y *= -1.0;
                p->arcs->arc[i].mid.y *= -1.0;
                p->arcs->arc[i].end.y *= -1.0;
            }
        }
    }

    if (p->circles) {
        for (i = 0; i < p->circles->count; i++) {
            if (horz) { p->circles->circle[i].center.x *= -1.0; }
            if (vert) { p->circles->circle[i].center.y *= -1.0; }
        }
    }

    if (p->ellipses) {
        for (i = 0; i < p->ellipses->count; i++) {
            if (horz) { p->ellipses->ellipse[i].center.x *= -1.0; }
            if (vert) { p->ellipses->ellipse[i].center.y *= -1.0; }
        }
    }

    if (p->lines) {
        for (i = 0; i < p->lines->count; i++) {
            if (horz) {
                p->lines->line[i].start.x *= -1.0;
                p->lines->line[i].end.x *= -1.0;
            }
            if (vert) {
                p->lines->line[i].start.y *= -1.0;
                p->lines->line[i].end.y *= -1.0;
            }
        }
    }

    if (p->paths) {
        for (i = 0; i < p->paths->count; i++) {
            EmbArray *path = p->paths->path[i].pointList;
            for (j=0; j < path->count; j++) {
                if (horz) {
                    path->point[j].position.x *= -1.0;
                }
                if (vert) {
                    path->point[j].position.y *= -1.0;
                }
            }
        }
    }

    if (p->points) {
        for (i = 0; i < p->points->count; i++) {
            if (horz) {
                p->points->point[i].position.x *= -1.0;
            }
            if (vert) {
                p->points->point[i].position.y *= -1.0;
            }
        }
    }

    if (p->polygons) {
        for (i = 0; i < p->polygons->count; i++) {
            EmbArray *polygon;
            polygon = p->polygons->polygon[i].pointList;
            for (j=0; j < polygon->count; j++) {
                if (horz) { polygon->point[j].position.x *= -1.0; }
                if (vert) { polygon->point[j].position.y *= -1.0; }
            }
        }
    }

    if (p->polylines) {
        for (i = 0; i < p->polylines->count; i++) {
            EmbArray *polyline;
            polyline = p->polylines->polygon[i].pointList;
            for (j=0; j < polyline->count; j++) {
                if (horz) { polyline->point[j].position.x *= -1.0; }
                if (vert) { polyline->point[j].position.y *= -1.0; }
            }
        }
    }

    if (p->rects) {
        for (i = 0; i < p->rects->count; i++) {
            if (horz) {
                p->rects->rect[i].left *= -1.0;
                p->rects->rect[i].right *= -1.0;
            }
            if (vert) {
                p->rects->rect[i].top *= -1.0;
                p->rects->rect[i].bottom *= -1.0;
            }
        }
    }

    if (p->splines) {
        for (i = 0; i < p->splines->count; i++) {
            /* TODO: embPattern_flip for splines */
        }
    }
}

void
embPattern_combineJumpStitches(EmbPattern* p)
{
    int jumpCount = 0, i;
    EmbArray *newList;
    EmbStitch j;

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_combineJumpStitches(), ");
        printf("p argument is null\n");
        return;
    }
    newList = embArray_create(EMB_STITCH);
    for (i = 0; i < p->stitchList->count; i++) {
        EmbStitch st = p->stitchList->stitch[i];
        if (st.flags & JUMP) {
            if (jumpCount == 0) {
                j = st;
            } else {
                j.x += st.x;
                j.y += st.y;
            }
            jumpCount++;
        } else {
            if (jumpCount > 0) {
                embArray_addStitch(newList, j);
            }
            embArray_addStitch(newList, st);
        }
    }
    embArray_free(p->stitchList);
    p->stitchList = newList;
}

/*TODO: The params determine the max XY movement rather than the length. 
    They need renamed or clarified further. */
void
embPattern_correctForMaxStitchLength(EmbPattern* p, 
                        double maxStitchLength, double maxJumpLength)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_correctForMaxStitchLength(), ");
        printf("p argument is null\n");
        return;
    }
    if (p->stitchList->count > 1) {
        int i, j, splits;
        double maxXY, maxLen, addX, addY;
        EmbArray *newList = embArray_create(EMB_STITCH);
        for (i=1; i < p->stitchList->count; i++) {
            EmbStitch st = p->stitchList->stitch[i];
            double xx = st.x;
            double yy = st.y;
            double dx = p->stitchList->stitch[i-1].x - xx;
            double dy = p->stitchList->stitch[i-1].y - yy;
            if ((fabs(dx) > maxStitchLength) || (fabs(dy) > maxStitchLength)) {
                maxXY = EMB_MAX(fabs(dx), fabs(dy));
                if (st.flags & (JUMP | TRIM)) {
                    maxLen = maxJumpLength;
                } else {
                    maxLen = maxStitchLength;
                }
                splits = (int)ceil((double)maxXY / maxLen);

                if (splits > 1) {
                    addX = (double)dx / splits;
                    addY = (double)dy / splits;

                    for (j = 1; j < splits; j++) {
                        EmbStitch s;
                        s = st;
                        s.x = xx + addX * j;
                        s.y = yy + addY * j;
                        embArray_addStitch(newList, s);
                    }
                }
            }
            embArray_addStitch(newList, st);
        }
        embArray_free(p->stitchList);
        p->stitchList = newList;
    }
    embPattern_end(p);
}

void
embPattern_center(EmbPattern* p)
{
    /* TODO: review this. currently not used in anywhere. 
        Also needs to handle various design objects */
    int moveLeft, moveTop, i;
    EmbRect boundingRect;
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_center(), p argument is null\n");
        return;
    }
    boundingRect = embPattern_calcBoundingBox(p);

    moveLeft = (int)(boundingRect.left - (boundingRect.right- boundingRect.left) / 2.0);
    moveTop = (int)(boundingRect.top - (boundingRect.bottom - boundingRect.top) / 2.0);

    for (i = 0; i < p->stitchList->count; i++) {
        p->stitchList->stitch[i].x -= moveLeft;
        p->stitchList->stitch[i].y -= moveTop;
    }
}

/*TODO: Description needed. */
void
embPattern_loadExternalColorFile(EmbPattern* p, const char* fileName)
{
    int hasRead, stub_len, format;
    char extractName[200];

    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_loadExternalColorFile(), p argument is null\n");
        return;
    }
    if (!fileName) {
        printf("ERROR: emb-pattern.c embPattern_loadExternalColorFile(), fileName argument is null\n");
        return;
    }

    strcpy(extractName, fileName);
    format = emb_identify_format(fileName);
    stub_len = strlen(fileName) - strlen(formatTable[format].extension);
    extractName[stub_len] = 0;
    strcat(extractName, ".edr");
    hasRead = embPattern_read(p, extractName, EMB_FORMAT_EDR);
    if (!hasRead) {
        extractName[stub_len] = 0;
        strcat(extractName,".rgb");
        hasRead = embPattern_read(p, extractName, EMB_FORMAT_RGB);
    }
    if (!hasRead) {
        extractName[stub_len] = 0;
        strcat(extractName,".col");
        hasRead = embPattern_read(p, extractName, EMB_FORMAT_COL);
    }
    if (!hasRead) {
        extractName[stub_len] = 0;
        strcat(extractName,".inf");
        hasRead = embPattern_read(p, extractName, EMB_FORMAT_INF);
    }
}

/*! Frees all memory allocated in the pattern (\a p). */
void
embPattern_free(EmbPattern* p)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_free(), p argument is null\n");
        return;
    }
    embArray_free(p->stitchList);
    embArray_free(p->arcs);
    embArray_free(p->circles);
    embArray_free(p->ellipses);
    embArray_free(p->lines);
    embArray_free(p->paths);
    embArray_free(p->points);
    embArray_free(p->polygons);
    embArray_free(p->polylines);
    embArray_free(p->rects);
    embArray_free(p->splines);

    free(p);
}

/*! Adds a circle object to pattern (\a p) with its center at the absolute
 * position (\a cx,\a cy) with a radius of (\a r). Positive y is up.
 * Units are in millimeters. */
void
embPattern_addCircleAbs(EmbPattern* p, EmbCircle circle)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addCircleObjectAbs(), p argument is null\n");
        return;
    }
    if (p->circles == 0) {
         p->circles = embArray_create(EMB_CIRCLE);
    }
    embArray_addCircle(p->circles, circle);
}

/*! Adds an ellipse object to pattern (\a p) with its center at the
 * absolute position (\a cx,\a cy) with radii of (\a rx,\a ry). Positive y is up.
 * Units are in millimeters. */
void
embPattern_addEllipseAbs(EmbPattern* p, EmbEllipse ellipse)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addEllipseObjectAbs(), p argument is null\n");
        return;
    }
    if (!p->ellipses) {
        p->ellipses = embArray_create(EMB_ELLIPSE);
    }
    embArray_addEllipse(p->ellipses, ellipse);
}

/*! Adds a line object to pattern (\a p) starting at the absolute position
 * (\a x1,\a y1) and ending at the absolute position (\a x2,\a y2).
 * Positive y is up. Units are in millimeters.
 */
void
embPattern_addLineAbs(EmbPattern* p, EmbLine line)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addLineObjectAbs(), p argument is null\n");
        return;
    }
    if (p->circles == 0) {
         p->lines = embArray_create(EMB_LINE);
    }
    embArray_addLine(p->lines, line);
}

void
embPattern_addPathAbs(EmbPattern* p, EmbPath obj)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addPathObjectAbs(), p argument is null\n");
        return;
    }
    if (!obj.pointList) {
        printf("ERROR: emb-pattern.c embPattern_addPathObjectAbs(), obj->pointList is empty\n");
        return;
    }
    if (!p->paths) {
        p->paths = embArray_create(EMB_PATH);
    }
    embArray_addPath(p->paths, obj);
}

/*! Adds a point object to pattern (\a p) at the absolute position (\a x,\a y). Positive y is up. Units are in millimeters. */
void
embPattern_addPointAbs(EmbPattern* p, EmbPoint obj)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addPointObjectAbs(), p argument is null\n");
        return;
    }
    if (!p->points) {
        p->points = embArray_create(EMB_POINT);
    }
    embArray_addPoint(p->points, obj);
}

void
embPattern_addPolygonAbs(EmbPattern* p, EmbPolygon obj)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addPolygonObjectAbs(), p argument is null\n");
        return;
    }
    if (!obj.pointList) {
        printf("ERROR: emb-pattern.c embPattern_addPolygonObjectAbs(), obj->pointList is empty\n");
        return;
    }

    if (!p->polygons) {
        p->polygons = embArray_create(EMB_POLYGON);
    }
    embArray_addPolygon(p->polygons, obj);
}

void
embPattern_addPolylineObjectAbs(EmbPattern* p, EmbPolyline obj)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addPolylineObjectAbs(), p argument is null\n");
        return;
    }
    if (!obj.pointList) {
        printf("ERROR: emb-pattern.c embPattern_addPolylineObjectAbs(), obj->pointList is empty\n");
        return;
    }
    if (!p->polylines) {
        p->polylines = embArray_create(EMB_POLYLINE);
    }
    embArray_addPolyline(p->polylines, obj);
}

/**
 * Adds a rectangle object to pattern (\a p) at the absolute position
 * (\a x,\a y) with a width of (\a w) and a height of (\a h).
 * Positive y is up. Units are in millimeters.
 */
void
embPattern_addRectAbs(EmbPattern* p, EmbRect rect)
{
    if (!p) {
        printf("ERROR: emb-pattern.c embPattern_addRectObjectAbs(), p argument is null\n");
        return;
    }
    if (!p->rects) {
        p->rects = embArray_create(EMB_RECT);
    }
    embArray_addRect(p->rects, rect);
}

void
embPattern_end(EmbPattern *p)
{
    if (p->stitchList->count == 0) {
        return;
    }
    /* Check for an END stitch and add one if it is not present */
    if (p->stitchList->stitch[p->stitchList->count-1].flags != END) {
        embPattern_addStitchRel(p, 0, 0, END, 1);
    }
}


int
embPattern_color_count(EmbPattern *pattern, EmbColor startColor)
{
    int numberOfColors = 0, i;
    EmbColor color = startColor;
    for (i=0; i<pattern->stitchList->count; i++) {
        EmbColor newColor;
        EmbStitch st;

        st = pattern->stitchList->stitch[i];

        newColor = pattern->thread_list[st.color].color;
        if (embColor_distance(newColor, color) != 0) {
            numberOfColors++;
            color = newColor;
        }
        else if (st.flags & END || st.flags & STOP) {
            numberOfColors++;
        }

        while (pattern->stitchList->stitch[i+1].flags == st.flags) {
            i++;
            if (i >= pattern->stitchList->count-2) {
                break;
            }
        }
    }
    return numberOfColors;
}


void
embPattern_designDetails(EmbPattern *pattern)
{
    int colors, num_stitches, real_stitches, jump_stitches, trim_stitches;
    int unknown_stitches, num_colors;
    EmbRect bounds;

    puts("Design Details");
    bounds = embPattern_calcBoundingBox(pattern);

    colors = 1;
    num_stitches = pattern->stitchList->count;
    real_stitches = 0;
    jump_stitches = 0;
    trim_stitches = 0;
    unknown_stitches = 0;
    bounds = embPattern_calcBoundingBox(pattern);
    
    if (emb_verbose > 1) {
        printf("colors: %d\n", colors);
        printf("num_stitches: %d\n", num_stitches);
        printf("real_stitches: %d\n", real_stitches);
        printf("jump_stitches: %d\n", jump_stitches);
        printf("trim_stitches: %d\n", trim_stitches);
        printf("unknown_stitches: %d\n", unknown_stitches);
        printf("num_colors: %d\n", pattern->n_threads);
        printf("bounds.left: %f\n", bounds.left);
        printf("bounds.right: %f\n", bounds.right);
        printf("bounds.top: %f\n", bounds.top);
        printf("bounds.bottom: %f\n", bounds.bottom);
    }
/*
    double minx = 0.0, maxx = 0.0, miny = 0.0, maxy = 0.0;
    double min_stitchlength = 999.0;
    double max_stitchlength = 0.0;
    double total_stitchlength = 0.0;
    int number_of_minlength_stitches = 0;
    int number_of_maxlength_stitches = 0;

    double xx = 0.0, yy = 0.0;
    double length = 0.0;

    if (num_stitches == 0) {
        QMessageBox::warning(this, tr("No Design Loaded"), tr("<b>A design needs to be loaded or created before details can be determined.</b>"));
        return;
    }
    QVector<double> stitchLengths;

    double totalColorLength = 0.0;
    for (int i = 0; i < num_stitches; i++) {
        EmbStitch st = pattern->stitchList->stitch[i];
        double dx, dy;
        dx = st.x - xx;
        dy = st.y - yy;
        xx = st.x;
        yy = st.y;
        length=sqrt(dx * dx + dy * dy);
        totalColorLength += length;
        if(i > 0 && embStitchList_getAt(pattern->stitchList, i-1).flags != NORMAL)
            length = 0.0; //can't count first normal stitch;
        if(!(embStitchList_getAt(pattern->stitchList, i).flags & (JUMP | TRIM)))
        {
            real_stitches++;
            if(length > max_stitchlength) { max_stitchlength = length; number_of_maxlength_stitches = 0; }
            if(length == max_stitchlength) number_of_maxlength_stitches++;
            if(length > 0 && length < min_stitchlength)
            {
                min_stitchlength = length;
                number_of_minlength_stitches = 0;
            }
            if(length == min_stitchlength) number_of_minlength_stitches++;
            total_stitchlength += length;
            if(xx < minx) minx = xx;
            if(xx > maxx) maxx = xx;
            if(yy < miny) miny = yy;
            if(yy > maxy) maxy = yy;
        }
        if (st.flags & JUMP) {
            jump_stitches++;
        }
        if (st.flags & TRIM) {
            trim_stitches++;
        }
        if (st.flags & STOP) {
            stitchLengths.push_back(totalColorLength);
            totalColorLength = 0;
            colors++;
        }
        if (st.flags & END) {
            stitchLengths.push_back(totalColorLength);
        }
    }

    //second pass to fill bins now that we know max stitch length
#define NUMBINS 10
    int bin[NUMBINS+1];
    for (int i = 0; i <= NUMBINS; i++) {
        bin[i]=0;
    }

    for (int i = 0; i < num_stitches; i++) {
        dx = embStitchList_getAt(pattern->stitchList, i).xx - xx;
        dy = embStitchList_getAt(pattern->stitchList, i).yy - yy;
        xx = embStitchList_getAt(pattern->stitchList, i).xx;
        yy = embStitchList_getAt(pattern->stitchList, i).yy;
        if(i > 0 && embStitchList_getAt(pattern->stitchList, i-1).flags == NORMAL && embStitchList_getAt(pattern->stitchList, i).flags == NORMAL)
        {
            length=sqrt(dx * dx + dy * dy);
            bin[int(floor(NUMBINS*length/max_stitchlength))]++;
        }
    }

    double binSize = max_stitchlength / NUMBINS;

    QString str;
    for (int i = 0; i < NUMBINS; i++) {
        str += QString::number(binSize * (i), 'f', 1) + " - " + QString::number(binSize * (i+1), 'f', 1) + " mm: " +  QString::number(bin[i]) + "\n\n";
    }

    puts("Stitches: %d\n", num_stitches);
    puts("Colors: %d\n", num_colors);
    puts("Jumps: %d\n", jump_stitches);
    puts("Top: %f mm", bounds.top);
    puts("Left: %f mm", bounds.left);
    puts("Bottom: %f mm", bounds.bottom);
    puts("Right: %f mm", bounds.right);
    puts("Width: %f mm", bounds.right - bounds.left);
    puts("Height: %f mm", bounds.bottom - bounds.top);
    grid->addWidget(new QLabel(tr("\nStitch Distribution: \n")),9,0,1,2);
    grid->addWidget(new QLabel(str), 10, 0, 1, 1);
    grid->addWidget(new QLabel(tr("\nThread Length By Color: \n")),11,0,1,2);
    int currentRow = 12;

    for (int i = 0; i < num_colors; i++) {
        QFrame *frame = new QFrame();
        frame->setGeometry(0,0,30,30);
        QPalette palette = frame->palette();
        EmbColor t = embThreadList_getAt(pattern->threadList, i).color;
        palette.setColor(backgroundRole(), QColor( t.r, t.g, t.b ) );
        frame->setPalette( palette );
        frame->setAutoFillBackground(true);
        grid->addWidget(frame, currentRow,0,1,1);
        debug_message("size: %d i: %d", stitchLengths.size(), i);
        grid->addWidget(new QLabel(QString::number(stitchLengths.at(i)) + " mm"), currentRow,1,1,1);
        currentRow++;
    }

    QDialogButtonBox buttonbox(Qt::Horizontal, &dialog);
    QPushButton button(&dialog);
    button.setText("Ok");
    buttonbox.addButton(&button, QDialogButtonBox::AcceptRole);
    buttonbox.setCenterButtons(true);
    connect(&buttonbox, SIGNAL(accepted()), &dialog, SLOT(accept()));

    grid->addWidget(&buttonbox, currentRow, 0, 1, 2);
*/
}


/*
 *
 */
int
convert(const char *inf, const char *outf)
{
    EmbPattern* p = 0;
    int reader, writer;

    p = embPattern_create();
    if (!p) {
        printf("ERROR: convert(), cannot allocate memory for p\n");
        return 1;
    }

    if (!embPattern_readAuto(p, inf)) {
        printf("ERROR: convert(), reading file was unsuccessful: %s\n", inf);
        embPattern_free(p);
        return 1;
    }

    reader = emb_identify_format(inf);
    writer = emb_identify_format(outf);
    if (formatTable[reader].type == EMBFORMAT_OBJECTONLY) {
        if (formatTable[writer].type == EMBFORMAT_STITCHONLY) {
            embPattern_movePolylinesToStitchList(p);
        }
    }

    if (!embPattern_writeAuto(p, outf)) {
        printf("ERROR: convert(), writing file %s was unsuccessful\n", outf);
        embPattern_free(p);
        return 1;
    }

    embPattern_free(p);
    return 0;
}

