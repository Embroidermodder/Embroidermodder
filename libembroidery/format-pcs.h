#ifndef FORMAT_PCS_H
#define FORMAT_PCS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readPcs(EmbPattern* pattern, const char* fileName);
int writePcs(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_PCS_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
