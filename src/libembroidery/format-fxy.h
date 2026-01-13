/*! @file format-fxy.h */
#ifndef FORMAT_FXY_H
#define FORMAT_FXY_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readFxy(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writeFxy(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_FXY_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
