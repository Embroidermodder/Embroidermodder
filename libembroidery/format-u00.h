#ifndef FORMAT_U00_H
#define FORMAT_U00_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readU00(EmbPattern* pattern, const char* fileName);
int writeU00(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_U00_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
