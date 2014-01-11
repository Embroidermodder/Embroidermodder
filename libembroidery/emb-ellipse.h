/*! @file emb-ellipse.h */
#ifndef EMB_ELLIPSE_H
#define EMB_ELLIPSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

typedef struct EmbEllipse_
{
    double centerX;
    double centerY;
    double radiusX;
    double radiusY;
} EmbEllipse;

double embEllipse_centerX(EmbEllipse ellipse);
double embEllipse_centerY(EmbEllipse ellipse);
double embEllipse_radiusX(EmbEllipse ellipse);
double embEllipse_radiusY(EmbEllipse ellipse);
double embEllipse_diameterX(EmbEllipse ellipse);
double embEllipse_diameterY(EmbEllipse ellipse);
double embEllipse_width(EmbEllipse ellipse);
double embEllipse_height(EmbEllipse ellipse);

typedef struct EmbEllipseObject_
{
    EmbEllipse ellipse;
    double rotation;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbEllipseObject;

EmbEllipseObject embEllipseObject_make(double cx, double cy, double rx, double ry);
EmbEllipseObject* embEllipseObject_create(double cx, double cy, double rx, double ry);

typedef struct EmbEllipseObjectList_
{
    EmbEllipseObject ellipseObj;
    struct EmbEllipseObjectList_* next;
} EmbEllipseObjectList;

EmbEllipseObjectList* embEllipseObjectList_add(EmbEllipseObjectList* pointer, EmbEllipseObject data);
int embEllipseObjectList_count(EmbEllipseObjectList* pointer);
int embEllipseObjectList_empty(EmbEllipseObjectList* pointer);
void embEllipseObjectList_free(EmbEllipseObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_ELLIPSE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
