/*! @file emb-polyline.h */
#ifndef EMB_POLYLINE_H
#define EMB_POLYLINE_H

#include "emb-color.h"
#include "emb-point.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbPolylineObject_
{
    EmbPointList* pointList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPolylineObject;

extern EMB_PUBLIC EmbPolylineObject* EMB_CALL embPolylineObject_create(EmbPointList* pointList, EmbColor color, int lineType);
extern EMB_PUBLIC void EMB_CALL embPolylineObject_free(EmbPolylineObject* pointer);

typedef struct EmbPolylineObjectList_
{
    EmbPolylineObject* polylineObj;
    struct EmbPolylineObjectList_* next;
} EmbPolylineObjectList;

extern EMB_PUBLIC EmbPolylineObjectList* EMB_CALL embPolylineObjectList_create(EmbPolylineObject* data);
extern EMB_PUBLIC EmbPolylineObjectList* EMB_CALL embPolylineObjectList_add(EmbPolylineObjectList* pointer, EmbPolylineObject* data);
extern EMB_PUBLIC int EMB_CALL embPolylineObjectList_count(EmbPolylineObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embPolylineObjectList_empty(EmbPolylineObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embPolylineObjectList_free(EmbPolylineObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_POLYLINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
