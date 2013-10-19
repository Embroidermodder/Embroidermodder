#ifndef EMB_POINT_H
#define EMB_POINT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

typedef struct EmbPoint_
{
    double xx; /* absolute position (not relative) */
    double yy; /* positive is up, units are in mm  */
} EmbPoint;

double embPoint_x(EmbPoint point);
double embPoint_y(EmbPoint point);
EmbPoint embPoint_make(double x, double y);

typedef struct EmbPointList_
{
    EmbPoint point;
    struct EmbPointList_* next;
} EmbPointList;

EmbPointList* embPointList_create(double x, double y);
EmbPointList* embPointList_add(EmbPointList* pointer, EmbPoint data);
int embPointList_count(EmbPointList* pointer);
int embPointList_empty(EmbPointList* pointer);
void embPointList_free(EmbPointList* list);

typedef struct EmbPointObject_
{
    EmbPoint point;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPointObject;

EmbPointObject embPointObject_make(double x, double y);
EmbPointObject* embPointObject_create(double x, double y);

typedef struct EmbPointObjectList_
{
    EmbPointObject pointObj;
    struct EmbPointObjectList_* next;
} EmbPointObjectList;

EmbPointObjectList* embPointObjectList_add(EmbPointObjectList* pointer, EmbPointObject data);
int embPointObjectList_count(EmbPointObjectList* pointer);
int embPointObjectList_empty(EmbPointObjectList* pointer);
void embPointObjectList_free(EmbPointObjectList* list);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_POINT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
