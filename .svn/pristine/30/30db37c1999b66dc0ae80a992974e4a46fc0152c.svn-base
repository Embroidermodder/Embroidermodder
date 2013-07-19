#ifndef EMB_LINE_H
#define EMB_LINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

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

typedef struct EmbLineObjectList_
{
    EmbLineObject lineObj;
    struct EmbLineObjectList_* next;
} EmbLineObjectList;

EmbLineObject embLineObj_make(double x1, double y1, double x2, double y2);
EmbLineObject* embLineObj_create(double x1, double y1, double x2, double y2);

void embLine_add(EmbLineObjectList* pointer, EmbLineObject data);
int embLine_count(EmbLineObjectList* pointer);
int embLine_empty(EmbLineObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_LINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
