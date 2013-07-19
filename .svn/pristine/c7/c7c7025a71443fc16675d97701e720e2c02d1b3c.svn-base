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

typedef struct EmbCircleObjectList_
{
    EmbCircleObject circleObj;
    struct EmbCircleObjectList_* next;
} EmbCircleObjectList;

EmbCircleObject embCircleObj_make(double cx, double cy, double r);
EmbCircleObject* embCircleObj_create(double cx, double cy, double r);

void embCircle_add(EmbCircleObjectList* pointer, EmbCircleObject data);
int embCircle_count(EmbCircleObjectList* pointer);
int embCircle_empty(EmbCircleObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_CIRCLE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
