#ifndef FORMAT_PEL_H
#define FORMAT_PEL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readPel(EmbPattern* pattern, const char* fileName);
int writePel(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_PEL_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
