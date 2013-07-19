#ifndef EMB_SPLINE_H
#define EMB_SPLINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

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
} EmbSplineObjectList;

int embSpline_count(EmbSplineObjectList* pointer);
int embSpline_empty(EmbSplineObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_SPLINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
