/*! @file format-u01.h */
#ifndef FORMAT_U01_H
#define FORMAT_U01_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readU01(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeU01(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_U01_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
