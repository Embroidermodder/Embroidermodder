#ifndef EMB_PATH_H
#define EMB_PATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"
#include "emb-point.h"

/* flags codes */
#define LINETO             0
#define MOVETO             1
#define ARCTOMID           2
#define ARCTOEND           4
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

    /* Properties */
    int lineType;
    EmbColor color;
} EmbPathObject;

typedef struct EmbPathObjectList_
{
    EmbPathObject* pathObj;
    struct EmbPathObjectList_* next;
} EmbPathObjectList;

EmbPathObjectList* embPathObjectList_create(EmbPathObject* data);
EmbPathObjectList* embPathObjectList_add(EmbPathObjectList* pointer, EmbPathObject* data);
int embPathObjectList_count(EmbPathObjectList* pointer);
int embPathObjectList_empty(EmbPathObjectList* pointer);
void embPathObjectList_free(EmbPathObjectList* pointer);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_PATH_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
