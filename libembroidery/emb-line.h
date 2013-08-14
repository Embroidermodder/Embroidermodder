#ifndef EMB_LINE_H
#define EMB_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"
#include "emb-vector.h"

typedef struct EmbLine_
{
    double x1;
    double y1;
    double x2;
    double y2;
} EmbLine;

double embLine_x1(EmbLine line);
double embLine_y1(EmbLine line);
double embLine_x2(EmbLine line);
double embLine_y2(EmbLine line);

typedef struct EmbLineObject_
{
    EmbLine line;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbLineObject;

EmbLineObject embLineObject_make(double x1, double y1, double x2, double y2);
EmbLineObject* embLineObject_create(double x1, double y1, double x2, double y2);

typedef struct EmbLineObjectList_
{
    EmbLineObject lineObj;
    struct EmbLineObjectList_* next;
} EmbLineObjectList;

void embLineObjectList_add(EmbLineObjectList* pointer, EmbLineObject data);
int embLineObjectList_count(EmbLineObjectList* pointer);
int embLineObjectList_empty(EmbLineObjectList* pointer);

void embLine_GetPerpendicularCWVector(EmbVector vector1, EmbVector vector2, EmbVector* result); /* TODO: API cleanup: This belongs in emb-vector.h */
void embLine_IntersectionWith(EmbVector v1, EmbVector v2, EmbVector v3, EmbVector v4, EmbVector* result); /* TODO: API cleanup: This belongs in emb-vector.h */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_LINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
