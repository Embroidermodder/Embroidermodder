/*! @file format-pes.h */
#ifndef FORMAT_PES_H
#define FORMAT_PES_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readPes(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePes(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_PES_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
