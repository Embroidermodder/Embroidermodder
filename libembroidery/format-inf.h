/*! @file format-inf.h */
#ifndef FORMAT_INF_H
#define FORMAT_INF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readInf(EmbPattern* pattern, const char* fileName);
int writeInf(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_INF_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
