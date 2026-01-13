/*! @file emb-stitch.h */
#ifndef EMB_STITCH_H
#define EMB_STITCH_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

/* Machine codes for stitch flags */
#define NORMAL              0 /* stitch to (xx, yy) */
#define JUMP                1 /* move to(xx, yy) */
#define TRIM                2 /* trim + move to(xx, yy) */
#define STOP                4 /* pause machine for thread change */
#define SEQUIN              8 /* sequin */
#define END                 16 /* end of program */

typedef struct EmbStitch_
{
    int flags; /* uses codes defined above */
    double xx; /* absolute position (not relative) */
    double yy; /* positive is up, units are in mm  */
    int color; /* color number for this stitch */ /* TODO: this should be called colorIndex since it is not an EmbColor */
} EmbStitch;

typedef struct EmbStitchList_
{
    struct EmbStitch_ stitch;
    struct EmbStitchList_* next;
} EmbStitchList;

extern EMB_PUBLIC EmbStitchList* EMB_CALL embStitchList_create(EmbStitch data);
extern EMB_PUBLIC EmbStitchList* EMB_CALL embStitchList_add(EmbStitchList* pointer, EmbStitch data);
extern EMB_PUBLIC int EMB_CALL embStitchList_count(EmbStitchList* pointer);
extern EMB_PUBLIC int EMB_CALL embStitchList_empty(EmbStitchList* pointer);
extern EMB_PUBLIC void EMB_CALL embStitchList_free(EmbStitchList* pointer);
extern EMB_PUBLIC EmbStitch EMB_CALL embStitchList_getAt(EmbStitchList* pointer, int num);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_STITCH_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
