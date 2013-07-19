#ifndef FORMAT_THR_H
#define FORMAT_THR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readThr(EmbPattern* pattern, const char* fileName);
int writeThr(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_THR_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
