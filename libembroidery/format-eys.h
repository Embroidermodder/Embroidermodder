#ifndef FORMAT_EYS_H
#define FORMAT_EYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readEys(EmbPattern* pattern, const char* fileName);
int writeEys(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_EYS_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
