#ifndef FORMAT_DAT_H
#define FORMAT_DAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "emb-pattern.h"

int readDat(EmbPattern* pattern, const char* fileName);
int writeDat(EmbPattern* pattern, const char* fileName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FORMAT_DAT_H */

/* kate: bom off; indent-mode cstyle; indent-width 4; replace-trailing-space-save on; */
