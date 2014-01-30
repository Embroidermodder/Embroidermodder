/*! @file format-inb.h */
#ifndef FORMAT_INB_H
#define FORMAT_INB_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readInb(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeInb(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_INB_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
