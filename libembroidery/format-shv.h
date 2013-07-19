#ifndef FORMAT_SHV_H
#define FORMAT_SHV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readShv(EmbPattern* pattern, const char* fileName);
int writeShv(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_SHV_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
