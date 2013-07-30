#ifndef FORMAT_MIT_H
#define FORMAT_MIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readMit(EmbPattern* pattern, const char* fileName);
int writeMit(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_MIT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
