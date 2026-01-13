/*! @file emb-circle.h */
#ifndef EMB_CIRCLE_H
#define EMB_CIRCLE_H

#include "emb-color.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbCircle_
{
    double centerX;
    double centerY;
    double radius;
} EmbCircle;

extern EMB_PUBLIC double EMB_CALL embCircle_centerX(EmbCircle circle);
extern EMB_PUBLIC double EMB_CALL embCircle_centerY(EmbCircle circle);
extern EMB_PUBLIC double EMB_CALL embCircle_radius(EmbCircle circle);

typedef struct EmbCircleObject_
{
    EmbCircle circle;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbCircleObject;

extern EMB_PUBLIC EmbCircleObject EMB_CALL embCircleObject_make(double cx, double cy, double r);
extern EMB_PUBLIC EmbCircleObject* EMB_CALL embCircleObject_create(double cx, double cy, double r);

typedef struct EmbCircleObjectList_
{
    EmbCircleObject circleObj;
    struct EmbCircleObjectList_* next;
} EmbCircleObjectList;

extern EMB_PUBLIC EmbCircleObjectList* EMB_CALL embCircleObjectList_create(EmbCircleObject data);
extern EMB_PUBLIC EmbCircleObjectList* EMB_CALL embCircleObjectList_add(EmbCircleObjectList* pointer, EmbCircleObject data);
extern EMB_PUBLIC int EMB_CALL embCircleObjectList_count(EmbCircleObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embCircleObjectList_empty(EmbCircleObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embCircleObjectList_free(EmbCircleObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_CIRCLE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
