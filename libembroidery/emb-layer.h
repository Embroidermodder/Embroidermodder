/*! @file emb-layer.h */
#ifndef EMB_LAYER_H
#define EMB_LAYER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-color.h"

typedef struct EmbLayer_
{
    EmbColor color;
    const char* name;
} EmbLayer;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EMB_LAYER_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
