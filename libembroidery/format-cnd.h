#ifndef FORMAT_CND_H
#define FORMAT_CND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readCnd(EmbPattern* pattern, const char* fileName);
int writeCnd(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_CND_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
