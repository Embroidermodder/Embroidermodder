/*! @file format-gt.h */
#ifndef FORMAT_GT_H
#define FORMAT_GT_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readGt(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeGt(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_GT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
