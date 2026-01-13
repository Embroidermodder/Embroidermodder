/*! @file emb-path.h */
#ifndef EMB_PATH_H
#define EMB_PATH_H

#include "emb-color.h"
#include "emb-point.h"
#include "emb-flag.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

/* path flag codes */
#define LINETO             0
#define MOVETO             1
#define BULGETOCONTROL     2
#define BULGETOEND         4
#define ELLIPSETORAD       8
#define ELLIPSETOEND      16
#define CUBICTOCONTROL1   32
#define CUBICTOCONTROL2   64
#define CUBICTOEND       128
#define QUADTOCONTROL    256
#define QUADTOEND        512

typedef struct EmbPathObject_
{
    EmbPointList* pointList;
    EmbFlagList* flagList;

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPathObject;

extern EMB_PUBLIC EmbPathObject* EMB_CALL embPathObject_create(EmbPointList* pointList, EmbFlagList* flagList, EmbColor color, int lineType);
extern EMB_PUBLIC void EMB_CALL embPathObject_free(EmbPathObject* pointer);

typedef struct EmbPathObjectList_
{
    EmbPathObject* pathObj;
    struct EmbPathObjectList_* next;
} EmbPathObjectList;

extern EMB_PUBLIC EmbPathObjectList* EMB_CALL embPathObjectList_create(EmbPathObject* data);
extern EMB_PUBLIC EmbPathObjectList* EMB_CALL embPathObjectList_add(EmbPathObjectList* pointer, EmbPathObject* data);
extern EMB_PUBLIC int EMB_CALL embPathObjectList_count(EmbPathObjectList* pointer);
extern EMB_PUBLIC int EMB_CALL embPathObjectList_empty(EmbPathObjectList* pointer);
extern EMB_PUBLIC void EMB_CALL embPathObjectList_free(EmbPathObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_PATH_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
