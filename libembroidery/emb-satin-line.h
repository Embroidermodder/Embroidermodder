/*! @file emb-satin-line.h */
#ifndef EMB_SATINLINE_H
#define EMB_SATINLINE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-vector.h"

typedef struct EmbSatinOutline_
{
    int length;
    EmbVector* side1;
    EmbVector* side2;
} EmbSatinOutline;

void embSatinOutline_generateSatinOutline(EmbVector lines[], int numberOfPoints, double thickness, EmbSatinOutline* result);
EmbVectorList* embSatinOutline_renderStitches(EmbSatinOutline* result, double density);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_SATINLINE_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
