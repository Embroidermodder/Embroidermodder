/*! @file emb-spline.h */
#ifndef EMB_SPLINE_H
#define EMB_SPLINE_H

#include "emb-color.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbBezier_
{
    double startX;
    double startY;
    double control1X;
    double control1Y;
    double control2X;
    double control2Y;
    double endX;
    double endY;
} EmbBezier;

typedef struct EmbSplineObject_
{
    EmbBezier bezier;
    struct EmbSplineObject_* next;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbSplineObject;

/* A list of bezier curves is a B-spline */
typedef struct EmbSplineObjectList_
{
    EmbSplineObject splineObj;
    struct EmbSplineObjectList_* next;
} EmbSplineObjectList; /* TODO: This struct/file needs reworked to work internally similar to polylines */

extern EMB_PUBLIC int EMB_CALL embSplineObjectList_count(EmbSplineObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embSplineObjectList_empty(EmbSplineObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_SPLINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
