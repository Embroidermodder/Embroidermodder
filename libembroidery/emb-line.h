/*! @file emb-line.h */
#ifndef EMB_LINE_H
#define EMB_LINE_H

#include "emb-color.h"
#include "emb-vector.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbLine_
{
    double x1;
    double y1;
    double x2;
    double y2;
} EmbLine;

extern EMB_PUBLIC double EMB_CALL embLine_x1(EmbLine line);
extern EMB_PUBLIC double EMB_CALL embLine_y1(EmbLine line);
extern EMB_PUBLIC double EMB_CALL embLine_x2(EmbLine line);
extern EMB_PUBLIC double EMB_CALL embLine_y2(EmbLine line);

typedef struct EmbLineObject_
{
    EmbLine line;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbLineObject;

extern EMB_PUBLIC EmbLineObject EMB_CALL embLineObject_make(double x1, double y1, double x2, double y2);
extern EMB_PUBLIC EmbLineObject* EMB_CALL embLineObject_create(double x1, double y1, double x2, double y2);

typedef struct EmbLineObjectList_
{
    EmbLineObject lineObj;
    struct EmbLineObjectList_* next;
} EmbLineObjectList;

extern EMB_PUBLIC EmbLineObjectList* EMB_CALL embLineObjectList_create(EmbLineObject data);
extern EMB_PUBLIC EmbLineObjectList* EMB_CALL embLineObjectList_add(EmbLineObjectList* pointer, EmbLineObject data);
extern EMB_PUBLIC int EMB_CALL embLineObjectList_count(EmbLineObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embLineObjectList_empty(EmbLineObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embLineObjectList_free(EmbLineObjectList* pointer);

extern EMB_PUBLIC void EMB_CALL embLine_normalVector(EmbVector vector1, EmbVector vector2, EmbVector* result, int clockwise);
extern EMB_PUBLIC void EMB_CALL embLine_intersectionPoint(EmbVector v1, EmbVector v2, EmbVector v3, EmbVector v4, EmbVector* result);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_LINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
