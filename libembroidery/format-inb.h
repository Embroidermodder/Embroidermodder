#ifndef FORMAT_INB_H
#define FORMAT_INB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readInb(EmbPattern* pattern, const char* fileName);
int writeInb(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_INB_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
