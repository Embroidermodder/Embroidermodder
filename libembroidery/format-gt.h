#ifndef FORMAT_GT_H
#define FORMAT_GT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readGt(EmbPattern* pattern, const char* fileName);
int writeGt(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_GT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
