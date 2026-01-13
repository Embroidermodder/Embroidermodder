/*! @file format-inf.h */
#ifndef FORMAT_INF_H
#define FORMAT_INF_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readInf(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeInf(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_INF_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
