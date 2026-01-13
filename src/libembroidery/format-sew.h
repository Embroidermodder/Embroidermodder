/*! @file format-sew.h */
#ifndef FORMAT_SEW_H
#define FORMAT_SEW_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readSew(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeSew(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_SEW_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
