/*
 * This file is part of libembroidery.
 *
 * Copyright 2018-2022 The Embroidermodder Team
 * Licensed under the terms of the zlib license.
 *
 * ---------------------------------------------------------
 *
 * The array management for libembroidery's arrays.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "embroidery.h"

EmbArray*
embArray_create(int type)
{
    int i;
    EmbArray *p;
    p = (EmbArray*)malloc(sizeof(EmbArray));
    p->type = type;
    p->length = CHUNK_SIZE;
    p->count = 0;
    switch (p->type) {
    case EMB_ARC:
        p->arc = (EmbArc*)malloc(CHUNK_SIZE*sizeof(EmbArc));
        break;
    case EMB_CIRCLE:
        p->circle = (EmbCircle*)malloc(CHUNK_SIZE*sizeof(EmbCircle));
        break;
    case EMB_ELLIPSE:
        p->ellipse = (EmbEllipse*)malloc(CHUNK_SIZE*sizeof(EmbEllipse));
        break;
    case EMB_FLAG:
        p->flag = (int*)malloc(CHUNK_SIZE*sizeof(int));
        break;
    case EMB_PATH:
        p->path = (EmbPath*)malloc(CHUNK_SIZE*sizeof(EmbPath));
        for (i=0; i<CHUNK_SIZE; i++) {
            p->path[i].pointList = embArray_create(EMB_POINT);
        }
        break;
    case EMB_POINT:
        p->point = (EmbPoint*)malloc(CHUNK_SIZE*sizeof(EmbPoint));
        break;
    case EMB_LINE:
        p->line = (EmbLine*)malloc(CHUNK_SIZE*sizeof(EmbLine));
        break;
    case EMB_POLYGON:
        p->polygon = (EmbPolygon*)malloc(CHUNK_SIZE*sizeof(EmbPolygon));
        for (i=0; i<CHUNK_SIZE; i++) {
            p->polygon[i].pointList = embArray_create(EMB_POINT);
            p->polygon[i].flagList = embArray_create(EMB_FLAG);
        }
        break;
    case EMB_POLYLINE:
        p->polyline = (EmbPolyline*)malloc(CHUNK_SIZE*sizeof(EmbPolyline));
        for (i=0; i<CHUNK_SIZE; i++) {
            p->polyline[i].pointList = embArray_create(EMB_POINT);
            p->polyline[i].flagList = embArray_create(EMB_FLAG);
        }
        break;
    case EMB_RECT:
        p->rect = (EmbRect*)malloc(CHUNK_SIZE*sizeof(EmbRect));
        break;
    case EMB_SPLINE:
        p->spline = (EmbSpline*)malloc(CHUNK_SIZE*sizeof(EmbSpline));
        break;
    case EMB_STITCH:
        p->stitch = (EmbStitch*)malloc(CHUNK_SIZE*sizeof(EmbStitch));
        break;
    case EMB_VECTOR:
        p->vector = (EmbVector*)malloc(CHUNK_SIZE*sizeof(EmbVector));
        break;
    default:
        break;
    }
    return p;
}

int
embArray_resize(EmbArray *p)
{
    int i;
    if (p->count < p->length) {
        return 1;
    }
    p->length += CHUNK_SIZE;
    switch (p->type) {
    case EMB_ARC:
        p->arc = (EmbArc *)realloc(p->arc, p->length*sizeof(EmbArc));
        if (!p->arc) return 0;
        break;
    case EMB_CIRCLE:
        p->circle = (EmbCircle *)realloc(p->circle, p->length*sizeof(EmbCircle));
        if (!p->circle) return 0;
        break;
    case EMB_ELLIPSE:
        p->ellipse = (EmbEllipse *)realloc(p->ellipse, p->length*sizeof(EmbEllipse));
        if (!p->ellipse) return 0;
        break;
    case EMB_FLAG:
        p->flag = (int *)realloc(p->flag, p->length*sizeof(int));
        if (!p->flag) return 0;
        break;
    case EMB_PATH:
        p->path = (EmbPath *)realloc(p->path, p->length*sizeof(EmbPath*));
        if (!p->path) return 0;
        break;
    case EMB_POINT:
        p->point = (EmbPoint *)realloc(p->point, p->length*sizeof(EmbPoint));
        if (!p->point) return 0;
        break;
    case EMB_LINE:
        p->line = (EmbLine *)realloc(p->line, p->length*sizeof(EmbLine));
        if (!p->line) return 0;
        break;
    case EMB_POLYGON:
        p->polygon = (EmbPolygon *)realloc(p->polygon, p->length*sizeof(EmbPolygon));
        if (p->polygon == NULL) {
            return 0;
        }
        break;
    case EMB_POLYLINE:
        p->polyline = (EmbPolyline *)realloc(p->polyline, p->length*sizeof(EmbPolyline));
        if (!p->polyline) return 0;
        for (i=0; i<CHUNK_SIZE; i++) {
            p->polyline[i].pointList = embArray_create(EMB_POINT);
            if (!p->polyline) return 0;
            p->polyline[i].flagList = embArray_create(EMB_FLAG);
        }
        break;
    case EMB_RECT:
        p->rect = (EmbRect *)realloc(p->rect, p->length*sizeof(EmbRect));
        if (!p->rect) return 0;
        break;
    case EMB_SPLINE:
        p->spline = (EmbSpline *)realloc(p->spline, p->length*sizeof(EmbSpline));
        if (!p->spline) return 0;
        break;
    case EMB_STITCH:
        p->stitch = (EmbStitch *)realloc(p->stitch, p->length*sizeof(EmbStitch));
        if (!p->stitch) return 0;
        break;
    case EMB_VECTOR:
        p->vector = (EmbVector *)realloc(p->vector, p->length*sizeof(EmbVector));
        if (!p->vector) return 0;
        break;
    default:
        break;
    }
    return 1;
}

void embArray_copy(EmbArray *dst, EmbArray *src)
{
    dst = (EmbArray*)malloc(sizeof(EmbArray));
    dst->type = src->type;
    dst->length = src->length;
    dst->count = src->count;
    embArray_resize(dst);
    switch (src->type) {
    case EMB_ARC:
        memcpy(dst->arc, src->arc, sizeof(EmbArc)*src->count);
        break;
    case EMB_CIRCLE:
        memcpy(dst->circle, src->circle, sizeof(EmbCircle)*src->count);
        break;
    case EMB_ELLIPSE:
        memcpy(dst->ellipse, src->ellipse, sizeof(EmbEllipse)*src->count);
        break;
    case EMB_FLAG:
        memcpy(dst->flag, src->flag, sizeof(int)*src->count);
        break;
    case EMB_PATH:
        memcpy(dst->path, src->path, sizeof(EmbPath)*src->count);
        break;
    case EMB_POINT:
        memcpy(dst->point, src->point, sizeof(EmbPoint)*src->count);
        break;
    case EMB_LINE:
        memcpy(dst->line, src->line, sizeof(EmbLine)*src->count);
        break;
    case EMB_POLYGON:
        memcpy(dst->polygon, src->polygon, sizeof(int)*src->count);
        break;
    case EMB_POLYLINE:
        memcpy(dst->polyline, src->polyline, sizeof(int)*src->count);
        break;
    case EMB_RECT:
        memcpy(dst->rect, src->rect, sizeof(int)*src->count);
        break;
    case EMB_SPLINE:
        memcpy(dst->spline, src->spline, sizeof(int)*src->count);
        break;
    case EMB_STITCH:
        memcpy(dst->stitch, src->stitch, sizeof(int)*src->count);
        break;
    case EMB_VECTOR:
        memcpy(dst->vector, src->vector, sizeof(int)*src->count);
        break;
    default:
        break;
    }
}

#define addGeometry(A, B) \
    int \
    embArray_add##A(EmbArray *a, Emb##A B) \
    { \
        a->count++; \
        if (!embArray_resize(a)) { \
            return 0; \
        } \
        a->B[a->count - 1] = B; \
        return 1; \
    }

addGeometry(Arc, arc)
addGeometry(Circle, circle)
addGeometry(Ellipse, ellipse)
addGeometry(Flag, flag)
addGeometry(Line, line)
addGeometry(Path, path)
addGeometry(Point, point)
addGeometry(Polygon, polygon)
addGeometry(Polyline, polyline)
addGeometry(Rect, rect)
addGeometry(Stitch, stitch)
addGeometry(Vector, vector)


void embArray_free(EmbArray* p) {
    int i;
    if (!p) {
        return;
    }
    switch (p->type) {
    case EMB_ARC:
        free(p->arc);
        break;
    case EMB_CIRCLE:
        free(p->circle);
        break;
    case EMB_ELLIPSE:
        free(p->ellipse);
        break;
    case EMB_FLAG:
        free(p->flag);
        break;
    case EMB_LINE:
        free(p->line);
        break;
    case EMB_PATH:
        for (i = 0; i < p->count; i++) {
            embArray_free(p->path[i].pointList);
        }
        free(p->path);
        break;
    case EMB_POINT:
        free(p->point);
        break;
    case EMB_POLYGON:
        for (i = 0; i < p->count; i++) {
            embArray_free(p->polygon[i].pointList);
        }
        free(p->polygon);
        break;
    case EMB_POLYLINE:
        for (i = 0; i < p->count; i++) {
            embArray_free(p->polyline[i].pointList);
        }
        free(p->polyline);
        break;
    case EMB_RECT:
        free(p->rect);
        break;
    case EMB_SPLINE:
        free(p->spline);
        break;
    case EMB_STITCH:
        free(p->stitch);
        break;
    case EMB_VECTOR:
        free(p->vector);
        break;
    default:
        break;
    }
    free(p);
}

