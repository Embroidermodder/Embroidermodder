/*! @file emb-ellipse.h */
#ifndef EMB_ELLIPSE_H
#define EMB_ELLIPSE_H

#include "emb-color.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbEllipse_
{
    double centerX;
    double centerY;
    double radiusX;
    double radiusY;
} EmbEllipse;

extern EMB_PUBLIC double EMB_CALL embEllipse_centerX(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_centerY(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_radiusX(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_radiusY(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_diameterX(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_diameterY(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_width(EmbEllipse ellipse);
extern EMB_PUBLIC double EMB_CALL embEllipse_height(EmbEllipse ellipse);

typedef struct EmbEllipseObject_
{
    EmbEllipse ellipse;
    double rotation;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbEllipseObject;

extern EMB_PUBLIC EmbEllipseObject EMB_CALL embEllipseObject_make(double cx, double cy, double rx, double ry);
extern EMB_PUBLIC EmbEllipseObject* EMB_CALL embEllipseObject_create(double cx, double cy, double rx, double ry);

typedef struct EmbEllipseObjectList_
{
    EmbEllipseObject ellipseObj;
    struct EmbEllipseObjectList_* next;
} EmbEllipseObjectList;

extern EMB_PUBLIC EmbEllipseObjectList* EMB_CALL embEllipseObjectList_create(EmbEllipseObject data);
extern EMB_PUBLIC EmbEllipseObjectList* EMB_CALL embEllipseObjectList_add(EmbEllipseObjectList* pointer, EmbEllipseObject data);
extern EMB_PUBLIC int EMB_CALL embEllipseObjectList_count(EmbEllipseObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embEllipseObjectList_empty(EmbEllipseObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embEllipseObjectList_free(EmbEllipseObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_ELLIPSE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
