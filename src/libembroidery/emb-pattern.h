/*! @file emb-pattern.h */
#ifndef EMB_PATTERN_H
#define EMB_PATTERN_H

#include "emb-arc.h"
#include "emb-circle.h"
#include "emb-ellipse.h"
#include "emb-hoop.h"
#include "emb-line.h"
#include "emb-path.h"
#include "emb-point.h"
#include "emb-polygon.h"
#include "emb-polyline.h"
#include "emb-rect.h"
#include "emb-settings.h"
#include "emb-spline.h"
#include "emb-stitch.h"
#include "emb-thread.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbPattern_
{
    EmbSettings settings;
    EmbHoop hoop;
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

extern EMB_PUBLIC EmbPattern* EMB_CALL embPattern_create(void);
extern EMB_PUBLIC void EMB_CALL embPattern_hideStitchesOverLength(EmbPattern* p, int length);
extern EMB_PUBLIC void EMB_CALL embPattern_fixColorCount(EmbPattern* p);
extern EMB_PUBLIC int EMB_CALL embPattern_addThread(EmbPattern* p, EmbThread thread);
extern EMB_PUBLIC void EMB_CALL embPattern_addStitchAbs(EmbPattern* p, double x, double y, int flags, int isAutoColorIndex);
extern EMB_PUBLIC void EMB_CALL embPattern_addStitchRel(EmbPattern* p, double dx, double dy, int flags, int isAutoColorIndex);
extern EMB_PUBLIC void EMB_CALL embPattern_changeColor(EmbPattern* p, int index);
extern EMB_PUBLIC void EMB_CALL embPattern_free(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_scale(EmbPattern* p, double scale);
extern EMB_PUBLIC EmbRect EMB_CALL embPattern_calcBoundingBox(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_flipHorizontal(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_flipVertical(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_flip(EmbPattern* p, int horz, int vert);
extern EMB_PUBLIC void EMB_CALL embPattern_combineJumpStitches(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_correctForMaxStitchLength(EmbPattern* p, double maxStitchLength, double maxJumpLength);
extern EMB_PUBLIC void EMB_CALL embPattern_center(EmbPattern* p);
extern EMB_PUBLIC void EMB_CALL embPattern_loadExternalColorFile(EmbPattern* p, const char* fileName);

extern EMB_PUBLIC void EMB_CALL embPattern_addCircleObjectAbs(EmbPattern* p, double cx, double cy, double r);
extern EMB_PUBLIC void EMB_CALL embPattern_addEllipseObjectAbs(EmbPattern* p, double cx, double cy, double rx, double ry); /* TODO: ellipse rotation */
extern EMB_PUBLIC void EMB_CALL embPattern_addLineObjectAbs(EmbPattern* p, double x1, double y1, double x2, double y2);
extern EMB_PUBLIC void EMB_CALL embPattern_addPathObjectAbs(EmbPattern* p, EmbPathObject* obj);
extern EMB_PUBLIC void EMB_CALL embPattern_addPointObjectAbs(EmbPattern* p, double x, double y);
extern EMB_PUBLIC void EMB_CALL embPattern_addPolygonObjectAbs(EmbPattern* p, EmbPolygonObject* obj);
extern EMB_PUBLIC void EMB_CALL embPattern_addPolylineObjectAbs(EmbPattern* p, EmbPolylineObject* obj);
extern EMB_PUBLIC void EMB_CALL embPattern_addRectObjectAbs(EmbPattern* p, double x, double y, double w, double h);

extern EMB_PUBLIC void EMB_CALL embPattern_copyStitchListToPolylines(EmbPattern* pattern);
extern EMB_PUBLIC void EMB_CALL embPattern_copyPolylinesToStitchList(EmbPattern* pattern);
extern EMB_PUBLIC void EMB_CALL embPattern_moveStitchListToPolylines(EmbPattern* pattern);
extern EMB_PUBLIC void EMB_CALL embPattern_movePolylinesToStitchList(EmbPattern* pattern);

extern EMB_PUBLIC int EMB_CALL embPattern_read(EmbPattern* pattern, const char* fileName);
extern EMB_PUBLIC int EMB_CALL embPattern_write(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_PATTERN_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
