/*! @file format-bro.h */
#ifndef FORMAT_BRO_H
#define FORMAT_BRO_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readBro(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeBro(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_BRO_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
