#ifndef EMB_POLYLINE_H
#define EMB_POLYLINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"
#include "emb-point.h"

typedef struct EmbPolylineObject_
{
    EmbPoint polylinePoint;
    EmbPointList* nextPolylinePoint;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolylineObject;

typedef struct EmbPolylineObjectList_
{
    EmbPolylineObject polylineObj;
    struct EmbPolylineObjectList_* next;
} EmbPolylineObjectList;

int embPolyline_count(EmbPolylineObjectList* pointer);
int embPolyline_empty(EmbPolylineObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_POLYLINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
