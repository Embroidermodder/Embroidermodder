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

typedef struct EmbEllipseObjectList_
{
    EmbEllipseObject ellipseObj;
    struct EmbEllipseObjectList_* next;
} EmbEllipseObjectList;

EmbEllipseObject embEllipseObj_make(double cx, double cy, double rx, double ry);
EmbEllipseObject* embEllipseObj_create(double cx, double cy, double rx, double ry);

void embEllipse_add(EmbEllipseObjectList* pointer, EmbEllipseObject data);
int embEllipse_count(EmbEllipseObjectList* pointer);
int embEllipse_empty(EmbEllipseObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_ELLIPSE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
