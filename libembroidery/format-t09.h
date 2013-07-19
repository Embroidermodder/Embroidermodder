#ifndef FORMAT_T09_H
#define FORMAT_T09_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readT09(EmbPattern* pattern, const char* fileName);
int writeT09(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_T09_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
