/*! @file format-tap.h */
#ifndef FORMAT_T01_H
#define FORMAT_T01_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readT01(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeT01(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_T01_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
