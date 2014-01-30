/*! @file format-rgb.h */
#ifndef FORMAT_RGB_H
#define FORMAT_RGB_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readRgb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeRgb(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_RGB_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
