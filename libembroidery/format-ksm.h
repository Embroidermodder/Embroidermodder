#ifndef FORMAT_KSM_H
#define FORMAT_KSM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readKsm(EmbPattern* pattern, const char* fileName);
int writeKsm(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_KSM_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
