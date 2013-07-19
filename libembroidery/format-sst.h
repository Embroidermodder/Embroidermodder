#ifndef FORMAT_SST_H
#define FORMAT_SST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readSst(EmbPattern* pattern, const char* fileName);
int writeSst(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_SST_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
