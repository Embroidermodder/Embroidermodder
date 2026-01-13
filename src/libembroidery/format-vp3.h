/*! @file format-vp3.h */
#ifndef FORMAT_VP3_H
#define FORMAT_VP3_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readVp3(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeVp3(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_VP3_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
