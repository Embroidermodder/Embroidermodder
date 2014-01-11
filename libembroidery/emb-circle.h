/*! @file emb-circle.h */
#ifndef EMB_CIRCLE_H
#define EMB_CIRCLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

typedef struct EmbCircle_
{
    double centerX;
    double centerY;
    double radius;
} EmbCircle;

double embCircle_centerX(EmbCircle circle);
double embCircle_centerY(EmbCircle circle);
double embCircle_radius(EmbCircle circle);

typedef struct EmbCircleObject_
{
    EmbCircle circle;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbCircleObject;

EmbCircleObject embCircleObject_make(double cx, double cy, double r);
EmbCircleObject* embCircleObject_create(double cx, double cy, double r);

typedef struct EmbCircleObjectList_
{
    EmbCircleObject circleObj;
    struct EmbCircleObjectList_* next;
} EmbCircleObjectList;

EmbCircleObjectList* embCircleObjectList_add(EmbCircleObjectList* pointer, EmbCircleObject data);
int embCircleObjectList_count(EmbCircleObjectList* pointer);
int embCircleObjectList_empty(EmbCircleObjectList* pointer);
void embCircleObjectList_free(EmbCircleObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_CIRCLE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
