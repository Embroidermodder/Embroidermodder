#ifndef FORMAT_PHB_H
#define FORMAT_PHB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readPhb(EmbPattern* pattern, const char* fileName);
int writePhb(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_PHB_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
