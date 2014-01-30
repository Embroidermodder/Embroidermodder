/*! @file format-pcd.h */
#ifndef FORMAT_PCD_H
#define FORMAT_PCD_H

#include "emb-pattern.h"

#include "api-start.h"
#ifdef __cplusplus
extern "C" {
#endif

extern EMB_PRIVATE int EMB_CALL readPcd(EmbPattern* pattern, const char* fileName);
extern EMB_PRIVATE int EMB_CALL writePcd(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "api-stop.h"

#endif /* FORMAT_PCD_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
