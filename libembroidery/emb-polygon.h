#ifndef EMB_POLYGON_H
#define EMB_POLYGON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"
#include "emb-point.h"

typedef struct EmbPolygonObject_
{
    EmbPoint polygonPoint;
    EmbPointList* nextPolygonPoint;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolygonObject;

typedef struct EmbPolygonObjectList_
{
    EmbPolygonObject polygonObj;
    struct EmbPolygonObjectList_* next;
} EmbPolygonObjectList;

int embPolygon_count(EmbPolygonObjectList* pointer);
int embPolygon_empty(EmbPolygonObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_POLYGON_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
