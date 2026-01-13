/* Smoothie G-Code */
/*! @file format-gc.h */
#ifndef FORMAT_GC_H
#define FORMAT_GC_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readGc(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeGc(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_GC_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
