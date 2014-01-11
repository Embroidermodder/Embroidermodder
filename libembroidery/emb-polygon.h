/*! @file emb-polygon.h */
#ifndef EMB_POLYGON_H
#define EMB_POLYGON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"
#include "emb-point.h"

typedef struct EmbPolygonObject_
{
    EmbPointList* pointList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolygonObject;

typedef struct EmbPolygonObjectList_
{
    EmbPolygonObject* polygonObj;
    struct EmbPolygonObjectList_* next;
} EmbPolygonObjectList;

EmbPolygonObjectList* embPolygonObjectList_create(EmbPolygonObject* data);
EmbPolygonObjectList* embPolygonObjectList_add(EmbPolygonObjectList* pointer, EmbPolygonObject* data);
int embPolygonObjectList_count(EmbPolygonObjectList* pointer);
int embPolygonObjectList_empty(EmbPolygonObjectList* pointer);
void embPolygonObjectList_free(EmbPolygonObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_POLYGON_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
