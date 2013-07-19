#ifndef FORMAT_100_H
#define FORMAT_100_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int read100(EmbPattern* pattern, const char* fileName);
int write100(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_100_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
