#ifndef EMB_POLYLINE_H
#define EMB_POLYLINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"
#include "emb-point.h"

typedef struct EmbPolylineObject_
{
    EmbPointList* pointList;
    int lineType;
    EmbColor color;
} EmbPolylineObject;

typedef struct EmbPolylineObjectList_
{
    EmbPolylineObject* polylineObj;
    struct EmbPolylineObjectList_* next;
} EmbPolylineObjectList;

EmbPolylineObjectList* embPolylineObjectList_create(EmbPolylineObject* data);
EmbPointObjectList* embPolylineObjectList_add(EmbPolylineObjectList* pointer, EmbPolylineObject* data);
int embPolylineObjectList_count(EmbPolylineObjectList* pointer);
int embPolylineObjectList_empty(EmbPolylineObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_POLYLINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
