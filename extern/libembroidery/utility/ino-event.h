/*! @file ino-event.h */
#ifndef INO_EVENT_H
#define INO_EVENT_H

#include "emb-pattern.h"
typedef EmbPattern InoPattern;

#ifdef __cplusplus
extern "C" {
#endif

void inoEvent_addStitchAbs(InoPattern* p, double x, double y, int flags, int isAutoColorIndex);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INO_EVENT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
