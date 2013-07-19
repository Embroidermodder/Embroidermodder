#ifndef FORMAT_XXX_H
#define FORMAT_XXX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readXxx(EmbPattern* pattern, const char* fileName);
int writeXxx(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_XXX_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
