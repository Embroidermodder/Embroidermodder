/*! @file format-t09.h */
#ifndef FORMAT_T09_H
#define FORMAT_T09_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readT09(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeT09(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_T09_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
