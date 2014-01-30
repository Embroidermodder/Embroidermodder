/*! @file format-max.h */
#ifndef FORMAT_MAX_H
#define FORMAT_MAX_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readMax(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeMax(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_MAX_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
