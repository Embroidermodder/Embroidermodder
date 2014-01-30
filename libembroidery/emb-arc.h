/*! @file emb-arc.h */
#ifndef EMB_ARC_H
#define EMB_ARC_H

#include "emb-color.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

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

extern EMB_PUBLIC EmbArcObject EMB_CALL embArcObject_make(double sx, double sy, double mx, double my, double ex, double ey);
extern EMB_PUBLIC EmbArcObject* EMB_CALL embArcObject_create(double sx, double sy, double mx, double my, double ex, double ey);

typedef struct EmbArcObjectList_
{
    EmbArcObject arcObj;
    struct EmbArcObjectList_* next;
} EmbArcObjectList;

extern EMB_PUBLIC EmbArcObjectList* EMB_CALL embArcObjectList_add(EmbArcObjectList* pointer, EmbArcObject data);
extern EMB_PUBLIC int EMB_CALL embArcObjectList_count(EmbArcObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embArcObjectList_empty(EmbArcObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embArcObjectList_free(EmbArcObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_ARC_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
