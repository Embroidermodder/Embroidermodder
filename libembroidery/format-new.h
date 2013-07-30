#ifndef FORMAT_NEW_H
#define FORMAT_NEW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readNew(EmbPattern* pattern, const char* fileName);
int writeNew(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_NEW_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
