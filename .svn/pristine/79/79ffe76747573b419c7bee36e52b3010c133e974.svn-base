#ifndef EMB_PATTERN_H
#define EMB_PATTERN_H

/* Disable warnings about unsafe use of fopen, fseek etc */
#pragma warning(disable: 4996)

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-stitch.h"
#include "emb-thread.h"

#include "emb-arc.h"
#include "emb-circle.h"
#include "emb-ellipse.h"
#include "emb-line.h"
#include "emb-settings.h"
#include "emb-path.h"
#include "emb-point.h"
#include "emb-polygon.h"
#include "emb-polyline.h"
#include "emb-rect.h"
#include "emb-spline.h"

typedef struct EmbPattern_
{
    EmbSettings settings;
    EmbStitchList* stitchList;
    EmbThreadList* threadList;

    EmbArcObjectList* arcObjList;
    EmbCircleObjectList* circleObjList;
    EmbEllipseObjectList* ellipseObjList;
    EmbLineObjectList* lineObjList;
    EmbPathObjectList* pathObjList;
    EmbPointObjectList* pointObjList;
    EmbPolygonObjectList* polygonObjList;
    EmbPolylineObjectList* polylineObjList;
    EmbRectObjectList* rectObjList;
    EmbSplineObjectList* splineObjList;

    EmbStitchList* lastStitch;
    EmbThreadList* lastThread;

    EmbArcObjectList* lastArcObj;
    EmbCircleObjectList* lastCircleObj;
    EmbEllipseObjectList* lastEllipseObj;
    EmbLineObjectList* lastLineObj;
    EmbPathObjectList* lastPathObj;
    EmbPointObjectList* lastPointObj;
    EmbPolygonObjectList* lastPolygonObj;
    EmbPolylineObjectList* lastPolylineObj;
    EmbRectObjectList* lastRectObj;
    EmbSplineObjectList* lastSplineObj;

    int currentColorIndex;
    double lastX;
    double lastY;
} EmbPattern;

EmbPattern* embPattern_create();
void embPattern_hideStitchesOverLength(EmbPattern* p, int length);
void embPattern_fixColorCount(EmbPattern* p);
void embPattern_addThread(EmbPattern* p, EmbThread thread);
void embPattern_addStitchAbs(EmbPattern* p, double x, double y, int flags, int isAutoColorIndex);
void embPattern_addStitchRel(EmbPattern* p, double dx, double dy, int flags, int isAutoColorIndex);
void embPattern_changeColor(EmbPattern* p, int index);
void embPattern_free(EmbPattern* p);
EmbPattern* embPattern_read(const char* fileName);
int embPattern_write(EmbPattern* p, const char *fileName);
void embPattern_scale(EmbPattern* p, double scale);
EmbRect embPattern_calcBoundingBox(EmbPattern* p);
void embPattern_flipVertical(EmbPattern* p);
void embPattern_correctForMaxStitchLength(EmbPattern* p, double maxStitchLength, double maxJumpLength);
void embPattern_center(EmbPattern* p);
void embPattern_loadExternalColorFile(EmbPattern* p, const char *fileName);

void embPattern_addCircleObjectAbs(EmbPattern* p, double cx, double cy, double r);
void embPattern_addEllipseObjectAbs(EmbPattern* p, double cx, double cy, double rx, double ry); /* TODO: ellipse rotation */
void embPattern_addLineObjectAbs(EmbPattern* p, double x1, double y1, double x2, double y2);
void embPattern_addPathObjectAbs(EmbPattern* p, EmbPathObject* pathObj);
void embPattern_addPointObjectAbs(EmbPattern* p, double x, double y);
void embPattern_addRectObjectAbs(EmbPattern* p, double x, double y, double w, double h);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_PATTERN_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
