#ifndef FORMAT_DSB_H
#define FORMAT_DSB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readDsb(EmbPattern* pattern, const char* fileName);
int writeDsb(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_DSB_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
