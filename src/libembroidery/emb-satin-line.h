/*! @file emb-satin-line.h */
#ifndef EMB_SATINLINE_H
#define EMB_SATINLINE_H

#include "emb-vector.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbSatinOutline_
{
    int length;
    EmbVector* side1;
    EmbVector* side2;
} EmbSatinOutline;

extern EMB_PUBLIC void EMB_CALL embSatinOutline_generateSatinOutline(EmbVector lines[], int numberOfPoints, double thickness, EmbSatinOutline* result);
extern EMB_PUBLIC EmbVectorList* EMB_CALL embSatinOutline_renderStitches(EmbSatinOutline* result, double density);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_SATINLINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
