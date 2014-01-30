/*! @file emb-point.h */
#ifndef EMB_POINT_H
#define EMB_POINT_H

#include "emb-color.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbPoint_
{
    double xx; /* absolute position (not relative) */
    double yy; /* positive is up, units are in mm  */
} EmbPoint;

extern EMB_PUBLIC double EMB_CALL embPoint_x(EmbPoint point);
extern EMB_PUBLIC double EMB_CALL embPoint_y(EmbPoint point);
extern EMB_PUBLIC EmbPoint EMB_CALL embPoint_make(double x, double y);

typedef struct EmbPointList_
{
    EmbPoint point;
    struct EmbPointList_* next;
} EmbPointList;

extern EMB_PUBLIC EmbPointList* EMB_CALL embPointList_create(double x, double y);
extern EMB_PUBLIC EmbPointList* EMB_CALL embPointList_add(EmbPointList* pointer, EmbPoint data);
extern EMB_PUBLIC int EMB_CALL embPointList_count(EmbPointList* pointer);
extern EMB_PUBLIC int EMB_CALL embPointList_empty(EmbPointList* pointer);
extern EMB_PUBLIC void EMB_CALL embPointList_free(EmbPointList* pointer);

typedef struct EmbPointObject_
{
    EmbPoint point;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPointObject;

extern EMB_PUBLIC EmbPointObject EMB_CALL embPointObject_make(double x, double y);
extern EMB_PUBLIC EmbPointObject* EMB_CALL embPointObject_create(double x, double y);

typedef struct EmbPointObjectList_
{
    EmbPointObject pointObj;
    struct EmbPointObjectList_* next;
} EmbPointObjectList;

extern EMB_PUBLIC EmbPointObjectList* EMB_CALL embPointObjectList_create(EmbPointObject data);
extern EMB_PUBLIC EmbPointObjectList* EMB_CALL embPointObjectList_add(EmbPointObjectList* pointer, EmbPointObject data);
extern EMB_PUBLIC int EMB_CALL embPointObjectList_count(EmbPointObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embPointObjectList_empty(EmbPointObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embPointObjectList_free(EmbPointObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_POINT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
