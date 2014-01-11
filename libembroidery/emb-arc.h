/*! @file emb-arc.h */
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

EmbArcObject embArcObject_make(double sx, double sy, double mx, double my, double ex, double ey);
EmbArcObject* embArcObject_create(double sx, double sy, double mx, double my, double ex, double ey);

typedef struct EmbArcObjectList_
{
    EmbArcObject arcObj;
    struct EmbArcObjectList_* next;
} EmbArcObjectList;

EmbArcObjectList* embArcObjectList_add(EmbArcObjectList* pointer, EmbArcObject data);
int embArcObjectList_count(EmbArcObjectList* pointer);
int embArcObjectList_empty(EmbArcObjectList* pointer);
void embArcObjectList_free(EmbArcObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_ARC_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
