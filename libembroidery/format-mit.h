/*! @file format-mit.h */
#ifndef FORMAT_MIT_H
#define FORMAT_MIT_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readMit(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeMit(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_MIT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
