#ifndef EMB_ARC_H
#define EMB_ARC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

typedef struct EmbArc_
{
    double startX;  /* absolute position (not relative) */
    double startY;

    double midX;    /* absolute position (not relative) */
    double midY;

    double endX;    /* absolute position (not relative) */
    double endY;
} EmbArc;

typedef struct EmbArcObject_
{
    EmbArc arc;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbArcObject;

typedef struct EmbArcObjectList_
{
    EmbArcObject arcObj;
    struct EmbArcObjectList_* next;
} EmbArcObjectList;

EmbArcObject embArcObj_make(double sx, double sy, double mx, double my, double ex, double ey);
EmbArcObject* embArcObj_create(double sx, double sy, double mx, double my, double ex, double ey);

void embArc_add(EmbArcObjectList* pointer, EmbArcObject data);
int embArc_count(EmbArcObjectList* pointer);
int embArc_empty(EmbArcObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_ARC_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
