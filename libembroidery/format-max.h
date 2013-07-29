#ifndef FORMAT_MAX_H
#define FORMAT_MAX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readMax(EmbPattern* pattern, const char* fileName);
int writeMax(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_MAX_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
