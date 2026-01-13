/*! @file emb-polygon.h */
#ifndef EMB_POLYGON_H
#define EMB_POLYGON_H

#include "emb-color.h"
#include "emb-point.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbPolygonObject_
{
    EmbPointList* pointList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolygonObject;

extern EMB_PUBLIC EmbPolygonObject* EMB_CALL embPolygonObject_create(EmbPointList* pointList, EmbColor color, int lineType);
extern EMB_PUBLIC void EMB_CALL embPolygonObject_free(EmbPolygonObject* pointer);

typedef struct EmbPolygonObjectList_
{
    EmbPolygonObject* polygonObj;
    struct EmbPolygonObjectList_* next;
} EmbPolygonObjectList;

extern EMB_PUBLIC EmbPolygonObjectList* EMB_CALL embPolygonObjectList_create(EmbPolygonObject* data);
extern EMB_PUBLIC EmbPolygonObjectList* EMB_CALL embPolygonObjectList_add(EmbPolygonObjectList* pointer, EmbPolygonObject* data);
extern EMB_PUBLIC int EMB_CALL embPolygonObjectList_count(EmbPolygonObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embPolygonObjectList_empty(EmbPolygonObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embPolygonObjectList_free(EmbPolygonObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_POLYGON_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
