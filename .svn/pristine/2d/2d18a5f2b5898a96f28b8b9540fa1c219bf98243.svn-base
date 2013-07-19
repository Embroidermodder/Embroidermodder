#ifndef EMB_POINT_H
#define EMB_POINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

typedef struct EmbPoint_
{
    double xx; /* absolute position (not relative) */
    double yy;
} EmbPoint;

double embPoint_x(EmbPoint point);
double embPoint_y(EmbPoint point);

typedef struct EmbPointList_
{
    EmbPoint point;
    struct EmbPointList_* next;
} EmbPointList;

typedef struct EmbPointObject_
{
    EmbPoint point;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPointObject;

typedef struct EmbPointObjectList_
{
    EmbPointObject pointObj;
    struct EmbPointObjectList_* next;
} EmbPointObjectList;

EmbPointObject embPointObj_make(double x, double y);
EmbPointObject* embPointObj_create(double x, double y);

void embPoint_add(EmbPointObjectList* pointer, EmbPointObject data);
int embPoint_count(EmbPointObjectList* pointer);
int embPoint_empty(EmbPointObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_POINT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
