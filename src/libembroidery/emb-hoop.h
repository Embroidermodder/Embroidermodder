/*! @file emb-hoop.h */
#ifndef EMB_HOOP_H
#define EMB_HOOP_H

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct EmbHoop_
{
    double width;
    double height;
} EmbHoop;

extern EMB_PUBLIC double EMB_CALL embHoop_width(EmbHoop hoop);
extern EMB_PUBLIC double EMB_CALL embHoop_height(EmbHoop hoop);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* EMB_HOOP_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
